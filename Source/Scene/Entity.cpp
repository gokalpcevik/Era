#include "Entity.h"

namespace Era
{
	Entity::Entity(Scene* pScene, entt::entity entity) : m_EntityHandle(entity) , m_Scene(pScene)
	{
		
	}
}