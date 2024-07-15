#pragma once
#include "CameraController.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <numbers>
#include "Function.h"

class DeathParticles {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	bool IsFinished() const { return isFinished_; }

private:
	static inline const uint32_t kNumParticles = 8;
	static inline const float kDuration = 1.5f;
	static inline const float kSpeed = 0.05f;	// particle speed
	// angle per nums of particle
	static inline const float kAngleUnit = (2 * std::numbers::pi_v<float>) / kNumParticles; 

	bool isFinished_ = false;
	float timer_ = 1.0f / 60.0f;
	float counter_ = 0.0f;

	ObjectColor objectColor_;
	Vector4 color_;

	std::array<WorldTransform, kNumParticles> worldTransform_;
	Model* model_;
	ViewProjection* viewProjection_ = nullptr;
};
