#include "EnvironmentManager.h"

#include <Scene/Component/Transform.h>
#include <Scene/Component/Sprite.h>

////////////////////

void EnvironmentManager::OnInit()
{
	ASSERT_SCENE_NULLPTR(m_Scene);
	const glm::vec2 screenSize = m_Scene->GetScreenSize();
	AddComponent<Transform>(glm::vec2(screenSize.x / 2.f, screenSize.y / 2.f));
	AddComponent<Sprite>(backgroundImagePath);

	std::shared_ptr<Entity> newEntity;
	glm::vec2 position = glm::vec2(0.f, 0.f);

	for (size_t i = 0; i < m_Boundaries.size(); i++)
	{
		newEntity = m_Scene->CreateEntity<Wall>();
		const std::shared_ptr<Wall> newWall = std::dynamic_pointer_cast<Wall>(newEntity);

		if (newWall != nullptr)
		{
			glm::vec2 size = glm::vec2(0.f, 0.f);
			if (i % 2 == 0) size.x = screenSize.x;
			else size.y = screenSize.y;

			position = boundaryPositions[i] * screenSize;
			if (boundaryPositions[i].x == 0) position.x += boundaryScreenOffset;
			else if (boundaryPositions[i].x == 1) position.x -= boundaryScreenOffset;
			if (boundaryPositions[i].y == 0) position.y += boundaryScreenOffset;
			else if (boundaryPositions[i].y == 1) position.y -= boundaryScreenOffset;

			newWall->Setup(position, size);
			m_Boundaries[i] = newWall;
		}
	}


	for (size_t i = 0; i < m_StaticObstacles.size(); i++)
	{
		newEntity = m_Scene->CreateEntity<StaticObstacle>();
		const std::shared_ptr<StaticObstacle> newWall = std::dynamic_pointer_cast<StaticObstacle>(newEntity);

		if (newWall != nullptr)
		{
			position.x = Random::RandomInRange<float>(staticObstacleOffset, screenSize.x - staticObstacleOffset);
			position.y = Random::RandomInRange<float>(staticObstacleOffset, screenSize.y - staticObstacleOffset);
			newWall->Setup(position);
			m_StaticObstacles[i] = newWall;
		}
	}
}