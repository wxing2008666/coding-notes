#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <set>

class TestPtr {
public:
    TestPtr() {
        std::cout << "default construct" << std::endl;
    }

    TestPtr(int n) : num_(n) {
        std::cout << "construct, num:" << num_ << std::endl;
    }

    ~TestPtr() {
        std::cout << "delete, " << num_ << std::endl;
    }

    TestPtr(const TestPtr&) {
        std::cout << "copy" << std::endl;
    }

    TestPtr& operator= (const TestPtr&) {
        std::cout << "operator=" << std::endl;
    }

    TestPtr(TestPtr&&) {
        std::cout << "move construct" << std::endl;
    }

    TestPtr& operator= (TestPtr&&) {
        std::cout << "move operator=" << std::endl;
    }

    int num() const {return num_;}

private:
    int num_;
};


void display_vector1(std::vector<std::unique_ptr<TestPtr>>& vec)
{
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        // std::cout << (*it)->num() << std::endl;
    }
}

void display_vector2(std::vector<std::unique_ptr<TestPtr>> vec)
{
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        // std::cout << (*it)->num() << std::endl;
    }
}

void display_vector3(std::vector<std::shared_ptr<TestPtr>>& vec)
{
    if (vec.size() >= 2) {
        std::cout << "param count in func:" << vec[1].use_count() << std::endl;
    }

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        std::cout << (*it)->num() << std::endl;
    }
}

void display_vector4(std::vector<std::shared_ptr<TestPtr>> vec)
{
    if (vec.size() >= 2) {
        std::cout << "param count in func:" << vec[1].use_count() << std::endl;
    }

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        std::cout << (*it)->num() << std::endl;
    }
}

std::unique_ptr<TestPtr> ret_func() {
    std::unique_ptr<TestPtr> ret(new TestPtr(33));
    std::cout << "in ret_func" << std::endl;
    // return std::move(ret);
    return ret; // the same effect as above
}

std::shared_ptr<TestPtr> ret_func_shared() {
    std::shared_ptr<TestPtr> ret(new TestPtr(44));
    std::cout << "in ret_func_shared" << std::endl;
    // return std::move(ret);
    return ret; // the same effect as above
}

struct C {int* data;};

struct C1 : public std::enable_shared_from_this<C1> {};


