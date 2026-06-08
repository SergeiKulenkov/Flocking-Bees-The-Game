#pragma once
#include <memory>
#include <string_view>
#include "glm/glm.hpp"

#include <Scene/Entity.h>

struct TransformData;
class Rigidbody;

////////////////////

class MovingObject : public Entity
{
public:
	MovingObject() = delete;
	virtual ~MovingObject() {}

	bool operator==(const MovingObject& other) const { return m_Id == other.GetId(); }

	virtual void Setup(const glm::vec2& screenSize,const uint16_t id);
	virtual void Update(float deltaTime) override;

	uint16_t GetId() const { return m_Id; }
	glm::vec2 GetPosition() const { return m_CenterPosition; }
	glm::vec2 GetVelocity() const { return m_Velocity; }
	float GetRadius() const { return m_Radius; }

	void UpdateAcceleration(const glm::vec2& acceleration) { m_Acceleration += acceleration; }
	void UpdateVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }
	void AvoidBoundaries(const glm::vec2& direction) { UpdateVelocity(direction * m_ObstacleAvoidanceSpeed); }

	////////////////////

	static inline bool isDrawingDebug = false;

protected:
	MovingObject(const std::string_view& path, const float minSpeed, const float maxSpeed, const float obstacleAvoidanceSpeed);

	////////////////////

	static constexpr float screenOffset = 50.f;
	static constexpr float mass = 0.25f;
	static constexpr float linearDamping = 0.1f;
	static constexpr float restitution = 0.9f;

	uint16_t m_Id = 0;
	glm::vec2 m_CenterPosition = glm::vec2(0, 0);
	glm::vec2 m_Direction = glm::vec2(0, 0);
	glm::vec2 m_Velocity = glm::vec2(0, 0);
	glm::vec2 m_Acceleration = glm::vec2(0, 0);
	float m_MinSpeed = 0.f;
	float m_MaxSpeed = 0.f;
	float m_ObstacleAvoidanceSpeed = 0.f;
	float m_Radius = 0.f;
	std::string_view m_SpritePath;

	std::shared_ptr<TransformData> m_Transform;
	std::shared_ptr<Rigidbody> m_Rigidbody;
};