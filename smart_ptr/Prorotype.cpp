// Prototype.cpp
// ԭ��ģʽ���ڴ����ظ��Ķ���ʵ�ֶ���Ŀ���������ģʽ�����ڴ�����ģʽ���ṩ�˴�����������ģʽ��
// ԭ��ģʽ�ṩһ�� clone �ӿ��������µĶ��󣬱����˴�����ͬ����ʱҪ��д���ظ�����
// cpp17 by vs2019

#include <cassert>
#include <string>
#include <memory>
#include <iostream>

// add for memory leak test
// _CrtSetDbgFlag�ڳ����˳�ʱ���� _CrtDumpMemoryLeaks()
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