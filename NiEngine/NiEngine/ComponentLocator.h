#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "GameObjectTag.h"
#include "GraphicsComponent.h"
#include "AnimatedGraphicsComponent.h"
#include "Id.h"

namespace ni {

class UpdateComponent;

class ComponentLocator
{
public:
	virtual TransformComponent* GetTransformComponent(Id<GameObjectTag> id) = 0;
	virtual PhysicsComponent*   GetPhysicsComponent  (Id<GameObjectTag> id) = 0;
	virtual UpdateComponent *   GetUpdateComponent   (Id<GameObjectTag> id) = 0;
	virtual std::vector<GraphicsComponent*> GetGraphicsComponents(Id<GameObjectTag> id) = 0;
	virtual AnimatedGraphicsComponent* GetFirstAnimatedGraphicsComponent(Id<GameObjectTag> id) = 0;

	virtual Id<GameObjectTag> GetIdByTag (std::string tag) = 0;
	virtual std::vector<ni::Id<ni::GameObjectTag>> GetIdsByTag(std::string tag) = 0;

protected:
	std::unordered_map<std::string, std::vector<Id<GameObjectTag>>> id_tag_map_;
};

}