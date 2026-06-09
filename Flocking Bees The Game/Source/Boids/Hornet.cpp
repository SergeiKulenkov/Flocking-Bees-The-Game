#include "Hornet.h"

#include <Scene/Scene.h>
#include <Scene/Component/Transform.h>

////////////////////

void Hornet::Setup(const glm::vec2& screenSize, const uint16_t id)
{
	m_Id = id;
	BoidBase::Setup(hornetImagePath, screenSize, screenOffset);
	m_Rigidbody = AddComponent<Rigidbody>(mass, linearDamping, restitution);
}

void Hornet::Update(float deltaTime)
{

}

void Hornet::DrawDebug(const RendererDebug& rendererDebug)
{
	if (isDrawingDebug)
	{
		rendererDebug.DrawCircle(m_Transform->position, m_Radius + separationRadius, Colour::green);
	}
}
