#include "BoidBase.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Scene/Component/Sprite.h>
#include <Scene/Component/Collider.h>
#include <Utility/Utility.h>

#include "../Environment/Wall.h"

////////////////////

void BoidBase::Setup(const std::string_view& path, const glm::vec2& screenSize, const float screenOffset)
{
	const glm::vec2 position = glm::vec2(Random::RandomInRange(screenOffset, screenSize.x - screenOffset),
										Random::RandomInRange(screenOffset, screenSize.y - screenOffset));
	const glm::vec2 direction = glm::vec2(Random::RandomInRange(-1.f, 1.f), Random::RandomInRange(-1.f, 1.f));

	m_Transform = AddComponent<Transform>(position, direction)->GetTransformData();
	const glm::vec2 size = AddComponent<Sprite>(path)->GetSize();
	m_Radius = glm::min(size.x, size.y);
	m_Velocity = m_Transform->rotation * m_MinSpeed;
}

void BoidBase::Update(float deltaTime)
{
	m_Velocity += m_SteeringForce * deltaTime;
	m_Speed = glm::length(m_Velocity);
	m_Transform->rotation = m_Velocity / m_Speed;

	m_Speed = glm::clamp(m_Speed, m_MinSpeed, m_MaxSpeed);
	m_Velocity = m_Transform->rotation * m_Speed;
	m_SteeringForce = glm::vec2(0, 0);
	m_Transform->position += m_Velocity * deltaTime;
}

void BoidBase::CheckWalls()
{
	bool hitWall = false;
	RaycastHit hitResult;
	float angleStep = 0.15f;

	for (int i = 0; i < 5; i++)
	{
		glm::vec2 direction = m_Transform->rotation;
		if (i != 0 && i % 2 == 0)
		{
			float angle = angleStep * (float)((i + 1) / 2);
			direction = Vector::Rotate(direction, -angle);
		}
		else if (i % 2 != 0)
		{
			float angle = angleStep * (float)((i + 1) / 2);
			direction = Vector::Rotate(direction, angle);
		}

		hitWall = m_Scene->Raycast(m_Transform->position + m_Transform->rotation * m_Radius, direction, m_RaycastLength, hitResult);
		if (hitWall)
		{
			const std::shared_ptr<Entity> sharedEntity = hitResult.entity.lock();
			ASSERT_ENTITY_SHARED_PTR(sharedEntity);
			hitWall = std::dynamic_pointer_cast<Wall>(sharedEntity) != nullptr;
			break;
		}
	}

	ChangeObstacleAvoidanceState(hitWall);
}