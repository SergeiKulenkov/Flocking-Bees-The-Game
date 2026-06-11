#include "BoidBase.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Scene/Component/Sprite.h>
#include <Scene/Component/Collider.h>
#include <Utility/Utility.h>

#include "BoidsManager.h"
#include "../Environment/Wall.h"

////////////////////

void BoidBase::Setup(const std::string_view& path, const glm::vec2& screenSize, const float screenOffset)
{
	const glm::vec2 position = glm::vec2(Random::RandomInRange(screenOffset, screenSize.x - screenOffset),
										Random::RandomInRange(screenOffset, screenSize.y - screenOffset));
	const glm::vec2 direction = glm::vec2(Random::RandomInRange(-1.f, 1.f), Random::RandomInRange(-1.f, 1.f));

	m_Transform = AddComponent<Transform>(position, direction)->GetTransformData();
	const std::shared_ptr<Sprite> sprite = AddComponent<Sprite>(path);
	m_Radius = sprite->GetSize().x;
	m_Velocity = m_Transform->rotation * m_MinSpeed;
}

void BoidBase::Update(float deltaTime)
{
	// if not in state obstacle avoidance
	// if yes, then apply steer away from it with some rotation rate
	Flock();
	if (m_FrameCounter == framesBetweenRaycast)
	{
		AvoidWalls();
		m_FrameCounter = 0;
	}
	else m_FrameCounter++;

	m_Velocity += m_SteeringForce * deltaTime;
	m_Speed = glm::length(m_Velocity);
	m_Transform->rotation = m_Velocity / m_Speed;

	m_Speed = glm::clamp(m_Speed, m_MinSpeed, m_MaxSpeed);
	m_Velocity = m_Transform->rotation * m_Speed;
	m_SteeringForce = glm::vec2(0, 0);
	m_Transform->position += m_Velocity * deltaTime;
}

void BoidBase::Flock()
{
	//m_Manager->GetNeighbours();
}

void BoidBase::AvoidWalls()
{
	std::shared_ptr<RaycastHit> hitResult = std::make_shared<RaycastHit>();
	if (m_Scene.lock()->Raycast(m_Transform->position + m_Transform->rotation * m_Radius, m_Transform->rotation, raycastLength, hitResult))
	{
		const std::shared_ptr<Entity> sharedEntity = hitResult->entity.lock();
		if (std::dynamic_pointer_cast<Wall>(sharedEntity) != nullptr)
		{
			// set state to obstalce avoidance
			UpdateVelocity(m_Transform->rotation * (-1.f) * m_ObstacleAvoidanceSpeed);
		}
	}
}