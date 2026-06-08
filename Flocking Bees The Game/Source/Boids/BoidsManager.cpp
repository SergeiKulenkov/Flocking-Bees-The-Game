#include "BoidsManager.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Utility/Utility.h>

////////////////////

void BoidsManager::OnInit()
{
	const std::shared_ptr<Scene> sharedScene = m_Scene.lock();
	ASSERT_SCENE_SHARED_PTR(sharedScene);
	const glm::vec2 screenSize = sharedScene->GetScreenSize();
	std::shared_ptr<Entity> newEntity;
	uint16_t id = 0;

	// boids
	{
		for (uint16_t i = 0; i < m_Boids.size(); i++)
		{
			newEntity = sharedScene->CreateEntity<Boid>();
			const std::shared_ptr<Boid> newBoid = std::dynamic_pointer_cast<Boid>(newEntity);
			if (newBoid != nullptr)
			{
				newBoid->Setup(screenSize, id);
				m_FlockingData[id].id = id;
				m_Boids[i] = newBoid;
				id++;
			}
		}
	}

	// predators
	{
		id = 0;
		for (uint16_t i = 0; i < m_Predators.size(); i++)
		{
			newEntity = sharedScene->CreateEntity<Predator>();
			const std::shared_ptr<Predator> newPredator = std::dynamic_pointer_cast<Predator>(newEntity);
			if (newPredator != nullptr)
			{
				newPredator->Setup(screenSize, id);
				m_PredatorsData[id].id = id;
				m_Predators[i] = newPredator;
				id++;
			}
		}
	}

	//sharedScene->RegisterDebugWindowField(numberOfBoidsText.data(), &numberOfBoids);
	//sharedScene->RegisterDebugWindowField(numberOfBoidsText.data(), &numberOfBoids);
	sharedScene->RegisterEditableDebugWindowField(perceptionRadiusText.data(), &Boid::perceptionRadius);
	sharedScene->RegisterEditableDebugWindowField(separationRadiusText.data(), &Boid::separationRadius);
	sharedScene->RegisterEditableDebugWindowField(predatorAvoidanceRadiusText.data(), &Boid::predatorAvoidanceRadius);
	//ImGui::Checkbox(drawDebugInfoText.data(), &m_DrawDebugInfo);
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
	}
}

void BoidsManager::Flock(const uint16_t index)
{
	const std::shared_ptr<Boid> current = m_Boids[index].lock();
	// TODO: assert valid shared ptr
	const float perceptionRadiusSquared = (Boid::perceptionRadius + current->GetRadius()) * (Boid::perceptionRadius + current->GetRadius());
	const float separationRadiusSquared = (Boid::separationRadius + current->GetRadius()) * (Boid::separationRadius + current->GetRadius());
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
		alignment = glm::normalize(alignment) * Boid::maxSpeed;
		alignment -= currentVelocity;
		current->UpdateAcceleration(alignment * Boid::allignmentWeight);

		cohesion /= numberOfNeighbours;
		cohesion -= currentPosition;
		cohesion = glm::normalize(cohesion) * Boid::maxSpeed;
		cohesion -= currentVelocity;
		current->UpdateAcceleration(cohesion * Boid::cohesionWeight);

		if (separation != glm::vec2(0, 0))
		{
			separation /= numberOfNeighbours;
			separation = glm::normalize(separation) * Boid::maxSpeed;
			separation -= currentVelocity;
			current->UpdateAcceleration(separation * Boid::separationWeight);
		}
	}
}

void BoidsManager::AvoidPredators(const uint16_t index)
{
	const std::shared_ptr<Boid> current = m_Boids[index].lock();
	// TODO: assert valid shared ptr
	const float avoidanceRadiusSquared = (Boid::predatorAvoidanceRadius + current->GetRadius()) * (Boid::predatorAvoidanceRadius + current->GetRadius());
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
		current->UpdateVelocity(separation * Boid::predatorAvoidanceSpeed);
	}
}

void BoidsManager::SeparatePredators(const uint16_t index)
{
	const std::shared_ptr<Boid> current = m_Boids[index].lock();
	// TODO: assert valid shared ptr
	const float separationRadiusSquared = (Predator::separationRadius + current->GetRadius()) * (Predator::separationRadius + current->GetRadius());
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
		separation = glm::normalize(separation) * Predator::maxSpeed;
		separation -= current->GetVelocity();
		current->UpdateAcceleration(separation * Predator::separationWeight);
	}
}
