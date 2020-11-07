// Singleton.cpp
// cpp17 by vs2019

#include <cassert>

template<typename T>
class Singleton {
public:
	static T& Instance();
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

private:
	Singleton() = default;
	~Singleton() = default;
};

template<typename T>
T& Singleton<T>::Instance() {
	static T instance;
	return instance;
}

class A{};

#define TEST

#ifdef TEST

int main() {
	auto &a = Singleton<A>::Instance();
	auto &b = Singleton<A>::Instance();
	assert(&a == &b);
	return 0;
}

#endif // TEST