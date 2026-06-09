#pragma once
#include <array>
#include <memory>
#include <string_view>

#include <Scene/Entity.h>

#include "Bee.h"
#include "Hornet.h"

////////////////////

struct MovingObjectData
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

	void OnInit();
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

	std::array<std::weak_ptr<Bee>, numberOfBoids> m_Boids;
	std::array<MovingObjectData, numberOfBoids> m_FlockingData;

	// probably better to move predators to a separate PredatorManager
	std::array<std::weak_ptr<Hornet>, numberOfPredators> m_Predators;
	std::array<MovingObjectData, numberOfPredators> m_PredatorsData;
};