#include "Hornet.h"

#include <Scene/Scene.h>
#include <Scene/Component/Transform.h>

#include "BoidsManager.h"

////////////////////

void Hornet::Setup(const glm::vec2& screenSize, const uint16_t id, BoidsManager* manager)
{
	m_Id = id;
	m_Manager = manager;
	BoidBase::Setup(hornetImagePath, screenSize, screenOffset);
	m_State = HornetState::Wandering;
}

void Hornet::Update(float deltaTime)
{
	if (m_State == HornetState::ObstacleAvoidance ||
		m_FrameCounter == framesBetweenRaycast)
	{
		CheckWalls();
		m_FrameCounter = 0;
	}
	else m_FrameCounter++;

	if (m_State == HornetState::Wandering)
	{
		AvoidPredators();
	}
	else if (m_State == HornetState::ObstacleAvoidance)
	{
		const float direction = rotateClockwise ? -1.f : 1.f;
		m_Transform->rotation = Vector::Rotate(m_Transform->rotation, rotationRate * deltaTime * direction);
	}

	BoidBase::Update(deltaTime);
}

void Hornet::AvoidPredators()
{
	m_SeparationForce = glm::vec2(0.f, 0.f);
	const float separationRadiusSquared = separationRadius * separationRadius;
	uint8_t numberOfNeighbours = 0;
	glm::vec2 positionDifference = glm::vec2(0.f, 0.f);

	for (const BoidData& neighbour : m_Manager->GetPredatorsData())
	{
		if (m_Id != neighbour.id)
		{
			positionDifference = m_Transform->position - neighbour.position;
			float distance = glm::dot(positionDifference, positionDifference);

			if (distance < separationRadiusSquared)
			{
				m_SeparationForce += positionDifference / distance;
				numberOfNeighbours++;
			}
		}
	}

	if (numberOfNeighbours)
	{
		m_SeparationForce *= numberOfNeighbours;
		m_SeparationForce = glm::normalize(m_SeparationForce) * maxSpeed;
		m_SeparationForce -= m_Velocity;
		UpdateSteeringForce(m_SeparationForce * separationWeight);
	}
}

void Hornet::UpdateObstacleAvoidance(const bool hitWall, const glm::vec2& rayContactPoint, const uint8_t rayId)
{
	if (hitWall)
	{
		if (m_State != HornetState::ObstacleAvoidance)
		{
			m_State = HornetState::ObstacleAvoidance;
			if (m_Transform->rotation.y > 0.f)
			{
				// looking down
				rotateClockwise = !(rayId != 0 && rayId % 2 == 0);
			}
			else rotateClockwise = (rayId % 2 != 0);
		}
	}
	else
	{
		if (m_State == HornetState::ObstacleAvoidance)
		{
			// TODO: add a timer or a frame counter to continue rotation for a bit or acelerate away
		}
		m_State = HornetState::Wandering;
	}
}

void Hornet::DrawDebug(const RendererDebug& rendererDebug)
{
	if (isDrawingDebug)
	{
		rendererDebug.DrawCircle(m_Transform->position, separationRadius, Colour::green);
	}

	const glm::vec2 start = m_Transform->position + m_Transform->rotation * m_Radius;
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

		rendererDebug.DrawLine(start, start + direction * raycastLength, Colour::red);
	}
}