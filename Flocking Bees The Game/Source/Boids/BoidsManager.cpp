#include "BoidsManager.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Utility/Utility.h>

#define ASSERT_BOID_SHARED_PTR(boid) assert(boid && "Can't get Boid's shared pointer because it's no longer valid.");
#define ASSERT_PREDATOR_SHARED_PTR(predator) assert(predator && "Can't get Predator's shared pointer because it's no longer valid.");

////////////////////

void BoidsManager::OnInit()
{
	const std::shared_ptr<Scene> sharedScene = m_Scene.lock();
	ASSERT_SCENE_SHARED_PTR(sharedScene);
	const glm::vec2 screenSize = sharedScene->GetScreenSize();
	std::shared_ptr<Entity> newEntity;

	// boids
	{
		for (uint16_t i = 0; i < m_Bees.size(); i++)
		{
			newEntity = sharedScene->CreateEntity<Bee>();
			const std::shared_ptr<Bee> newBoid = std::dynamic_pointer_cast<Bee>(newEntity);
			if (newBoid != nullptr)
			{
				newBoid->Setup(screenSize, i);
				m_FlockingData[i].id = i;
				m_Bees[i] = newBoid;
			}
		}
	}

	// predators
	{
		for (uint16_t i = 0; i < m_Hornets.size(); i++)
		{
			newEntity = sharedScene->CreateEntity<Hornet>();
			const std::shared_ptr<Hornet> newPredator = std::dynamic_pointer_cast<Hornet>(newEntity);
			if (newPredator != nullptr)
			{
				newPredator->Setup(screenSize, i);
				m_PredatorsData[i].id = i;
				m_Hornets[i] = newPredator;
			}
		}
	}

	// TODO: use integer function
	//sharedScene->RegisterDebugWindowField(numberOfBoidsText.data(), &numberOfBoids);
	sharedScene->RegisterEditableDebugWindowField(perceptionRadiusText.data(), &Bee::perceptionRadius);
	sharedScene->RegisterEditableDebugWindowField(separationRadiusText.data(), &Bee::separationRadius);
	sharedScene->RegisterEditableDebugWindowField(predatorAvoidanceRadiusText.data(), &Bee::predatorAvoidanceRadius);
	sharedScene->RegisterCheckbox(drawDebugInfoField, &BoidBase::isDrawingDebug);
}

void BoidsManager::Update(float deltaTime)
{
	for (uint16_t index = 0; index < m_PredatorsData.size(); index++)
	{
		const std::shared_ptr<Hornet> hornet = m_Hornets[index].lock();
		ASSERT_PREDATOR_SHARED_PTR(hornet);
		m_PredatorsData[index].position = hornet->GetPosition();
		m_PredatorsData[index].velocity = hornet->GetVelocity();
	}

	for (uint16_t i = 0; i < m_Hornets.size(); i++)
	{
		SeparatePredators(i);
	}

	for (uint16_t index = 0; index < m_FlockingData.size(); index++)
	{
		const std::shared_ptr<Bee> bee = m_Bees[index].lock();
		ASSERT_BOID_SHARED_PTR(bee);
		m_FlockingData[index].position = bee->GetPosition();
		m_FlockingData[index].velocity = bee->GetVelocity();
	}

	for (uint16_t i = 0; i < m_Bees.size(); i++)
	{
		Flock(i);
		AvoidPredators(i);
	}
}

void BoidsManager::Flock(const uint16_t index)
{
	const std::shared_ptr<Bee> current = m_Bees[index].lock();
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
		current->UpdateAcceleration(alignment * Bee::alignmentWeight);

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
	const std::shared_ptr<Bee> current = m_Bees[index].lock();
	ASSERT_BOID_SHARED_PTR(current);
	const float avoidanceRadiusSquared = (Bee::predatorAvoidanceRadius + current->GetRadius()) * (Bee::predatorAvoidanceRadius + current->GetRadius());
	const glm::vec2 currentPosition = current->GetPosition();
	uint16_t numberOfPredators = 0;
	glm::vec2 positionDifference = glm::vec2(0, 0);
	glm::vec2 separation = glm::vec2(0, 0);

	for (const std::weak_ptr<Hornet>& hornet : m_Hornets)
	{
		const std::shared_ptr<Hornet> sharedHornet = hornet.lock();
		ASSERT_PREDATOR_SHARED_PTR(sharedHornet);
		positionDifference = currentPosition - sharedHornet->GetPosition();

		float distance = glm::dot(positionDifference, positionDifference);
		if (distance < avoidanceRadiusSquared)
		{
			separation += positionDifference / distance;
			numberOfPredators++;
		}
	}

	if (numberOfPredators > 0)
	{
		separation *= numberOfPredators;
		separation = glm::normalize(separation);
		current->UpdateVelocity(separation * Bee::predatorAvoidanceSpeed);
	}
}

void BoidsManager::SeparatePredators(const uint16_t index)
{
	const std::shared_ptr<Hornet> current = m_Hornets[index].lock();
	ASSERT_PREDATOR_SHARED_PTR(current);
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
