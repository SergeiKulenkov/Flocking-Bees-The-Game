#pragma once
#include <array>
#include <memory>
#include <string_view>

#include <Scene/Entity.h>
#include "Wall.h"
#include "StoneObstacle.h"

////////////////////

class EnvironmentManager : public Entity
{
public:
	EnvironmentManager() {}
	virtual ~EnvironmentManager() {}

protected:
	// maybe the manager should call Update for the environment objects
	virtual void Update(float deltaTime) override {}

private:
	virtual void OnInit() override;

	////////////////////
	static constexpr std::string_view backgroundImagePath = "../Assets/background.png";
	static constexpr uint8_t numberOfBoundaries = 4;
	static constexpr uint8_t numberOfStaticObstacles = 4;
	static constexpr float stoneObstacleOffset = 100.f;

	// values are multiplied by screen size
	static constexpr std::array<glm::vec2, numberOfBoundaries> boundaryPositions = { glm::vec2(0.5f, 0), glm::vec2(1, 0.5f), glm::vec2(0.5f, 1), glm::vec2(0, 0.5f) };
	static constexpr float boundaryScreenOffset = 5.f;

	std::array<std::weak_ptr<Wall>, numberOfBoundaries> m_Boundaries;
	std::array<std::weak_ptr<StoneObstacle>, numberOfStaticObstacles> m_StaticObstacles;
};