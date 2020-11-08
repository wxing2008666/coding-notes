// Command.cpp, 命令模式将请求封装成对象，解耦了调用者与处理者
// 角色描述：
// Receiver接受者角色，就是干活的码农，命令传递打这里就应该被执行。
// Command命令角色，对命令的封装，所有命令声明在此
// Invoker调用者角色，接受到命令并且执行命令
// cpp17 by vs2019

#include <memory>
#include <iostream>
#include <functional>
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
class Receiver {
public:
	void Print() { std::cout << "working" << std::endl; }
};

class Command {
public:
	virtual void Execute() = 0;
	virtual ~Command() = default;
};

template<typename T>
class CommandA : public Command {
public:
	using Action = std::function<void(T&)>;

	CommandA(const std::shared_ptr<T>& p, Action a) :
		receiver_(p), action_(std::move(a)) {}

	void Execute() override {
		std::cout << "A" << std::endl;
		if (const auto p = receiver_.lock()) {
			action_(*p);
		}
	}
private:
	std::weak_ptr<T> receiver_;
	Action action_;
};

template<typename T>
class CommandB : public Command {
public:
	using Action = std::function<void(T&)>;

	CommandB(const std::shared_ptr<T>& p, Action a) :
		receiver_(p), action_(std::move(a)) {}

	void Execute() override {
		std::cout << "B" << std::endl;
		if (const auto p = receiver_.lock()) {
			action_(*p);
		}
	}
private:
	std::weak_ptr<T> receiver_;
	Action action_;
};

class Invoker : public Command {
public:
	void Append(const std::shared_ptr<Command>& c) {
		commands_.emplace_back(c);
	}

	void Remove(const std::shared_ptr<Command>& c) {
		commands_.remove_if(
			[&](std::weak_ptr<Command>& x) {return x.lock() == c; }
		);
	}

	void Execute() override {
		for (auto&& c : commands_) {
			if (const auto p = c.lock()) {
				p->Execute();
			}
		}
	}
private:
	std::list<std::weak_ptr<Command>> commands_;
};

//
#define TEST

#ifdef TEST

int main() {
	enable_memory_leak_check();

	auto receiver = std::make_shared<Receiver>();
	const std::shared_ptr<Command> commandA = std::make_shared<CommandA<Receiver>>(receiver, &Receiver::Print);
	const std::shared_ptr<Command> commandB = std::make_shared<CommandB<Receiver>>(receiver, &Receiver::Print);

	Invoker invoker;
	invoker.Append(commandA);
	invoker.Append(commandB);
	invoker.Execute();
	return 0;
}

#endif // TEST