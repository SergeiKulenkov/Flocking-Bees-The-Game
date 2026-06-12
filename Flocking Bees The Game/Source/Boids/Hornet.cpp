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
		const glm::vec2 rotate = Vector::Rotate(m_Transform->rotation, rotationRate * deltaTime * (-1.f));
		UpdateVelocity(m_Transform->rotation * (-1.f) * obstacleAvoidanceSpeed);
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

void Hornet::ChangeObstacleAvoidanceState(const bool avoidWalls)
{
	if (avoidWalls) m_State = HornetState::ObstacleAvoidance;
	else m_State = HornetState::Wandering;
}

void Hornet::DrawDebug(const RendererDebug& rendererDebug)
{
	if (isDrawingDebug)
	{
		rendererDebug.DrawCircle(m_Transform->position, separationRadius, Colour::green);
	}
}