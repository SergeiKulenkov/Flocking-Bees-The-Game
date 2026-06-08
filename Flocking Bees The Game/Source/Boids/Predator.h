#pragma once
#include "MovingObject.h"

////////////////////

class Predator : public MovingObject
{
public:
	Predator() : MovingObject(predatorImagePath, minSpeed, maxSpeed, obstacleAvoidanceSpeed) {}

	virtual void DrawDebug(const RendererDebug& rendererDebug) override;

	static constexpr float separationRadius = 125.f;
	static constexpr float separationWeight = 1.5f;
	static constexpr float minSpeed = 67.5f;
	static constexpr float maxSpeed = 112.25f;
	static constexpr float obstacleAvoidanceSpeed = 5.f;

private:
	static constexpr std::string_view predatorImagePath = "../Assets/predator.png";
};