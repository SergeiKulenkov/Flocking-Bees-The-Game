#include "Wall.h"

#include <Scene/Component/Collider.h>

////////////////////

void Wall::Setup(glm::vec2 position, const glm::vec2& size)
{
	if (!HasComponent<Transform>()) AddComponent<Transform>(position);

	if (m_Size == glm::vec2(0.f, 0.f))
	{
		m_Size = size;
		if (size.x == 0) m_Size.x = thickness;
		else m_Size.y = thickness;
	}
	AddComponent<BoxCollider>(m_Size);
}

void Wall::DrawDebug(const RendererDebug& rendererDebug)
{
	const AABB boundingBox = GetComponent<BoxCollider>()->GetAABB();
	rendererDebug.DrawRectangle(boundingBox.min, boundingBox.max, m_Colour);
}