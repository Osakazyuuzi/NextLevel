
#include <Windows.h>
#include <crtdbg.h>
#include <memory>
#include <chrono>
#include <iostream>

#include "core/Engine.h"

#include "core/ecs/Entity.h"
#include "core/ecs/Archetype.h"
#include "core/ecs/Chunk.h"
#include "core/ecs/World.h"
#include "core/ecs/IComponentData.h"
#include "core/ecs/SystemBase.h"
#include "core/ecs/EntityManager.h"

#include "core/ecs/TestComponent.h"
#include "core/ecs/TestSystem.h"

using namespace ecs;


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int   nCmdShow)
{
	// メモリリーク検出（デバッグビルド時のみ）
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Engine engine;

	//engine.Run();

	ecs::Archetype arche{};
	//arche.AddType<Test>();

	ecs::World world;

	world.AddSystem<TestSystem>(0);

	for (int i = 0; i < 100; i++)
	{
		auto entity = world.GetEntityManager()->CreateEntity();
	}

	for (int i = 0; i < 100; i++)
	{
		world.Update(0.0f);
	}


	return 0;
}

int main(void)
{
	ecs::World world;

	world.AddSystem<TestSystem>(0);

	int num = 1000;

	for (int i = 0; i < num; i++)
	{
		auto entity = world.GetEntityManager()->CreateEntity();
	}

	//===

	for (int i = 0; i < 5; i++)
	{

		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num; i++)
		{
			world.Update(0.0f);
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


		std::cout << "Update処理時間: " << duration << "ミリ秒" << std::endl;
	}


	rewind(stdin);
	getchar();

	return 0;
}