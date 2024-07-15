#pragma once
#include "MapChipField.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <numbers>

class Player;

class Enemy {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	void OnCollision(const Player* player);

	// Getter
	const Vector3 GetWorldPosition();
	const AABB GetAABB();
	// Setter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

private:
	enum class LRDirection {
		kRight,
		kLeft,
	};
	// Size
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	// Walking speed
	static inline const float kWalkSpeed = 0.05f;
	static inline const float kInitialWalkMotionAngle = -60.0f; // Initial angle
	static inline const float kFinalWalkMotionAngle = 120.0f;   // Final angle
	static inline const float kWalkMotionTime = 1.0f;           // Animation loop time
	float walkTimer_ = 0.0f;
	// Velocity
	Vector3 velocity_ = {};

	MapChipField* mapChipField_ = nullptr;

	LRDirection lrDirection_ = LRDirection::kLeft;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
};
