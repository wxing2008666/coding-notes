// Mediator.cpp, 中介者模式：用一个中介对象来封装一系列的对象交互。
// 中介者使各对象不需要显式地相互引用，从而使其耦合松散，而且可以独立地改变它们之间的交互。
// cpp17 by vs2019

#include <iostream>
#include <memory>
#include <string_view>
#include <utility>
#include <list>

// add for memory leak test
// _CrtSetDbgFlag在程序退出时调用 _CrtDumpMemoryLeaks()
#include <crtdbg.h>
inline void enable_memory_leak_check() {
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

// when use new operator, can print filename and line num
#ifdef _DEBUG
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//
class Colleague;

class Mediator {
public:
	virtual void Notify(const std::shared_ptr<Colleague>&, std::string_view) = 0;
	virtual ~Mediator() = default;
};

class Colleague {
public:
	explicit Colleague(const std::shared_ptr<Mediator>& p) : mediator_(p) {}
	virtual void Send(std::string_view) = 0;
	virtual void Receive(std::string_view) const = 0;
	virtual ~Colleague() = default;
protected:
	std::weak_ptr<Mediator> mediator_;
};

class ColleagueA : public Colleague,
	public std::enable_shared_from_this<ColleagueA> {
public:
	using Colleague::Colleague;

	void Send(std::string_view s) override {
		if (const auto p = mediator_.lock()) {
			p->Notify(shared_from_this(), s);
		}
	}

	void Receive(std::string_view s) const override {
		std::cout << "A receive:" << s << std::endl;
	}
};

class ColleagueB : public Colleague,
	public std::enable_shared_from_this<ColleagueB> {
public:
	using Colleague::Colleague;

	void Send(std::string_view s) override {
		mediator_.lock()->Notify(shared_from_this(), s);
	}

	void Receive(std::string_view s) const override {
		std::cout << "B receive:" << s << std::endl;
	}
};

class ConcreteMediator : public Mediator {
public:
	void Append(const std::shared_ptr<Colleague>& p) {
		colleagues_.emplace_back(p);
	}

	void Notify(const std::shared_ptr<Colleague>& c, std::string_view s) override {
		for (auto&& x : colleagues_) {
			if (const auto p = x.lock()) {
				if (p != c) {
					p->Receive(s);
				}
			}
		}
	}
private:
	std::list<std::weak_ptr<Colleague>> colleagues_;
};

//
#define TEST

#ifdef TEST

int main() {
	enable_memory_leak_check();

	auto mediator = std::make_shared<ConcreteMediator>();
	auto colleagueA = std::make_shared<ColleagueA>(mediator);
	auto colleagueB = std::make_shared<ColleagueB>(mediator);

	mediator->Append(colleagueA);
	mediator->Append(colleagueB);

	colleagueA->Send("A");
	colleagueB->Send("B");

	return 0;
}

#endif // TEST