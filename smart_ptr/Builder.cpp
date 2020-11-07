// Builder.cpp
// 将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。
// 在《大话设计模式》一书中，例举了一个很好的例子————建造小人。
// 建造一个小人，要分为六步：头部、身体、左手、右手、左脚和右脚。
// 与抽象工厂模式不同的是，建造者模式是在Director的控制下一步一步的构造出来的，
// 在建造的过程中，建造者模式可以进行更精细的控制。
// 不管人的头部、身体、左手、右手、左脚或者右脚如何变化，但是最终还是由这几部分组合在一起形成一个人，
// 虽然是同一个建造过程，但是这个人就会有不同的表示，比如，胖子，瘦子，个高的，个低的等等。
// cpp17 by vs2019

#include <memory>
#include <string_view>

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

class DeviceSuite {
public:
	void SetMouse(std::string_view) {}
	void SetKeyboard(std::string_view) {}
	void SetHeadphone(std::string_view) {}
};

class GameDevice {
public:
	virtual void BuildMouse() = 0;
	virtual void BuildKeyboard() = 0;
	virtual void BuildHeadphone() = 0;
	virtual DeviceSuite& Device() const = 0;
	virtual ~GameDevice() = default;
};

class LOLGameDevice : public GameDevice {
public:
	LOLGameDevice() : device_(std::make_unique<DeviceSuite>()) {}

	void BuildMouse() override { device_->SetMouse("Logitech"); }
	void BuildKeyboard() override { device_->SetKeyboard("Filco"); }
	void BuildHeadphone() override { device_->SetHeadphone("Sen"); }
	DeviceSuite& Device() const override { return *device_; }
private:
	std::unique_ptr<DeviceSuite> device_;
};

class FootballGameDevice : public GameDevice {
public:
	FootballGameDevice() : device_(std::make_unique<DeviceSuite>()) {}

	void BuildMouse() override { device_->SetMouse("kMouse"); }
	void BuildKeyboard() override { device_->SetKeyboard("Cheery"); }
	void BuildHeadphone() override { device_->SetHeadphone("pHone"); }
	DeviceSuite& Device() const override { return *device_; }
private:
	std::unique_ptr<DeviceSuite> device_;
};

class Life {
public:
	DeviceSuite& CreateDevice(GameDevice& builder) {
		builder.BuildMouse();
		builder.BuildKeyboard();
		builder.BuildHeadphone();
		return builder.Device();
	}
};

#define TEST

#ifdef TEST

int main() {
	enable_memory_leak_check();

	Life life;
	LOLGameDevice lol_game;
	FootballGameDevice ball_game;

	DeviceSuite lol_device = life.CreateDevice(lol_game);
	DeviceSuite ball_device = life.CreateDevice(ball_game);

	//int* test = new int();
	return 0;
}

#endif // TEST