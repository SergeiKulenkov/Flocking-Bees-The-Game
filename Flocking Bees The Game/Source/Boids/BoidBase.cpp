#include "BoidBase.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Scene/Component/Sprite.h>
#include <Utility/Utility.h>

#include "../Environment/Wall.h"

////////////////////

void BoidBase::Setup(const std::string_view& path, const glm::vec2& screenSize, const float screenOffset)
{
	const glm::vec2 position = glm::vec2(Random::RandomInRange(screenOffset, screenSize.x - screenOffset),
										Random::RandomInRange(screenOffset, screenSize.y - screenOffset));
	const glm::vec2 direction = glm::vec2(Random::RandomInRange(-1.f, 1.f), Random::RandomInRange(-1.f, 1.f));

	m_Transform = AddComponent<Transform>(position, glm::normalize(direction))->GetTransformData();
	const glm::vec2 size = AddComponent<Sprite>(path)->GetSize();
	m_Radius = glm::min(size.x, size.y) / 2.f;
	m_Velocity = m_Transform->rotation * m_MinSpeed;
}

void BoidBase::Update(float deltaTime)
{
	CheckBoundaries();
	//if (m_SteeringForce != glm::vec2(0.f, 0.f))
	//{
		m_Velocity += m_SteeringForce * deltaTime;
		m_Speed = glm::length(m_Velocity);
		m_Transform->rotation = m_Velocity / m_Speed;

		m_Speed = glm::clamp(m_Speed, m_MinSpeed, m_MaxSpeed);
		m_Velocity = m_Transform->rotation * m_Speed;
		m_SteeringForce = glm::vec2(0, 0);
		m_Transform->position += m_Velocity * deltaTime;
	//}
	//else
	//{
	//	// there's no outside force so just move in the current direction
	//	// maybe add a bit of randomness to speed and direction
	//	m_Speed = glm::clamp(m_Speed, m_MinSpeed, m_MaxSpeed);
	//	// don't multiply a vector twice
	//	m_Transform->position += m_Speed * deltaTime * m_Transform->rotation;
	//}
}

void BoidBase::CheckWalls()
{
	bool hitWall = false;
	RaycastHit hitResult;
	const glm::vec2 start = m_Transform->position + m_Transform->rotation * m_Radius;
	uint8_t rayId = 0;

	for (uint8_t i = 0; i < numebrOfRays; i++)
	{
		glm::vec2 direction = m_Transform->rotation;
		if (i != 0 && i % 2 == 0)
		{
			const float angle = raycastAngleStep * (float)((i + 1) / 2);
			direction = Vector::Rotate(direction, -angle);
		}
		else if (i % 2 != 0)
		{
			const float angle = raycastAngleStep * (float)((i + 1) / 2);
			direction = Vector::Rotate(direction, angle);
		}

		if (m_Scene->Raycast(start, direction, m_RaycastLength, hitResult))
		{
			const std::shared_ptr<Entity> hitEntity = hitResult.entity.lock();
			ASSERT_ENTITY_SHARED_PTR(hitEntity);

			hitWall = std::dynamic_pointer_cast<Wall>(hitEntity) != nullptr;
			if (hitWall)
			{
				rayId = i;
				break;
			}
		}
	}

	UpdateObstacleAvoidance(hitWall, hitResult.contactPoint, rayId);
}

void BoidBase::CheckBoundaries()
{
	glm::vec2 avoidanceDirection = glm::vec2(0, 0);

	if (m_Transform->position.x < m_EdgeMargin)
	{
		avoidanceDirection.x = 1;
	}
	else if (m_Transform->position.x > boundaries.x - m_EdgeMargin)
	{
		avoidanceDirection.x = -1;
	}

	if (m_Transform->position.y < m_EdgeMargin)
	{
		avoidanceDirection.y = 1;
	}
	else if (m_Transform->position.y > boundaries.y - m_EdgeMargin)
	{
		avoidanceDirection.y = -1;
	}

	UpdateVelocity(avoidanceDirection * m_BoundariesAvoidanceSpeed);
}