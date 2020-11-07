// Prototype.cpp
// 原型模式用于创建重复的对象，实现对象的拷贝。这种模式类似于创建型模式，提供了创建对象的最佳模式。
// 原型模式提供一个 clone 接口来创建新的对象，避免了创建相同对象时要编写的重复代码
// cpp17 by vs2019

#include <cassert>
#include <string>
#include <memory>
#include <iostream>

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

class Prototype {
public:
	virtual std::unique_ptr<Prototype> Clone() = 0;

	void show() {
		std::cout << "str:" << str_ << std::endl;
	}

	Prototype() { str_ = ""; }
	Prototype(std::string s) : str_(s) {}
	virtual ~Prototype() = default;
private:
	std::string str_;
};

class A : public Prototype {
public:
	A() {}
	A(std::string s) : Prototype(s) {}

	std::unique_ptr<Prototype> Clone() override {
		return std::make_unique<A>(*this);
	}
};

#define TEST

#ifdef TEST

int main() {
	enable_memory_leak_check();

	std::unique_ptr<Prototype> p = std::make_unique<A>("test");
	std::unique_ptr<Prototype> q = p->Clone();
	p->show();
	q->show();
	assert(p != q);

	//int* a = new int();
	return 0;
}

#endif // TEST