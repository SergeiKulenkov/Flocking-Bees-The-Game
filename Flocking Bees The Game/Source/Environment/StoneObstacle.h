#pragma once
#include <string_view>

#include "Wall.h"
#include <Scene/Component/Sprite.h>
#include <Scene/Component/Transform.h>

////////////////////

class StoneObstacle : public Wall
{
public:
	StoneObstacle() {}
	virtual ~StoneObstacle() {}

	virtual void Setup(glm::vec2 position, const glm::vec2& size = glm::vec2(0.f, 0.f)) override
	{
		AddComponent<Transform>(position);
		m_Size = AddComponent<Sprite>(stoneImagePath)->GetSize();

		Wall::Setup(position, size);
	}

protected:
	virtual void OnInit() override {}

private:
	static constexpr std::string_view stoneImagePath = "../Assets/stone.png";
};