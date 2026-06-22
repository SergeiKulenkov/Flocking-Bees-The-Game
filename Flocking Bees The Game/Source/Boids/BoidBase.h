#pragma once
#include <memory>
#include <string_view>
#include "glm/glm.hpp"

#include <Scene/Entity.h>
#include <Scene/Component/Transform.h>

class BoidsManager;

////////////////////

class BoidBase : public Entity
{
public:
	BoidBase() = delete;
	virtual ~BoidBase() { m_Manager = nullptr; }

	bool operator==(const BoidBase& other) const { return m_Id == other.GetId(); }

	uint16_t GetId() const { return m_Id; }
	glm::vec2 GetPosition() const { return m_Transform->position; }
	glm::vec2 GetVelocity() const { return m_Velocity; }
	float GetRadius() const { return m_Radius; }

	////////////////////

	static inline bool isDrawingDebug = false;
	// this is just to make wall avoidance easier while testing raycasting for obstacle avoidance
	static inline glm::vec2 boundaries = glm::vec2(0.f, 0.f);

protected:
	BoidBase(const float minSpeed, const float maxSpeed, const float raycastLength)
		: m_MinSpeed(minSpeed), m_MaxSpeed(maxSpeed), m_RaycastLength(raycastLength)
	{}

	virtual void OnInit() override {}
	virtual void OnDestroy() override {}
	virtual void Update(float deltaTime) override;

	virtual void Setup(const glm::vec2& screenSize, const uint16_t id, BoidsManager* manager) {}
	void Setup(const std::string_view& path, const glm::vec2& screenSize, const float screeOffset);

	void UpdateSteeringForce(const glm::vec2& force) { m_SteeringForce += force; }
	void UpdateVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }
	//void Rotate(const float angle) {  }

	void CheckWalls();
	virtual void UpdateObstacleAvoidance(const bool hitWall, const glm::vec2& rayContactPoint, const uint8_t rayId) {}
	void CheckBoundaries();

	////////////////////

	static constexpr uint8_t framesBetweenRaycast = 5;
	static constexpr uint8_t numebrOfRays = 5;
	static constexpr float raycastAngleStep = 0.349f;

	// this should be set during Setup and reset to null when the manager is destroyed
	// also useful if there are different teams with team managers, instead of using a static reference
	BoidsManager* m_Manager = nullptr;
	uint16_t m_Id = 0;

	glm::vec2 m_Velocity = glm::vec2(0, 0);
	glm::vec2 m_SteeringForce = glm::vec2(0, 0);
	float m_Speed = 0.f;
	float m_MinSpeed = 0.f;
	float m_MaxSpeed = 0.f;
	float m_Radius = 0.f;

	uint8_t m_FrameCounter = 0;
	float m_RaycastLength = 0.f;
	bool m_RotateClockwise = false;

	std::shared_ptr<TransformData> m_Transform;

	// temporary while testing raycasting for obstacle avoidance
	float m_BoundariesAvoidanceSpeed = 0.f;
	float m_EdgeMargin = 0.f;

	friend class BoidsManager;
};