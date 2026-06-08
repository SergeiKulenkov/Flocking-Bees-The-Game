#include "Boid.h"

#include <Scene/Scene.h>
#include <Scene/Component/Transform.h>

////////////////////

void Boid::DrawDebug(const RendererDebug& rendererDebug)
{
	if (isDrawingDebug)
	{
		rendererDebug.DrawCircle(m_Transform->position, m_Radius + perceptionRadius, Colour::green);
		rendererDebug.DrawCircle(m_Transform->position, m_Radius + separationRadius, Colour::yellow);
		rendererDebug.DrawCircle(m_Transform->position, m_Radius + predatorAvoidanceRadius, Colour::red);
	}
}