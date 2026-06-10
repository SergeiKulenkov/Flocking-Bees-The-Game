#include "BoidBase.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Scene/Component/Sprite.h>
#include <Scene/Component/Collider.h>
#include <Utility/Utility.h>

////////////////////

void BoidBase::Setup(const std::string_view& path, const glm::vec2& screenSize, const float screenOffset)
{
	const glm::vec2 position = glm::vec2(Random::RandomInRange(screenOffset, screenSize.x - screenOffset),
										Random::RandomInRange(screenOffset, screenSize.y - screenOffset));
	const glm::vec2 direction = glm::vec2(Random::RandomInRange(-1.f, 1.f), Random::RandomInRange(-1.f, 1.f));

	m_Transform = AddComponent<Transform>(position, direction)->GetTransformData();
	const std::shared_ptr<Sprite> sprite = AddComponent<Sprite>(path);
	m_Radius = sprite->GetSize().x;
	AddComponent<CircleCollider>(m_Radius);
	m_Velocity = m_Transform->rotation * m_MinSpeed;
}

void BoidBase::Update(float deltaTime)
{
	// avoid walls

	m_Velocity += m_Acceleration * deltaTime;
	m_Speed = glm::length(m_Velocity);
	m_Transform->rotation = m_Velocity / m_Speed;

	m_Speed = glm::clamp(m_Speed, m_MinSpeed, m_MaxSpeed);
	m_Velocity = m_Transform->rotation * m_Speed;
	m_Acceleration = glm::vec2(0, 0);
	m_Transform->position += m_Velocity * deltaTime;
}