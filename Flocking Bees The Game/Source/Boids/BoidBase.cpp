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
}

void BoidBase::Update(float deltaTime)
{
	// use apply force or update velocity?
	// update rotation
	// avoid walls

	//m_Velocity += m_Acceleration * deltaTime;
	//float speed = glm::length(m_Velocity);
	//m_Direction = m_Velocity / speed;
	//speed = glm::clamp(speed, m_MinSpeed, m_MaxSpeed);
	//m_Velocity = m_Direction * speed;
	//m_Acceleration = glm::vec2(0, 0);

	//m_Transform->rotation = Vector::Rotate(m_Transform->rotation, rotationRate * deltaTime * input.x);
	//m_Speed += acceleration;
	//m_Speed = glm::clamp(m_Speed, 0.f, m_MaxSpeed);
	//m_Rigidbody->ApplyForce(Force(m_Transform->rotation * m_Speed, false));
}