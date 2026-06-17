#include "BoidsManager.h"

#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Utility/Utility.h>

#define ASSERT_BOID_SHARED_PTR(boid) assert(boid && "Can't get Boid's shared pointer because it's no longer valid.");
#define ASSERT_PREDATOR_SHARED_PTR(predator) assert(predator && "Can't get Predator's shared pointer because it's no longer valid.");

////////////////////

void BoidsManager::OnInit()
{
	ASSERT_SCENE_NULLPTR(m_Scene);
	const glm::vec2 screenSize = m_Scene->GetScreenSize();
	m_BoidsQuadTree = QuadTree<uint16_t>(glm::vec2(0.f, 0.f), screenSize);
	
	InitBoids(screenSize);
	InitPredators(screenSize);
	RegisterDebugFields();
}

void BoidsManager::InitBoids(const glm::vec2& screenSize)
{
	std::shared_ptr<Entity> newEntity;
	m_Bees.reserve(numberOfBoids);
	m_FlockingData.reserve(numberOfBoids);

	for (uint16_t id = 0; id < numberOfBoids; id++)
	{
		newEntity = m_Scene->CreateEntity<Bee>();
		const std::shared_ptr<Bee> newBoid = std::dynamic_pointer_cast<Bee>(newEntity);
		if (newBoid != nullptr)
		{
			newBoid->Setup(screenSize, id, this);
			m_FlockingData.push_back(BoidData());
			m_FlockingData[id].id = id;
			m_Bees.push_back(newBoid);
		}
	}
}

void BoidsManager::InitPredators(const glm::vec2& screenSize)
{
	std::shared_ptr<Entity> newEntity;
	m_Hornets.reserve(numberOfPredators);
	m_PredatorsData.reserve(numberOfPredators);

	for (uint16_t id = 0; id < numberOfPredators; id++)
	{
		newEntity = m_Scene->CreateEntity<Hornet>();
		const std::shared_ptr<Hornet> newPredator = std::dynamic_pointer_cast<Hornet>(newEntity);
		if (newPredator != nullptr)
		{
			newPredator->Setup(screenSize, id, this);
			m_PredatorsData.push_back(BoidData());
			m_PredatorsData[id].id = id;
			m_Hornets.push_back(newPredator);
		}
	}
}

void BoidsManager::RegisterDebugFields() const
{
	m_Scene->RegisterFieldInteger(numberOfBoidsText.data(), (int*)(&numberOfBoids));
	m_Scene->RegisterEditableDebugWindowField(perceptionRadiusText.data(), &Bee::perceptionRadius, Bee::perceptionRadiusMax);
	m_Scene->RegisterEditableDebugWindowField(separationRadiusText.data(), &Bee::separationRadius, Bee::separationRadiusMax);
	m_Scene->RegisterEditableDebugWindowField(predatorAvoidanceRadiusText.data(), &Bee::predatorAvoidanceRadius, Bee::predatorAvoidanceRadiusMax);
	m_Scene->RegisterCheckbox(drawDebugInfoField, &BoidBase::isDrawingDebug);
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

	m_BoidsQuadTree.Clear();
	for (uint16_t index = 0; index < m_FlockingData.size(); index++)
	{
		const std::shared_ptr<Bee> bee = m_Bees[index].lock();
		ASSERT_BOID_SHARED_PTR(bee);

		m_FlockingData[index].position = bee->GetPosition();
		m_FlockingData[index].velocity = bee->GetVelocity();
		m_BoidsQuadTree.Insert(m_FlockingData[index].id, Area(m_FlockingData[index].position, glm::vec2(bee->GetRadius())));
	}
}

std::vector<BoidData> BoidsManager::GetNeighboursData(const glm::vec2& position, const float radius, const uint16_t selfId) const
{
	std::vector<BoidData> result;
	std::vector<uint16_t> queryResult;

	m_BoidsQuadTree.Query(Area(position, glm::vec2(radius)), queryResult, selfId);
	result.reserve(queryResult.size());
	for (const uint16_t id : queryResult)
	{
		result.emplace_back(m_FlockingData[id]);
	}

	return result;
}