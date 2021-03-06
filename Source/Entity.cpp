#include "Entity.h"

namespace se {

	void Entity::SetPosition(float x, float y, float z) {
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

	void Entity::SetPosition(Vector3f position) {
		m_position = position;
	}

	void Entity::SetScale(float x, float y, float z) {
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}

	void Entity::SetScale(Vector3f scale) {
		m_scale = scale;
	}

	void Entity::SetRotation(float x, float y, float z) {
		m_rotation.x = x;
		m_rotation.y = y;
		m_rotation.z = z;
	}

	void Entity::SetRotation(Vector3f rotation) {
		m_rotation = rotation;
	}

	void Entity::SetAssetName(const std::string &assetName) {
		m_assetName = assetName;
	}

	void Entity::SetEntityType(EntityType type) {
		m_entityType = type;
	}

	std::string Entity::GetAssetName() const {
		return m_assetName;
	}

	EntityType Entity::GetEntityType() const {
		return m_entityType;
	}

	Vector3f Entity::GetPosition() const {
		return m_position;
	}

	Vector3f Entity::GetScale() const {
		return m_scale;
	}

	Vector3f Entity::GetRotation() const {
		return m_rotation;
	}
}