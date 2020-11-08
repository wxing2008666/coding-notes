// AbstractFactory.cpp, 抽象工厂模式
// cpp17 by vs2019

#include <memory>

class Door {
public:
	virtual ~Door() = default;
};

class Window {
public:
	virtual ~Window() = default;
};

class House {
public:
	virtual void SetDoor(const Door&) {}
	virtual void SetWindow(const Window&) {}
	virtual ~House() = default;
};

class HouseFactory {
public:
	virtual std::unique_ptr<Door> MakeDoor() {
		return std::make_unique<Door>();
	}

	virtual std::unique_ptr<Window> MakeWindow() {
		return std::make_unique<Window>();
	}

	virtual std::unique_ptr<House> MakeHouse() {
		return std::make_unique<House>();
	}

	virtual ~HouseFactory() = default;
};

class Life {
public:
	std::unique_ptr<House> CreateHouse(HouseFactory& factory) {
		std::unique_ptr<Door> door = factory.MakeDoor();
		std::unique_ptr<Window> window = factory.MakeWindow();
		std::unique_ptr<House> house = factory.MakeHouse();
		house->SetDoor(*door);
		house->SetWindow(*window);
		return house;
	}
};

class MyDoor : public Door {};
class MyWindow : public Window {};
class MyHose : public House {};

class MyHouseFactory : public HouseFactory {
public:
	std::unique_ptr<Door> MakeDoor() override {
		return std::unique_ptr<MyDoor>();
	}

	std::unique_ptr<Window> MakeWindow() override {
		return std::make_unique<MyWindow>();
	}

	std::unique_ptr<House> MakeHouse() override {
		return std::make_unique<House>();
	}
};

#define TEST

#ifdef TEST

int main() {
	Life life;
	MyHouseFactory factor;

	std::unique_ptr<House> my_house = life.CreateHouse(factor);
	return 0;
}

#endif // TEST