int main() {
    // unique_ptr
    std::cout << "======unique ptr======" << std::endl;
    {
        // base
        std::unique_ptr<TestPtr> test_ptr(new TestPtr);

        // reset will hold new object and drop old object
        // Destroys the object currently managed by the unique_ptr (if any) and takes ownership of new object
        test_ptr.reset(new TestPtr(5));
        std::cout << "unique_ptr reset" << std::endl;

        // operator = will hold new object and drop old object
        test_ptr = std::unique_ptr<TestPtr>(new TestPtr(10));
        std::cout << "operator=" << std::endl;

        // allowed, move
        std::unique_ptr<TestPtr> test_ptr_move = std::move(test_ptr);
        std::cout << "operator= move" << std::endl;


        // not allowed, copy construct
        // std::unique_ptr<TestPtr> ttt(test_ptr);
        // not allowed, copy assignment
        // std::unique_ptr<TestPtr> ttt = test_ptr;
        // allowed, construct
        // std::unique_ptr<TestPtr> ttt = std::unique_ptr<TestPtr>(new TestPtr(10));

        // get, Returns the stored pointer.
        TestPtr* p_tmp = test_ptr_move.get();
        *p_tmp = 20;
        std::cout << "get" << std::endl;

        // release
        // Releases ownership of its stored pointer, by returning its value and replacing it with a null pointer
        TestPtr* p_tmp1 = test_ptr_move.release();
        std::cout << "release" << std::endl;
        // after release, This call does not destroy the managed object，so must manual release
        delete p_tmp1;

        // swap
        std::unique_ptr<int> foo(new int(10));
        std::unique_ptr<int> bar(new int(20));
        foo.swap(bar);

        std::cout << std::endl;
        std::cout << "STL" << std::endl;

        // STL
        {
            // vector
            std::vector<std::unique_ptr<TestPtr>> vec_ptr;
            // not allowed, copy construct && copy assignment
            // vec_ptr.push_back(new TestPtr(30));
            std::unique_ptr<TestPtr> v_a(new TestPtr(30));
            vec_ptr.push_back(std::move(v_a));
            vec_ptr.push_back(std::unique_ptr<TestPtr>(new TestPtr(40)));

            // map
            std::map<int, std::unique_ptr<TestPtr>> map_ptr;
            map_ptr[123] = std::unique_ptr<TestPtr>(new TestPtr(50));
            map_ptr[456] = std::unique_ptr<TestPtr>(new TestPtr(60));

            // set
            std::set<std::unique_ptr<TestPtr>> set_ptr;
            set_ptr.insert(std::unique_ptr<TestPtr>(new TestPtr(70)));
            set_ptr.insert(std::unique_ptr<TestPtr>(new TestPtr(80)));
        }

        std::cout << std::endl;
        std::cout << "FUNC" << std::endl;
        // func, 作为函数参数或者函数返回值

        std::vector<std::unique_ptr<TestPtr>> vec_ptr;
        std::unique_ptr<TestPtr> f_a(new TestPtr(11));
        vec_ptr.push_back(std::move(f_a));
        vec_ptr.push_back(std::unique_ptr<TestPtr>(new TestPtr(22)));
        display_vector1(vec_ptr);
        std::cout << "now, vec size:" << vec_ptr.size() << std::endl;
        display_vector2(std::move(vec_ptr)); // 转移所有权, 这个函数结束就释放资源
        std::cout << "now, vec size:" << vec_ptr.size() << std::endl;

        std::unique_ptr<TestPtr> ret = ret_func();
        std::cout << "get ptr from func return" << std::endl;
    }
    std::cout << "======unique ptr======" << std::endl;
    std::cout << std::endl;
    
    // shared_ptr
    std::cout << "======shared ptr======" << std::endl;
    {
        // base
        {
            // Construct shared_ptr
            std::shared_ptr<TestPtr> s1;
            std::shared_ptr<TestPtr> s2(nullptr);
            std::shared_ptr<TestPtr> s3(new TestPtr(111));
            std::cout << "s3 count:" << s3.use_count() << std::endl;
            std::shared_ptr<TestPtr> s4(s3);
            std::cout << "s3 count:" << s3.use_count() << std::endl;
            std::shared_ptr<TestPtr> s5(std::move(s4));
            std::cout << "s3 count:" << s3.use_count() << std::endl;

            std::unique_ptr<TestPtr> u1(new TestPtr(222));
            std::shared_ptr<TestPtr> s6(std::move(u1));
            // std::shared_ptr<TestPtr> s6(std::unique_ptr<TestPtr>(new TestPtr(222)));
            std::cout << "s6 count:" << s6.use_count() << std::endl;

            std::shared_ptr<C> s7(new C);
            std::cout << "s7 count:" << s7.use_count() << std::endl;
            std::shared_ptr<int> s8(s7, s7->data);
            std::cout << "s7 count:" << s7.use_count() << std::endl;
            std::cout << "s8 count:" << s8.use_count() << std::endl;

            // copy construct
            std::shared_ptr<TestPtr> s9;
            s9 = s3; // copy
            std::cout << "s3 count:" << s3.use_count() << std::endl;

            // s6.swap(s3);
            // std::cout << "swap" << std::endl;
            // std::cout << "s3 count:" << s3.use_count() << std::endl;
            // std::cout << "s6 count:" << s6.use_count() << std::endl;

            std::shared_ptr<TestPtr> s10(new TestPtr(333));
            std::cout << "s10 count:" << s10.use_count() << std::endl;
            std::cout << "s10, value:" << s10->num() << std::endl;
            s10 = std::make_shared<TestPtr>(TestPtr(444)); // move class
            std::cout << "s10 count:" << s10.use_count() << std::endl;
            std::cout << "s10 value:" << s10->num() << std::endl;
            std::shared_ptr<int> bar(new int(10));
            std::cout << "bar count:" << bar.use_count() << std::endl;
            std::cout << "bar, value:" << *bar << std::endl;
            bar = std::make_shared<int>(20);   // move int
            std::cout << "bar count:" << bar.use_count() << std::endl;
            std::cout << "bar, value:" << *bar << std::endl;
            // move from unique_ptr
            std::shared_ptr<TestPtr> s11;
            std::cout << "s11 count:" << s11.use_count() << std::endl;
            std::unique_ptr<TestPtr> s_u1(new TestPtr(555));
            s11 = std::move(s_u1);
            std::cout << "s11 count:" << s11.use_count() << std::endl;

            s11.reset(new TestPtr(666));
            std::cout << "s11 count:" << s11.use_count() << std::endl;
            auto p_get = s11.get();
            std::cout << "s11 value:" << p_get->num() << std::endl;
            std::cout << "s11 unique:" << s11.unique() << std::endl;
        }

        std::cout << std::endl;
        std::cout << "STL" << std::endl;

        // STL
        {
            // vector
            std::vector<std::shared_ptr<TestPtr>> vec_ptr;
            // not allowed, copy construct && copy assignment
            // vec_ptr.push_back(new TestPtr(30));
            std::shared_ptr<TestPtr> v_a(new TestPtr(10));
            // vec_ptr.push_back(std::move(v_a));
            vec_ptr.push_back(v_a);
            vec_ptr.push_back(std::shared_ptr<TestPtr>(new TestPtr(20)));
            // map
            std::map<int, std::shared_ptr<TestPtr>> map_ptr;
            map_ptr[123] = std::shared_ptr<TestPtr>(new TestPtr(30));
            map_ptr[456] = std::shared_ptr<TestPtr>(new TestPtr(40));
            // set
            std::set<std::shared_ptr<TestPtr>> set_ptr;
            set_ptr.insert(std::shared_ptr<TestPtr>(new TestPtr(50)));
            set_ptr.insert(std::shared_ptr<TestPtr>(new TestPtr(60)));
        }

        std::cout << std::endl;
        std::cout << "FUNC" << std::endl;
        // func, 作为函数参数或者函数返回值

        std::vector<std::shared_ptr<TestPtr>> vec_ptr;
        std::shared_ptr<TestPtr> f_a(new TestPtr(11));
        vec_ptr.push_back(std::move(f_a));
        vec_ptr.push_back(std::shared_ptr<TestPtr>(new TestPtr(22)));
        std::cout << "param count:" << vec_ptr[1].use_count() << std::endl;
        display_vector3(vec_ptr);
        std::cout << "now, vec size:" << vec_ptr.size() << std::endl;
        std::cout << "param count:" << vec_ptr[1].use_count() << std::endl;
        // display_vector4(std::move(vec_ptr)); // 转移所有权, 这个函数结束就释放资源
        display_vector4(vec_ptr);
        std::cout << "now, vec size:" << vec_ptr.size() << std::endl;
        std::cout << "param count:" << vec_ptr[1].use_count() << std::endl;

        std::shared_ptr<TestPtr> ret = ret_func();
        std::cout << "get ptr from func return" << std::endl;
        std::shared_ptr<TestPtr> ret_shared = ret_func_shared();
        std::cout << "get ptr from func shared return" << std::endl;
    }
    std::cout << "======shared ptr======" << std::endl;
    std::cout << std::endl;

    // weak_ptr
    std::cout << "======weak ptr======" << std::endl;
    {
        // base
        {
            // construct
            std::shared_ptr<TestPtr> s1(new TestPtr(100));

            std::weak_ptr<TestPtr> wp1;
            std::weak_ptr<TestPtr> wp2(wp1);
            std::weak_ptr<TestPtr> wp3(s1);
            std::cout << "wp1 count:" << wp1.use_count() << std::endl;
            std::cout << "wp2 count:" << wp2.use_count() << std::endl;
            std::cout << "wp3 count:" << wp3.use_count() << std::endl;
            std::weak_ptr<TestPtr> wp4(wp3);
            std::cout << "wp4 count:" << wp4.use_count() << std::endl;
            std::cout << "s1 count:" << s1.use_count() << std::endl;
            // operator =
            std::shared_ptr<TestPtr> sp1, sp2;
            std::weak_ptr<TestPtr> wp5;
            sp1 = std::make_shared<TestPtr>(TestPtr(200));
            wp5 = sp1;
            std::cout << "wp5 count:" << wp5.use_count() << std::endl;
            std::cout << "sp1 count:" << sp1.use_count() << std::endl;
            std::cout << "sp2 count:" << sp2.use_count() << std::endl;
            std::cout << std::endl;
            sp2 = wp5.lock();
            std::cout << "wp5 count:" << wp5.use_count() << std::endl;
            std::cout << "sp1 count:" << sp1.use_count() << std::endl;
            std::cout << "sp2 count:" << sp2.use_count() << std::endl;
            std::cout << std::endl;
            sp1.reset();
            std::cout << "wp5 count:" << wp5.use_count() << std::endl;
            std::cout << "sp1 count:" << sp1.use_count() << std::endl;
            std::cout << "sp2 count:" << sp2.use_count() << std::endl;
            std::cout << std::endl;
            sp1 = wp5.lock();
            std::cout << "wp5 count:" << wp5.use_count() << std::endl;
            std::cout << "sp1 count:" << sp1.use_count() << std::endl;
            std::cout << "sp2 count:" << sp2.use_count() << std::endl;
            std::cout << "wp5 expired?:" << wp5.expired() <<std::endl;
            wp5.reset();
            std::cout << "wp5 count:" << wp5.use_count() << std::endl;
            std::cout << "sp1 count:" << sp1.use_count() << std::endl;
            std::cout << "sp2 count:" << sp2.use_count() << std::endl;
            std::cout << "wp5 expired?:" << wp5.expired() <<std::endl;
        }

        std::cout << std::endl;
        std::cout << "STL" << std::endl;
        // STL
        {
            std::vector<std::weak_ptr<TestPtr>> vec_ptr;
            std::shared_ptr<TestPtr> sp1(new TestPtr(11));
            std::weak_ptr<TestPtr> wp1(sp1);
            vec_ptr.push_back(wp1);
            std::cout << "=====" << std::endl;
        }

        std::cout << std::endl;
        std::cout << "FUNC" << std::endl;
        // func, 作为函数参数或者函数返回值
    }
    std::cout << "======weak ptr======" << std::endl;

    std::shared_ptr<C1> p1, p2;
    p1 = std::make_shared<C1>();
    p2 = p1->shared_from_this();
    if (!p1.owner_before(p2) && !p2.owner_before(p1))
        std::cout << "p1 and p2 share ownship" << std::endl;

    return 0;
}