// Builder.cpp
// ��һ�����Ӷ���Ĺ��������ı�ʾ���룬ʹ��ͬ���Ĺ������̿��Դ�����ͬ�ı�ʾ��
// �ڡ������ģʽ��һ���У�������һ���ܺõ����ӡ�����������С�ˡ�
// ����һ��С�ˣ�Ҫ��Ϊ������ͷ�������塢���֡����֡���ź��ҽš�
// ����󹤳�ģʽ��ͬ���ǣ�������ģʽ����Director�Ŀ�����һ��һ���Ĺ�������ģ�
// �ڽ���Ĺ����У�������ģʽ���Խ��и���ϸ�Ŀ��ơ�
// �����˵�ͷ�������塢���֡����֡���Ż����ҽ���α仯���������ջ������⼸���������һ���γ�һ���ˣ�
// ��Ȼ��ͬһ��������̣���������˾ͻ��в�ͬ�ı�ʾ�����磬���ӣ����ӣ����ߵģ����͵ĵȵȡ�
// cpp17 by vs2019

#include <memory>
#include <string_view>

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