#pragma once
#include "BoidBase.h"

class BoidsManager;

enum class HornetState
{
	None,
	Wandering,
	ObstacleAvoidance,
};

////////////////////

class Hornet : public BoidBase
{
public:
	Hornet() : BoidBase(minSpeed, maxSpeed, raycastLength) {}

protected:
	virtual void Setup(const glm::vec2& screenSize, const uint16_t id, BoidsManager* manager) override;
	virtual void Update(float deltaTime) override;
	virtual void DrawDebug(const RendererDebug& rendererDebug) override;

	void AvoidPredators();
	virtual void ChangeObstacleAvoidanceState(const bool avoidWalls);

private:
	static constexpr std::string_view hornetImagePath = "../Assets/predator.png";
	static constexpr float minSpeed = 67.5f;
	static constexpr float maxSpeed = 112.25f;
	static constexpr float rotationRate = 60.f;
	static constexpr float obstacleAvoidanceSpeed = 1.f;
	static constexpr float raycastLength = 100.f;

	static constexpr float separationRadius = 125.f;
	static constexpr float separationWeight = 1.5f;

	static constexpr float screenOffset = 70.f;
	static constexpr float mass = 0.25f;
	static constexpr float linearDamping = 0.1f;
	static constexpr float restitution = 0.9f;

	HornetState m_State = HornetState::None;
	glm::vec2 m_SeparationForce = glm::vec2(0.f, 0.f);

	friend class BoidsManager;
};