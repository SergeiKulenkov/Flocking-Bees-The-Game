#pragma once
#include <memory>
#include <string_view>
#include "glm/glm.hpp"

#include <Scene/Entity.h>
#include <Scene/Component/Transform.h>
#include <Scene/Component/Rigidbody.h>

////////////////////

class BoidBase : public Entity
{
public:
	BoidBase() = delete;
	virtual ~BoidBase() {}

	bool operator==(const BoidBase& other) const { return m_Id == other.GetId(); }

	virtual void Setup(const glm::vec2& screenSize, const uint16_t id) {}
	virtual void Update(float deltaTime) override;

	uint16_t GetId() const { return m_Id; }
	glm::vec2 GetPosition() const { return m_Transform->position; }
	glm::vec2 GetVelocity() const { return m_Velocity; }
	float GetRadius() const { return m_Radius; }

	void UpdateAcceleration(const glm::vec2& acceleration) { m_Acceleration += acceleration; }
	void UpdateVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }
	void AvoidBoundaries(const glm::vec2& direction) { UpdateVelocity(direction * m_ObstacleAvoidanceSpeed); }

	////////////////////

	static inline bool isDrawingDebug = false;

protected:
	BoidBase(const float minSpeed, const float maxSpeed, const float obstacleAvoidanceSpeed)
		: m_MinSpeed(minSpeed), m_MaxSpeed(maxSpeed), m_ObstacleAvoidanceSpeed(obstacleAvoidanceSpeed)
	{}

	virtual void OnInit() {}

	void Setup(const std::string_view& path, const glm::vec2& screenSize, const float screeOffset);

	////////////////////

	uint16_t m_Id = 0;
	glm::vec2 m_Velocity = glm::vec2(0, 0);
	glm::vec2 m_Acceleration = glm::vec2(0, 0);
	float m_Speed = 0.f;
	float m_MinSpeed = 0.f;
	float m_MaxSpeed = 0.f;
	float m_ObstacleAvoidanceSpeed = 0.f;
	float m_Radius = 0.f;

	std::shared_ptr<TransformData> m_Transform;
};