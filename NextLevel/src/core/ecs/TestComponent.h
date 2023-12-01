#pragma once

#include "IComponentData.h"

namespace ecs
{
	struct Test : public IComponentData
	{
		int value = 0;
	};

}