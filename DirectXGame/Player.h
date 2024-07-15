#pragma once

#define NOMINMAX
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include <imgui.h>
#include "Function.h"
#include "cassert"
#include <numbers>
#include <algorithm>
#include "CameraController.h"
#include "DeathParticles.h"

class MapChipField;
class Enemy;

enum class LRDirection {
	kRight,
	kLeft,
};

enum Corner { kBottomRight, kBottomLeft, kTopRight, kTopLeft, kNumCorner };

struct CollisionMapInfo {
	bool isHitCeiling = false;
	bool isHitGround = false;
	bool isHitWall = false;
	Vector3 velocity;
};

class Player {
public:
	Player();

	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	/// <param name="viewProjection"></param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, CameraController::Rect movableArea);
	void Update();
	void Draw();

	void PlayerMovement();
	void MovementInput();
	void PlayerDirection();

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void MovementByMapCollision(CollisionMapInfo& info);
	void WhenHitCeiling(const CollisionMapInfo& info);
	void SwitchToOnGround(const CollisionMapInfo& info);
	void WhenHitWall(const CollisionMapInfo& info);

	// Collision
	void OnCollision(const Enemy* enemy);

	// Getter
	const WorldTransform& GetWorldTransform() { return worldTransform_; };
	const Vector3& GetVelocity() const { return velocity_; };
	const Vector3 GetWorldPosition();
	const AABB GetAABB();
	bool IsDead() const { return isDead_; }
	// Setter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

private:
	void IsCollideMapTop(CollisionMapInfo& info);
	void IsCollideMapBottom(CollisionMapInfo& info);
	void IsCollideMapLeft(CollisionMapInfo& info);
	void IsCollideMapRight(CollisionMapInfo& info);
	void MapCollision(CollisionMapInfo& info);

	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	static inline const float kAcceleration = 0.05f;
	static inline const float kAttenuation = 0.15f;
	static inline const float kMaxVelocity = 9.8f;

	static inline const float kBlank = 1.0f;
	static inline const float kLeftWallBlank = 0.9f;
	static inline const float kRightWallBlank = 1.1f;
	static inline const float kAboveGroundDistance = 0.2f;
	static inline const float kAdjustWallDistance = 0.2f;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	DeathParticles* deathParticles_;

	Vector3 velocity_ = {};
	CameraController::Rect movableArea_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0;
	static inline const float kTimeTurn = 0.3f;

	static inline const float kGravityAcceleration = 0.08f;
	static inline const float kLimitFallSpeed = 1.4f;
	static inline const float kJumpAcceleration = 1.2f;
	static inline const float kAttenuationLanding = 0.2f;
	bool onGround_ = true;
	static inline const float kAttenuationWall = 0.2f;

	bool isDead_ = false;

	MapChipField* mapChipField_ = nullptr;
};