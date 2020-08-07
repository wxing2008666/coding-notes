#include <iostream>
#include <sstream>
using namespace std;
//
#include "msgpack.hpp"

class myclass
{
private:
    std::string m_str_;
    std::vector<int> m_vec_int_;
public:
    myclass() = default;
    myclass(string str)
    {
        m_str_ = str;
        m_vec_int_.push_back(1);
        m_vec_int_.push_back(2);
        m_vec_int_.push_back(3);
    }

    std::string get_str()
    {
        return m_str_;
    }

    std::vector<int>& get_vec_int()
    {
        return m_vec_int_;
    }
public:
    MSGPACK_DEFINE(m_str_, m_vec_int_);
};

int main()
{
    cout<<"msgpack version:"<<MSGPACK_VERSION<<endl;
    /////////////////////////// test1
    std::tuple<int, string, bool> src(1, "hello", true);
    stringstream buffer;
    msgpack::pack(buffer, src);
    //cout<<"pack:"<<buffer.str()<<endl;
    buffer.seekg(0);
    std::string str(buffer.str());
    msgpack::object_handle handle1 = msgpack::unpack(str.data(), str.size());
    msgpack::object obj1 = handle1.get();
    cout<<"test1, unpack result:"<<obj1<<endl;
    std::tuple<int, string, bool> rsrc;
    obj1.convert(rsrc);
    cout<<"test1, convert, tuple[0]:"<<std::get<0>(rsrc)
    <<", tuple[1]:"<<std::get<1>(rsrc)
    <<", tuple[2]:"<<std::get<2>(rsrc)<<endl;

    //////////////////////////// test2
    std::vector<string> vec_test;
    vec_test.push_back("hello");
    vec_test.push_back("world");
    // serialize it into simple buffer.
    msgpack::sbuffer buf1;
    msgpack::pack(buf1, vec_test);
    // deserialize it.
    //string str1(buf1.data(), buf1.size());
    //msgpack::object_handle handle2 = msgpack::unpack(str1.data(), str1.size());
    msgpack::object_handle handle2 = msgpack::unpack(buf1.data(), buf1.size());
    msgpack::object obj2 = handle2.get();
    cout<<"test2, unpack result:"<<obj2<<endl;
    // convert it into statically typed object.
    std::vector<string> rvec;
    obj2.convert(rvec);
    cout<<"test2, convert, vec[0]:"<<rvec[0]<<", vec[1]:"<<rvec[1]<<endl;

    //////////////////////////// test3
    msgpack::sbuffer buf2;
    msgpack::packer<msgpack::sbuffer> pk2(&buf2);
    pk2.pack(1);
    pk2.pack(std::string("hello"));
    pk2.pack(3.1415926);
    pk2.pack_float(3.1415926);
    pk2.pack_double(3.1415926);
    pk2.pack_long(1234567890);
    // feeds the buffer.
    msgpack::unpacker unper;
    unper.reserve_buffer(buf2.size());
    memcpy(unper.buffer(), buf2.data(), buf2.size());
    unper.buffer_consumed(buf2.size());
    // now starts streaming deserialization.
    msgpack::object_handle handle3;
    cout<<"test3, ";
    while (unper.next(handle3))
        cout<<"data:"<<handle3.get()<<" ";
    cout<<endl;

    //////////////////////////// test4
    msgpack::sbuffer buf4;
    msgpack::packer<msgpack::sbuffer> pk4(&buf4);
    pk4.pack_array(3);
    pk4.pack("123");
    pk4.pack("456");
    pk4.pack("789");
    pk4.pack_map(2);
    pk4.pack(string("x"));
    pk4.pack(6);
    pk4.pack(string("y"));
    pk4.pack(7);
    pk4.pack("end");
    // feeds the buffer.
    msgpack::unpacker unper4;
    unper4.reserve_buffer(buf4.size());
    memcpy(unper4.buffer(), buf4.data(), buf4.size());
    unper4.buffer_consumed(buf4.size());
    // now starts streaming deserialization.
    msgpack::object_handle handle4;
    cout<<"test4, ";
    while (unper4.next(handle4))
        cout<<"data:"<<handle4.get()<<" ";
    cout<<endl;

    //////////////////////////// test5
    //use serialize/deserializes user-defined classes using MSGPACK_DEFINE macro.
    std::vector<myclass> vec_class;
    myclass s("test");
    vec_class.push_back(s);
    // you can serialize myclass directly
    msgpack::sbuffer buf5;
    msgpack::pack(buf5, vec_class);
    // deserialize it.
    msgpack::object_handle handle5 = msgpack::unpack(buf5.data(), buf5.size());
    msgpack::object obj5 = handle5.get();
    // you can convert object to myclass directly
    std::vector<myclass> rvec_class;
    obj5.convert(rvec_class);
    cout<<"test5, deserialize str:"<<rvec_class[0].get_str()<<endl;
    cout<<"test5, deserialize vec:"<<rvec_class[0].get_vec_int()[0]<<endl;
    
    return 0;
}
