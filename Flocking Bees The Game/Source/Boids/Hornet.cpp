#include "Hornet.h"

#include <Scene/Scene.h>
#include <Scene/Component/Transform.h>

////////////////////

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
