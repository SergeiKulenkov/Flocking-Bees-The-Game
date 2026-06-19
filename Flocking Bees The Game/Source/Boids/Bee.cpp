#include "Bee.h"

#include <Scene/Scene.h>
#include <Scene/Component/Transform.h>

#include "BoidsManager.h"

////////////////////

void Bee::Setup(const glm::vec2& screenSize, const uint16_t id, BoidsManager* manager)
{
	m_Id = id;
	m_Manager = manager;
	BoidBase::Setup(beeImagePath, screenSize, screenOffset);
	m_State = BeeState::Flocking;
}

void Bee::Update(float deltaTime)
{
	// TDOD: use a FSM
	if (m_State == BeeState::ObstacleAvoidance ||
		m_FrameCounter == framesBetweenRaycast)
	{
		CheckWalls();
		m_FrameCounter = 0;
	}
	else m_FrameCounter++;

	if (m_State == BeeState::Flocking)
	{
		Flock();
		AvoidPredators();
	}
	else if (m_State == BeeState::ObstacleAvoidance)
	{
		const float direction = rotateClockwise ? -1.f : 1.f;
		m_Transform->rotation = Vector::Rotate(m_Transform->rotation, rotationRate * deltaTime * direction);
	}

	BoidBase::Update(deltaTime);
}

void Bee::Flock()
{
	m_AlignmentForce = glm::vec2(0.f, 0.f);
	m_CohesionForce = glm::vec2(0.f, 0.f);
	m_SeparationForce = glm::vec2(0.f, 0.f);
	const float perceptionRadiusSquared = perceptionRadius * perceptionRadius;
	const float separationRadiusSquared = separationRadius * separationRadius;

	uint16_t numberOfNeighbours = 0;
	glm::vec2 positionDifference = glm::vec2(0.f, 0.f);

	// TODO: exclude objects that are behind
	// preception radius is used because it should be bigger than separation radius
	for (const BoidData& neighbour : m_Manager->GetNeighboursData(m_Transform->position, perceptionRadius, m_Id))
	{
		if (neighbour.id != m_Id)
		{
			positionDifference = m_Transform->position - neighbour.position;
			float distance = glm::dot(positionDifference, positionDifference);

			if (distance < perceptionRadiusSquared)
			{
				m_AlignmentForce += neighbour.velocity;
				m_CohesionForce += neighbour.position;

				if (distance < separationRadiusSquared)
				{
					m_SeparationForce += positionDifference / distance;
				}
				numberOfNeighbours++;
			}
		}
	}

	if (numberOfNeighbours > 0)
	{
		m_AlignmentForce /= numberOfNeighbours;
		m_AlignmentForce = glm::normalize(m_AlignmentForce) * maxSpeed;
		m_AlignmentForce -= m_Velocity;
		UpdateSteeringForce(m_AlignmentForce * alignmentWeight);

		m_CohesionForce /= numberOfNeighbours;
		m_CohesionForce -= m_Transform->position;
		m_CohesionForce = glm::normalize(m_CohesionForce) * maxSpeed;
		m_CohesionForce -= m_Velocity;
		UpdateSteeringForce(m_CohesionForce * cohesionWeight);

		if (m_SeparationForce != glm::vec2(0.f, 0.f))
		{
			m_SeparationForce /= numberOfNeighbours;
			m_SeparationForce = glm::normalize(m_SeparationForce) * maxSpeed;
			m_SeparationForce -= m_Velocity;
			UpdateSteeringForce(m_SeparationForce * separationWeight);
		}
	}
}

void Bee::AvoidPredators()
{
	m_SeparationForce = glm::vec2(0.f, 0.f);
	const float avoidanceRadiusSquared = predatorAvoidanceRadius * predatorAvoidanceRadius;
	uint16_t numberOfPredators = 0;
	glm::vec2 positionDifference = glm::vec2(0.f, 0.f);

	// TODO: exclude objects that are behind
	for (const BoidData& predator : m_Manager->GetPredatorsData())
	{
		positionDifference = m_Transform->position - predator.position;
		float distance = glm::dot(positionDifference, positionDifference);

		if (distance < avoidanceRadiusSquared)
		{
			m_SeparationForce += positionDifference / distance;
			numberOfPredators++;
		}
	}

	if (numberOfPredators > 0)
	{
		m_SeparationForce *= numberOfPredators;
		m_SeparationForce = glm::normalize(m_SeparationForce) * maxSpeed;
		//m_SeparationForce -= m_Velocity;
		UpdateSteeringForce(m_SeparationForce * predatorAvoidanceWeight);
	}
}

void Bee::UpdateObstacleAvoidance(const bool hitWall, const glm::vec2& rayContactPoint, const uint8_t rayId)
{
	if (hitWall)
	{
		if (m_State != BeeState::ObstacleAvoidance)
		{
			m_State = BeeState::ObstacleAvoidance;
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
		if (m_State == BeeState::ObstacleAvoidance)
		{
			// TODO: add a timer or a frame counter to continue rotation for a bit or acelerate away
		}
		m_State = BeeState::Flocking;
	}
}

void Bee::DrawDebug(const RendererDebug& rendererDebug)
{
	if (isDrawingDebug)
	{
		rendererDebug.DrawCircle(m_Transform->position, perceptionRadius, Colour::green);
		rendererDebug.DrawCircle(m_Transform->position, separationRadius, Colour::yellow);
		rendererDebug.DrawCircle(m_Transform->position, predatorAvoidanceRadius, Colour::red);
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