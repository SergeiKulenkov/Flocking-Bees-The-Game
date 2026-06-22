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
	virtual void UpdateObstacleAvoidance(const bool hitWall, const glm::vec2& rayContactPoint, const uint8_t rayId);

private:
	static constexpr std::string_view hornetImagePath = "../Assets/predator.png";
	static constexpr float minSpeed = 40.f;
	static constexpr float maxSpeed = 60.f;
	static constexpr float rotationRate = 1.5f;
	static constexpr float raycastLength = 150.f;
	static constexpr float screenOffset = 150.f;

	static constexpr float separationRadius = 150.f;
	static constexpr float separationWeight = 1.5f;

	// temporary while testing raycasting for obstacle avoidance
	static constexpr float boundaryAvoidanceSpeed = 3.f;
	static constexpr float edgeMargin = 50.f;

	HornetState m_State = HornetState::None;
	glm::vec2 m_SeparationForce = glm::vec2(0.f, 0.f);

	friend class BoidsManager;
};