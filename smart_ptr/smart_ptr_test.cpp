// smart_ptr_test.cpp : 用于测试智能指针的使用以及检测是否有内存泄露的问题
// test and run by vs2019
//

#include <iostream>
#include <memory>

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

// add test code
class B;
class A {
public:
    std::shared_ptr<B> p_b_;
};

class B {
public:
    //std::weak_ptr<A> p_a_;
    std::shared_ptr<A> p_a_;
};

int main() {
    enable_memory_leak_check();

    auto a = std::make_shared<A>();
    a->p_b_ = std::make_shared<B>();
    a->p_b_->p_a_ = a;

    //int* c = new int();

    //_CrtDumpMemoryLeaks();

    return 0;
}
