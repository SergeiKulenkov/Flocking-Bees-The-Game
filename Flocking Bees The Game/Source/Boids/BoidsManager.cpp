#include "BoidsManager.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Utility/Utility.h>

#define ASSERT_BOID_SHARED_PTR(boid) assert(boid && "Can't get Boid's shared pointer because it's no longer valid.");

////////////////////

void BoidsManager::OnInit()
{
	const std::shared_ptr<Scene> sharedScene = m_Scene.lock();
	ASSERT_SCENE_SHARED_PTR(sharedScene);
	const glm::vec2 screenSize = sharedScene->GetScreenSize();
	std::shared_ptr<Entity> newEntity;

	// boids
	{
		for (uint16_t i = 0; i < m_Boids.size(); i++)
		{
			newEntity = sharedScene->CreateEntity<Bee>();
			const std::shared_ptr<Bee> newBoid = std::dynamic_pointer_cast<Bee>(newEntity);
			if (newBoid != nullptr)
			{
				newBoid->Setup(screenSize, i);
				m_FlockingData[i].id = i;
				m_Boids[i] = newBoid;
			}
		}
	}

	// predators
	{
		for (uint16_t i = 0; i < m_Predators.size(); i++)
		{
			newEntity = sharedScene->CreateEntity<Hornet>();
			const std::shared_ptr<Hornet> newPredator = std::dynamic_pointer_cast<Hornet>(newEntity);
			if (newPredator != nullptr)
			{
				newPredator->Setup(screenSize, i);
				m_PredatorsData[i].id = i;
				m_Predators[i] = newPredator;
			}
		}
	}

	sharedScene->RegisterDebugWindowField(numberOfBoidsText.data(), (float*)(&numberOfBoids));
	sharedScene->RegisterEditableDebugWindowField(perceptionRadiusText.data(), &Bee::perceptionRadius);
	sharedScene->RegisterEditableDebugWindowField(separationRadiusText.data(), &Bee::separationRadius);
	sharedScene->RegisterEditableDebugWindowField(predatorAvoidanceRadiusText.data(), &Bee::predatorAvoidanceRadius);
	sharedScene->RegisterCheckbox(drawDebugInfoField, &BoidBase::isDrawingDebug);
}

void BoidsManager::Update(float deltaTime)
{
	for (uint16_t i = 0; i < m_Predators.size(); i++)
	{
		SeparatePredators(i);
	}

	for (uint16_t i = 0; i < m_Boids.size(); i++)
	{
		Flock(i);
		AvoidPredators(i);
	}
}

void BoidsManager::Flock(const uint16_t index)
{
	const std::shared_ptr<Bee> current = m_Boids[index].lock();
	ASSERT_BOID_SHARED_PTR(current);
	const float perceptionRadiusSquared = (Bee::perceptionRadius + current->GetRadius()) * (Bee::perceptionRadius + current->GetRadius());
	const float separationRadiusSquared = (Bee::separationRadius + current->GetRadius()) * (Bee::separationRadius + current->GetRadius());
	const glm::vec2 currentPosition = current->GetPosition();
	const uint16_t currentId = current->GetId();

	uint16_t numberOfNeighbours = 0;
	glm::vec2 alignment = glm::vec2(0, 0);
	glm::vec2 cohesion = glm::vec2(0, 0);
	glm::vec2 separation = glm::vec2(0, 0);
	glm::vec2 positionDifference = glm::vec2(0, 0);

	for (const MovingObjectData& neighbour : m_FlockingData)
	{
		if (neighbour.id != currentId)
		{
			positionDifference = currentPosition - neighbour.position;
			float distance = glm::dot(positionDifference, positionDifference);

			if (distance < perceptionRadiusSquared)
			{
				alignment += neighbour.velocity;
				cohesion += neighbour.position;

				if (distance < separationRadiusSquared)
				{
					separation += positionDifference / distance;
				}
				numberOfNeighbours++;
			}
		}
	}

	if (numberOfNeighbours > 0)
	{
		const glm::vec2 currentVelocity = current->GetVelocity();
		alignment /= numberOfNeighbours;
		alignment = glm::normalize(alignment) * Bee::maxSpeed;
		alignment -= currentVelocity;
		current->UpdateAcceleration(alignment * Bee::allignmentWeight);

		cohesion /= numberOfNeighbours;
		cohesion -= currentPosition;
		cohesion = glm::normalize(cohesion) * Bee::maxSpeed;
		cohesion -= currentVelocity;
		current->UpdateAcceleration(cohesion * Bee::cohesionWeight);

		if (separation != glm::vec2(0, 0))
		{
			separation /= numberOfNeighbours;
			separation = glm::normalize(separation) * Bee::maxSpeed;
			separation -= currentVelocity;
			current->UpdateAcceleration(separation * Bee::separationWeight);
		}
	}
}

void BoidsManager::AvoidPredators(const uint16_t index)
{
	const std::shared_ptr<Bee> current = m_Boids[index].lock();
	ASSERT_BOID_SHARED_PTR(current);
	const float avoidanceRadiusSquared = (Bee::predatorAvoidanceRadius + current->GetRadius()) * (Bee::predatorAvoidanceRadius + current->GetRadius());
	const glm::vec2 currentPosition = current->GetPosition();
	uint16_t numberOfPredators = 0;
	glm::vec2 positionDifference = glm::vec2(0, 0);
	glm::vec2 separation = glm::vec2(0, 0);

	//for (const Predator& predator : m_Predators)
	//{
	//	positionDifference = currentPosition - predator.GetPosition();
	//	float distance = glm::dot(positionDifference, positionDifference);

	//	if (distance < avoidanceRadiusSquared)
	//	{
	//		separation += positionDifference / distance;
	//		numberOfPredators++;
	//	}
	//}

	if (numberOfPredators > 0)
	{
		separation *= numberOfPredators;
		separation = glm::normalize(separation);
		current->UpdateVelocity(separation * Bee::predatorAvoidanceSpeed);
	}
}

void BoidsManager::SeparatePredators(const uint16_t index)
{
	const std::shared_ptr<Bee> current = m_Boids[index].lock();
	// TODO: assert valid shared ptr
	const float separationRadiusSquared = (Hornet::separationRadius + current->GetRadius()) * (Hornet::separationRadius + current->GetRadius());
	const glm::vec2 currentPosition = current->GetPosition();
	const uint16_t currentId = current->GetId();

	uint8_t numberOfNeighbours = 0;
	glm::vec2 positionDifference = glm::vec2(0, 0);
	glm::vec2 separation = glm::vec2(0, 0);

	for (const MovingObjectData& neighbour : m_PredatorsData)
	{
		if (currentId != neighbour.id)
		{
			positionDifference = currentPosition - neighbour.position;
			float distance = glm::dot(positionDifference, positionDifference);

			if (distance < separationRadiusSquared)
			{
				separation += positionDifference / distance;
				numberOfNeighbours++;
			}
		}
	}

	if (numberOfNeighbours)
	{
		separation *= numberOfNeighbours;
		separation = glm::normalize(separation) * Hornet::maxSpeed;
		separation -= current->GetVelocity();
		current->UpdateAcceleration(separation * Hornet::separationWeight);
	}
}
