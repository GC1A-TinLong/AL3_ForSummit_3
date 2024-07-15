#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <algorithm>

class Player;

class CameraController {
public:
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void Initialize(ViewProjection* viewProjection, Rect movableArea);

	void Update();

	void SetTarget(Player* target) { target_ = target; };
	void SetMovableArea(Rect area) { movableArea_ = area; };

	void Reset();

private:
	static inline const float kInterpolationRate = 1.0f;
	static inline const float kVelocityBias = 2.7f;
	static inline const Rect margin = {-14.0f, 14.0f, -14.0f, 14.0f};

	ViewProjection* viewProjection_ = nullptr;
	WorldTransform worldTransform_;

	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -40.0f};
	Rect movableArea_ = {0, 100.0f, 0, 100.0f};

	Vector3 targetPosition_ = {};
};

//CameraController::Rect CameraController::movableArea_ = {0, 100.0f, 0, 100.0f};