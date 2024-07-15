#include "Enemy.h"

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f * 3.0f;

	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	worldTransform_.translation_.x += velocity_.x;

	walkTimer_ += 1.0f / 60.0f;
	worldTransform_.rotation_.x = std::sin(2 * std::numbers::pi_v<float> * (walkTimer_ / kWalkMotionTime));
	float param = std::sin(2 * std::numbers::pi_v<float> * (walkTimer_ / kWalkMotionTime));
	float radian = kInitialWalkMotionAngle + (kFinalWalkMotionAngle * (param + 1.0f) / 2.0f);
	worldTransform_.rotation_.x = (std::numbers::pi_v<float> * radian) / 180.0f;

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void Enemy::OnCollision(const Player* player) { (void)player; }

const Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos{};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

const AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb{};
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
