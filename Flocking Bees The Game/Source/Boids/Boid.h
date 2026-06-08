#pragma once
#include "MovingObject.h"

////////////////////

class Boid : public MovingObject
{
public:
	Boid() : MovingObject(beeImagePath, minSpeed, maxSpeed, obstacleAvoidanceSpeed) {}

	virtual void DrawDebug(const RendererDebug& rendererDebug) override;

	static inline float perceptionRadius = 50.f;
	static inline float separationRadius = 35.f;
	static constexpr float allignmentWeight = 0.8f;
	static constexpr float cohesionWeight = 0.8f;
	static constexpr float separationWeight = 1.3f;

	static inline float predatorAvoidanceRadius = 100.f;
	static constexpr float predatorAvoidanceSpeed = 22.0f;

	static constexpr float minSpeed = 97.5f;
	static constexpr float maxSpeed = 187.75f;
	static constexpr float obstacleAvoidanceSpeed = 8.f;

private:
	static constexpr std::string_view beeImagePath = "../Assets/bee.png";
};

