#include "Player.h"

#include <Input/InputManager.h>
#include <Scene/Scene.h>
#include <Scene/Physics.h>
#include <Scene/Component/Transform.h>
#include <Scene/Component/Tag.h>
#include <Scene/Component/Sprite.h>
#include <Scene/Component/Collider.h>
#include <Scene/Component/Rigidbody.h>
#include <Utility/Utility.h>

////////////////////

void Player::OnInit()
{
	m_Transform = AddComponent<Transform>(defaultPosition)->GetTransformData();
	const glm::vec2 size = AddComponent<Sprite>(spritePath)->GetSize();
	
	const float radius = glm::min(size.x, size.y);
	AddComponent<CircleCollider>(radius / 2.f);
	m_Rigidbody = AddComponent<Rigidbody>(1.f, linearDamping, restitution);
}

void Player::Update(float deltaTime)
{
	const glm::vec2 input = GetMovementInput();
	if (input.x != 0)
	{
		m_Transform->rotation = Vector::Rotate(m_Transform->rotation, rotationRate * deltaTime * input.x);
	}

	if (input.y != 0)
	{
		if (input.y == 1)
		{
			m_Speed += acceleration;
		}
		else if (input.y == -1)
		{
			m_Speed += deceleration * (-1);
		}

		m_Speed = glm::clamp(m_Speed, 0.f, m_MaxSpeed);
		m_Rigidbody->ApplyForce(Force(m_Transform->rotation * m_Speed, false));
	}
	else if (m_Speed > 0)
	{
		m_Speed -= linearDamping;
		if (m_Speed < 0.f) m_Speed = 0.f;
	}
}

void Player::OnCollision(Collision& other)
{
	m_Speed = 0.f;
}

glm::vec2 Player::GetMovementInput() const
{
	glm::vec2 input = glm::vec2(0, 0);
	if (InputManager::IsKeyDown(KeyCode::W))
		input.y = 1;
	else if (InputManager::IsKeyDown(KeyCode::S))
		input.y = -1;
	if (InputManager::IsKeyDown(KeyCode::A))
		input.x = -1;
	else if (InputManager::IsKeyDown(KeyCode::D))
		input.x = 1;

	return input;
}

void Player::DrawDebug(const RendererDebug& rendererDebug)
{
	rendererDebug.DrawCircle(m_Transform->position, GetComponent<CircleCollider>()->GetRadius(), Colour::green);
}