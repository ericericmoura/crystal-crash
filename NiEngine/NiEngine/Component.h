#pragma once

#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>

namespace ni {

class Component
{
protected:
	Id<GameObjectTag> owner_id_;
};

}