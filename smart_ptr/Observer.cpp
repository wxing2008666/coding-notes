// Observer.cpp, 观察者模式
// 定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新
// cpp17 by vs2019

#include <memory>
#include <iostream>
#include <cstdint>
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
class Subject;

class Observer {
public:
	explicit Observer(const std::shared_ptr<Subject> &p) : subject_(p) {}
	virtual void Update() = 0;
	virtual ~Observer() = default;
protected:
	std::weak_ptr<Subject> subject_;
};

class Subject {
public:
	void Append(const std::shared_ptr<Observer>& o) {
		observers_.emplace_back(o);
	}

	void Remove(const std::shared_ptr<Observer>& o) {
		observers_.remove(o);
	}

	void Notify() {
		for (auto&& o : observers_) {
			o->Update();
		}
	}

	void SetValue(std::int32_t n) { value_ = n; }
	std::int32_t GetValue() const { return value_; }
private:
	std::int32_t value_ = 0;
	std::list<std::shared_ptr<Observer>> observers_;
};

class ObserverA : public Observer {
public:
	using Observer::Observer;

	void Update() override {
		if (const auto p = subject_.lock()) {
			std::cout << "Subject value in A:" << p->GetValue() << std::endl;
		}
	}
};

class ObserverB : public Observer {
public:
	using Observer::Observer;

	void Update() override {
		if (const auto p = subject_.lock()) {
			std::cout << "Subjecct value in B:" << p->GetValue() << std::endl;
		}
	}
};

//
#define TEST

#ifdef TEST

int main() {
	enable_memory_leak_check();

	auto subject = std::make_shared<Subject>();
	auto a = std::make_shared<ObserverA>(subject);
	auto b = std::make_shared<ObserverB>(subject);

	subject->Append(a);
	subject->Append(b);

	subject->SetValue(3);
	subject->Notify();

	return 0;
}

#endif // TEST