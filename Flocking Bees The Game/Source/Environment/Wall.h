#pragma once
#include <Scene/Entity.h>
#include <Scene/Scene.h>
#include <Scene/Component/Collider.h>

////////////////////

class Wall : public Entity
{
public:
	Wall() {}
	virtual ~Wall() {}

	void ChangeColour(const uint32_t colour) { m_Colour = colour; }

	virtual void Setup(glm::vec2 position, const glm::vec2& size = glm::vec2(0.f, 0.f));

protected:
	virtual void OnInit() override {}
	virtual void Update(float deltaTime) override {}

	virtual void DrawDebug(const RendererDebug& rendererDebug) override;

	////////////////////

	// colour for drawing the box colliders
	uint32_t m_Colour = Colour::green;
	glm::vec2 m_Size = glm::vec2(0.f, 0.f);

private:
	static constexpr float thickness = 5.f;
};