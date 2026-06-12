#pragma once
#include "BoidBase.h"

class BoidsManager;

////////////////////

enum class BeeState
{
	None,
	Flocking,
	ObstacleAvoidance,
};

////////////////////

class Bee : public BoidBase
{
public:
	Bee() : BoidBase(minSpeed, maxSpeed, raycastLength) {}

	// not const so they can be edited in the debug window
	static inline float perceptionRadius = 50.f;
	static inline float separationRadius = 35.f;
	static inline float predatorAvoidanceRadius = 100.f;
	static inline float perceptionRadiusMax = 100.f;
	static inline float separationRadiusMax = 75.f;
	static inline float predatorAvoidanceRadiusMax = 200.f;

protected:
	virtual void Setup(const glm::vec2& screenSize, const uint16_t id, BoidsManager* manager) override;
	virtual void Update(float deltaTime) override;
	virtual void DrawDebug(const RendererDebug& rendererDebug) override;

	void Flock();
	void AvoidPredators();
	virtual void ChangeObstacleAvoidanceState(const bool avoidWalls);

private:
	static constexpr std::string_view beeImagePath = "../Assets/bee.png";

	static constexpr float minSpeed = 47.5f;
	static constexpr float maxSpeed = 77.75f;
	static constexpr float rotationRate = 90.f;
	static constexpr float obstacleAvoidanceSpeed = 1.f;
	static constexpr float raycastLength = 80.f;

	static constexpr float alignmentWeight = 0.8f;
	static constexpr float cohesionWeight = 0.8f;
	static constexpr float separationWeight = 1.3f;
	static constexpr float predatorAvoidanceSpeed = 22.0f;

	static constexpr float screenOffset = 100.f;
	static constexpr float mass = 0.25f;
	static constexpr float linearDamping = 0.1f;
	static constexpr float restitution = 0.9f;

	glm::vec2 m_AlignmentForce = glm::vec2(0.f, 0.f);
	glm::vec2 m_CohesionForce = glm::vec2(0.f, 0.f);
	glm::vec2 m_SeparationForce = glm::vec2(0.f, 0.f);

	BeeState m_State = BeeState::None;

	friend class BoidsManager;
};

