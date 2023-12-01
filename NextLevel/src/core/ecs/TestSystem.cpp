#include "TestSystem.h"

#include "EntityManager.h"
#include "TestComponent.h"
#include "Chunk.h"

namespace ecs
{
	void TestSystem::Init()
	{
		m_Archetype.AddType<Test>();
	}

	void TestSystem::Update(float deltaTime)
	{
		auto chunkList = GetEntityManager()->GetContainChunkList(m_Archetype);
		for (auto&& chunk : chunkList)
		{
			auto compList = chunk->GetComponentList<Test>();
			for (std::uint32_t i = 0; i < chunk->GetSize(); i++)
			{
				compList[(int)i].value++;
			}
		}
	}
}