#pragma once
#include "BoidBase.h"

////////////////////

class Bee : public BoidBase
{
public:
	Bee() : BoidBase(minSpeed, maxSpeed, obstacleAvoidanceSpeed) {}

	virtual void Setup(const glm::vec2& screenSize, const uint16_t id) override;
	virtual void Update(float deltaTime) override;
	virtual void DrawDebug(const RendererDebug& rendererDebug) override;

	// not const so they can be edited in the debug window
	static inline float perceptionRadius = 50.f;
	static inline float separationRadius = 35.f;
	static constexpr float alignmentWeight = 0.8f;
	static constexpr float cohesionWeight = 0.8f;
	static constexpr float separationWeight = 1.3f;

	static inline float predatorAvoidanceRadius = 100.f;
	static constexpr float predatorAvoidanceSpeed = 22.0f;

	static constexpr float minSpeed = 97.5f;
	static constexpr float maxSpeed = 187.75f;
	static constexpr float obstacleAvoidanceSpeed = 8.f;

private:
	static constexpr std::string_view beeImagePath = "../Assets/bee.png";

	static constexpr float screenOffset = 50.f;
	static constexpr float mass = 0.25f;
	static constexpr float linearDamping = 0.1f;
	static constexpr float restitution = 0.9f;
};

