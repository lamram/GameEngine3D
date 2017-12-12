#ifndef SCENE_H
#define SCENE_H

#include "std.h"
#include "Entity.h"
#include "DirectX9\DirectXTerrain.h"

namespace se {

	class Scene {
	public:
		void AddEntity(Entity *entity);
		void RemoveEntity(Entity *entity);
		void Update(float delta);
		void Render();
		void Release();
		const std::vector<Entity*> &GetEntities() const;
	private:
		std::vector<Entity*> m_entities;
		Terrain m_terrain;
	};

}

#endif