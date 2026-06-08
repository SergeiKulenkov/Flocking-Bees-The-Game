#include "MovingObject.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Scene/Component/Transform.h>
#include <Scene/Component/Sprite.h>
#include <Scene/Component/Collider.h>
#include <Scene/Component/Rigidbody.h>
#include <Utility/Utility.h>

////////////////////

MovingObject::MovingObject(const std::string_view& path, const float minSpeed, const float maxSpeed, const float obstacleAvoidanceSpeed)
	: m_SpritePath(path), m_MinSpeed(minSpeed), m_MaxSpeed(maxSpeed), m_ObstacleAvoidanceSpeed(obstacleAvoidanceSpeed)
{}

void MovingObject::Setup(const glm::vec2& screenSize, const uint16_t id)
{
	const glm::vec2 position = glm::vec2(Random::RandomInRange(screenOffset, screenSize.x - screenOffset),
										Random::RandomInRange(screenOffset, screenSize.y - screenOffset));
	const glm::vec2 direction = glm::vec2(Random::RandomInRange(-1.f, 1.f), Random::RandomInRange(-1.f, 1.f));

	m_Transform = AddComponent<Transform>(position, direction)->GetTransformData();
	const std::shared_ptr<Sprite> sprite = AddComponent<Sprite>(m_SpritePath);
	m_Radius = sprite->GetSize().x;
	AddComponent<CircleCollider>(m_Radius);
	m_Rigidbody = AddComponent<Rigidbody>(mass, linearDamping, restitution);

	m_CenterPosition = position;
	m_Direction = direction;
	m_Velocity = m_Direction * m_MinSpeed;
	m_Id = id;
}

void MovingObject::Update(float deltaTime)
{
	// just use apply force??
	// avoid walls

	m_Velocity += m_Acceleration * deltaTime;
	float speed = glm::length(m_Velocity);
	m_Direction = m_Velocity / speed;
	speed = glm::clamp(speed, m_MinSpeed, m_MaxSpeed);
	m_Velocity = m_Direction * speed;
	m_Acceleration = glm::vec2(0, 0);

	m_CenterPosition += m_Velocity * deltaTime;
}