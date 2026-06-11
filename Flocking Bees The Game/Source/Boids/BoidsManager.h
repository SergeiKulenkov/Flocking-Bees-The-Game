#pragma once
#include <array>
#include <memory>
#include <string_view>

#include <Scene/Entity.h>
#include <Utility/DataStructures/QuadTree.h>

#include "Bee.h"
#include "Hornet.h"

////////////////////

struct BoidData
{
	uint16_t id = 0;
	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 velocity = glm::vec2(0, 0);
};

////////////////////

class BoidsManager : public Entity
{
public:
	BoidsManager() {}
	~BoidsManager() {}

	std::vector<BoidData> GetNeighbours(const glm::vec2& position, const float radius, const uint16_t selfId);

protected:
	virtual void OnInit() override;
	virtual void OnDestroy() override {}

	virtual void Update(float deltaTime) override;

private:
	//void CheckBoundaries(MovingObject& object);
	void Flock(const uint16_t index);
	void AvoidPredators(const uint16_t index);
	void SeparatePredators(const uint16_t index);

	////////////////////

	static constexpr uint16_t numberOfBoids = 100;
	static constexpr uint8_t numberOfPredators = 2;

	static constexpr std::string_view drawDebugInfoField = "Draw Debug Info";
	static constexpr std::string_view numberOfBoidsText = "Number of Bodis";

	static constexpr std::string_view perceptionRadiusText = "Perception Radius";
	static constexpr std::string_view separationRadiusText = "Separation Radius";
	static constexpr std::string_view predatorAvoidanceRadiusText = "Predator Avoidance";
	static constexpr std::string_view alignmentWeightText = "Alignment Weight";
	static constexpr std::string_view cohesionWeightText = "Cohesion Weight";
	static constexpr std::string_view separationWeightText = "Separation Weight";

	std::array<std::weak_ptr<Bee>, numberOfBoids> m_Bees;
	std::array<BoidData, numberOfBoids> m_FlockingData;

	// probably better to move predators to a separate PredatorManager
	std::array<std::weak_ptr<Hornet>, numberOfPredators> m_Hornets;
	std::array<BoidData, numberOfPredators> m_PredatorsData;

	QuadTree<uint16_t> m_QuadTree = QuadTree<uint16_t>(glm::vec2(0.f, 0.f), glm::vec2(1920.f, 1080.f));
};