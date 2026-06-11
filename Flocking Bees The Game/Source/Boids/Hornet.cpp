#include "Hornet.h"

#include <Scene/Scene.h>
#include <Scene/Component/Transform.h>

////////////////////

void Hornet::Setup(const glm::vec2& screenSize, const uint16_t id, BoidsManager* manager)
{
	m_Id = id;
	m_Manager = manager;
	BoidBase::Setup(hornetImagePath, screenSize, screenOffset);
}

void Hornet::Update(float deltaTime)
{
	// avoid predators
	BoidBase::Update(deltaTime);
}

void Hornet::DrawDebug(const RendererDebug& rendererDebug)
{
	if (isDrawingDebug)
	{
		rendererDebug.DrawCircle(m_Transform->position, separationRadius, Colour::green);
	}
}
