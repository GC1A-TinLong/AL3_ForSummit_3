#include "CameraController.h"
#include "Player.h"

void CameraController::Initialize(ViewProjection* viewProjection, Rect movableArea) {
	viewProjection_ = viewProjection;
	movableArea_ = movableArea;
}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	Vector3 targetVelocity = target_->GetVelocity();
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;
	// Camera Lerp
	viewProjection_->translation_.x = std::lerp(viewProjection_->translation_.x, targetPosition_.x, kInterpolationRate);
	viewProjection_->translation_.y = std::lerp(viewProjection_->translation_.y, targetPosition_.y, kInterpolationRate);
	// Camera Margin
	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, targetWorldTransform.translation_.x + margin.left, 
		targetWorldTransform.translation_.x + margin.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, targetPosition_.y + margin.bottom, 
		targetPosition_.y + margin.top);
	// Movable Area
	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, movableArea_.left, movableArea_.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, movableArea_.bottom, movableArea_.top);

	viewProjection_->UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}
