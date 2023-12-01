#pragma once

#include "SystemBase.h"

namespace ecs
{
	class World;

	class TestSystem : public SystemBase
	{
	public:
		TestSystem(World* _pWorld)
			: SystemBase(_pWorld)
		{}

		void Init() override;
		void Update(float deltaTime) override;
	};
}