// Composite.cpp, 组合模式
// 组合模式同意你将对象组合成树形结构来表现“总体/部分”层次结构。组合能让客户以一致的方式处理个别对象以及对象组合。
// cpp17 by vs2019

#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>

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
class Shape {
public:
	virtual void Display() = 0;
	virtual void Append(const std::shared_ptr<Shape>&) {}
	virtual void Remove(const std::shared_ptr<Shape>&) {}

	virtual std::shared_ptr<Shape> Child(std::size_t) { return nullptr; }

	virtual ~Shape() = default;
};

class Line : public Shape {
public:
	void Display() override {
		std::cout << "Line" << std::endl;
	}
};

class Rect : public Shape {
public:
	void Display() override {
		std::cout << "Rect" << std::endl;
	}
};

class Picture : public Shape {
public:
	void Display() override {
		for (auto&& shape : shapes_) {
			shape->Display();
		}
	}

	void Append(const std::shared_ptr<Shape>& shape) override {
		shapes_.emplace_back(shape);
	}

	void Remove(const std::shared_ptr<Shape>& s) override {
		if (const auto it = std::find(std::begin(shapes_), std::end(shapes_), s); it != std::end(shapes_)) {
			shapes_.erase(it);
		}
	}

	std::shared_ptr<Shape> Child(std::size_t n) override { return shapes_.at(n); }
private:
	std::vector<std::shared_ptr<Shape>> shapes_;
};

//
#define TEST

#ifdef TEST

int main() {
	enable_memory_leak_check();

	std::unique_ptr<Picture> pic = std::make_unique<Picture>();
	const auto line = std::make_shared<Line>();
	const auto rect = std::make_shared<Rect>();

	pic->Append(line);
	pic->Append(rect);
	pic->Display();

	pic->Remove(rect);
	pic->Display();

	return 0;
}

#endif // TEST