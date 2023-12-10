
#include <memory>

#include "src/common/core/PlatformDefinitions.h"
#include "src/platform/WindowManager.h"
#include "src/common/core/Application.h"
#include "src/common/core/utils/CustomException.h"

using namespace NextLevel;

#if PLATFORM_WINDOWS
#include <Windows.h>

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int   nCmdShow)
{
	// ウィンドウ初期化に必要な設定
	WindowManager::SetHInstance(hInstance);
#else
int main(int argc, char* argv[])
{
#endif

	std::unique_ptr<Application> app = std::make_unique<Application>();
	app->Init();

	app->Run();

	app->Uninit();

#if PLATFORM_WINDOWS
	return 0;
}
#else
}
#endif