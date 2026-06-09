#pragma once
#include "BoidBase.h"

////////////////////

class Hornet : public BoidBase
{
public:
	Hornet() : BoidBase(predatorImagePath, minSpeed, maxSpeed, obstacleAvoidanceSpeed) {}

	virtual void Update(float deltaTime) override;
	virtual void DrawDebug(const RendererDebug& rendererDebug) override;

	static constexpr float separationRadius = 125.f;
	static constexpr float separationWeight = 1.5f;
	static constexpr float minSpeed = 67.5f;
	static constexpr float maxSpeed = 112.25f;
	static constexpr float obstacleAvoidanceSpeed = 5.f;

private:
	static constexpr std::string_view predatorImagePath = "../Assets/predator.png";
};