#include "Bee.h"

#include <Scene/Scene.h>
#include <Scene/Component/Transform.h>

////////////////////

void Bee::Setup(const glm::vec2& screenSize, const uint16_t id, BoidsManager* manager)
{
	m_Id = id;
	m_Manager = manager;
	BoidBase::Setup(beeImagePath, screenSize, screenOffset);
}

void Bee::Update(float deltaTime)
{
	// flock
	// avoid predators
	BoidBase::Update(deltaTime);
}

void Bee::DrawDebug(const RendererDebug& rendererDebug)
{
	if (isDrawingDebug)
	{
		rendererDebug.DrawCircle(m_Transform->position, perceptionRadius, Colour::green);
		rendererDebug.DrawCircle(m_Transform->position, separationRadius, Colour::yellow);
		rendererDebug.DrawCircle(m_Transform->position, predatorAvoidanceRadius, Colour::red);
	}
}