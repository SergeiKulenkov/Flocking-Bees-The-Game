#include "BoidsManager.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Utility/Utility.h>

#define ASSERT_BOID_SHARED_PTR(boid) assert(boid && "Can't get Boid's shared pointer because it's no longer valid.");
#define ASSERT_PREDATOR_SHARED_PTR(predator) assert(predator && "Can't get Predator's shared pointer because it's no longer valid.");

////////////////////

std::vector<BoidData> BoidsManager::GetNeighboursData(const glm::vec2& position, const float radius, const uint16_t selfId) const
{
	std::vector<BoidData> result;

	return result;
}

void BoidsManager::OnInit()
{
	const std::shared_ptr<Scene> sharedScene = m_Scene.lock();
	ASSERT_SCENE_SHARED_PTR(sharedScene);
	const glm::vec2 screenSize = sharedScene->GetScreenSize();
	m_QuadTree = QuadTree<uint16_t>(glm::vec2(0.f, 0.f), screenSize);

	std::shared_ptr<Entity> newEntity;
	// boids
	{
		m_Bees.reserve(numberOfBoids);
		m_FlockingData.reserve(numberOfBoids);
		for (uint16_t i = 0; i < numberOfBoids; i++)
		{
			newEntity = sharedScene->CreateEntity<Bee>();
			const std::shared_ptr<Bee> newBoid = std::dynamic_pointer_cast<Bee>(newEntity);
			if (newBoid != nullptr)
			{
				newBoid->Setup(screenSize, i, this);
				m_FlockingData.push_back(BoidData());
				m_FlockingData[i].id = i;
				m_Bees.push_back(newBoid);
			}
		}
	}

	// predators
	{
		m_Hornets.reserve(numberOfPredators);
		m_PredatorsData.reserve(numberOfPredators);
		for (uint16_t i = 0; i < numberOfPredators; i++)
		{
			newEntity = sharedScene->CreateEntity<Hornet>();
			const std::shared_ptr<Hornet> newPredator = std::dynamic_pointer_cast<Hornet>(newEntity);
			if (newPredator != nullptr)
			{
				newPredator->Setup(screenSize, i, this);
				m_PredatorsData.push_back(BoidData());
				m_PredatorsData[i].id = i;
				m_Hornets.push_back(newPredator);
			}
		}
	}

	// TODO: use integer function
	//sharedScene->RegisterDebugWindowField(numberOfBoidsText.data(), &numberOfBoids);
	sharedScene->RegisterEditableDebugWindowField(perceptionRadiusText.data(), &Bee::perceptionRadius, Bee::perceptionRadiusMax);
	sharedScene->RegisterEditableDebugWindowField(separationRadiusText.data(), &Bee::separationRadius, Bee::separationRadiusMax);
	sharedScene->RegisterEditableDebugWindowField(predatorAvoidanceRadiusText.data(), &Bee::predatorAvoidanceRadius, Bee::predatorAvoidanceRadiusMax);
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

	for (uint16_t index = 0; index < m_FlockingData.size(); index++)
	{
		const std::shared_ptr<Bee> bee = m_Bees[index].lock();
		ASSERT_BOID_SHARED_PTR(bee);
		m_FlockingData[index].position = bee->GetPosition();
		m_FlockingData[index].velocity = bee->GetVelocity();
	}
}