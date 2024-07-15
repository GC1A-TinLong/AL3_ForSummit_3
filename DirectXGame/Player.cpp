#include "Player.h"
#include "DebugText.h"
#include "MapChipField.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, CameraController::Rect movableArea) {
	assert(model);
	model_ = model;
	// textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	movableArea_ = movableArea;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	PlayerMovement();

	CollisionMapInfo collisionMapInfo; // Initializing collision info
	collisionMapInfo.velocity = velocity_;

	MapCollision(collisionMapInfo);

	// Restrict movable area
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, movableArea_.left, movableArea_.right);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, movableArea_.bottom, movableArea_.top);

	PlayerDirection();

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("window");
	ImGui::InputFloat3("Velocity", &velocity_.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat3("Translation", &worldTransform_.translation_.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat3("info.velocity", &collisionMapInfo.velocity.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::End();
#endif // _DEBUG
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }

void Player::PlayerMovement() { MovementInput(); }

void Player::MovementInput() {
	// LR Movement
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_D)) {
				if (lrDirection_ != LRDirection::kRight) { // player direction
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

				if (velocity_.x < 0.0f) { // when pushing opposing input, attenuate velocity
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_A)) { // player direction
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

				if (velocity_.x > 0.0f) { // when pushing opposing input, attenuate velocity
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
			}
			velocity_.x = velocity_.x + acceleration.x;
			velocity_.x = std::clamp(velocity_.x, -kMaxVelocity, kMaxVelocity);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
			if (fabsf(velocity_.x) < 0.05f) {
				velocity_.x = 0;
			}
		}
	}
	// Jump
	bool isLand = false;
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			velocity_.y += kJumpAcceleration;
		}
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		// Fall speed
		velocity_.y += -kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		// is falling? Collision with ground
		if (velocity_.y < 0.0f) {
			// if translation Y is lower than the ground, landed
			if (worldTransform_.translation_.y <= 2.0f) {
				worldTransform_.translation_.y = 2.0f; // prevent going underground
				isLand = true;
			}
		}
		if (isLand) {
			worldTransform_.translation_.y = 2.0f; // prevent going underground
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f; // reseting fall speed
			onGround_ = true;
		}
	}
}

void Player::PlayerDirection() {
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		// Player's Left & Right angle table
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,
		    std::numbers::pi_v<float> * 3.0f / 2.0f,
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		float easing = 1 - turnTimer_ / kTimeTurn;
		float nowRotationY = std::lerp(turnFirstRotationY_, destinationRotationY, easing);
		// Get angle from status
		worldTransform_.rotation_.y = nowRotationY;
	}
}

void Player::MovementByMapCollision(CollisionMapInfo& info) { worldTransform_.translation_ += info.velocity; }

void Player::WhenHitCeiling(const CollisionMapInfo& info) {
	if (info.isHitCeiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

void Player::SwitchToOnGround(const CollisionMapInfo& info) {
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			std::array<Vector3, 4> positionNew{};
			for (uint32_t i = 0; i < positionNew.size(); i++) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
			}

			MapChipType mapChipType{};
			// Check directly below
			// check bottom left collision
			IndexSet indexSet{};
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomLeft] + Vector3(0, kAboveGroundDistance, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

			bool hit = false;
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// check bottom right collision
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomRight] + Vector3(0, kAboveGroundDistance, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			if (!hit) {
				velocity_.y += -kGravityAcceleration;
				velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
			}
		}
	} else {
		if (info.isHitGround) {
			onGround_ = true;
			// when landed, decay the speed of x
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
		}
	}
}

void Player::WhenHitWall(const CollisionMapInfo& info) {
	if (info.isHitWall) {
		velocity_.x *= 1.0f - kAttenuationWall;
	}
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	if (deathParticles_) {
		deathParticles_->Update();
	}
	isDead_ = true;
}

const Vector3 Player::GetWorldPosition() {
	Vector3 worldPos{};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

const AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb{};
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::IsCollideMapTop(CollisionMapInfo& info) {
	std::array<Vector3, kNumCorner> positionNew{};
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}
	if (info.velocity.y <= 0) {
		return;
	}

	MapChipType mapChipType{};
	// check directly above top
	bool hit = false;
	// check top left collision
	IndexSet indexSet{};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kTopLeft]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	} else {
		// check top right collision
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kTopRight]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition({worldTransform_.translation_.x + kWidth / 2.0f, worldTransform_.translation_.y + kHeight / 2.0f, 0});
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - kHeight + 0.2f); //
		// record it when hitting the ceiling
		info.isHitCeiling = true;
	}
}
void Player::IsCollideMapBottom(CollisionMapInfo& info) {
	std::array<Vector3, kNumCorner> positionNew{};
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}
	if (info.velocity.y >= 0) {
		return;
	}

	MapChipType mapChipType{};
	// Check directly below
	bool hit = false;
	// check bottom left collision
	IndexSet indexSet{};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomLeft] + Vector3(0, -kAboveGroundDistance, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// check bottom right collision
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomRight] + Vector3(0, -kAboveGroundDistance, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// Started falling
	if (!hit) {
		// Switch to airborne
		onGround_ = false;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition({worldTransform_.translation_.x + kWidth / 2.0f, worldTransform_.translation_.y - kHeight / 2.0f, 0});
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		// calculate the velocty again to prevent going underground
		info.velocity.y = std::min(0.0f, rect.bottom - worldTransform_.translation_.y - kBlank);
		// record it when hitting the floor
		info.isHitGround = true;
	}
}
void Player::IsCollideMapLeft(CollisionMapInfo& info) {
	if (velocity_.x > 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew{};
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType{};
	// Check directly below
	bool hit = false;
	IndexSet indexSet{};

	bool isOnGround = true;
	IndexSet botLeftIndex = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomLeft]);
	IndexSet botRightIndex = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomRight]);
	MapChipType botLeftType = mapChipField_->GetMapChipTypeByIndex(botLeftIndex.xIndex, botLeftIndex.yIndex);
	MapChipType botRightType = mapChipField_->GetMapChipTypeByIndex(botRightIndex.xIndex, botRightIndex.yIndex);
	if (botLeftType == MapChipType::kBlock && botRightType == MapChipType::kBlock) {
		isOnGround = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kTopLeft] + Vector3(-kAdjustWallDistance, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (!isOnGround) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomLeft] + Vector3(-kAdjustWallDistance, 0, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition({worldTransform_.translation_.x + kWidth / 2.0f, worldTransform_.translation_.y - kHeight / 2.0f, 0});
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		// calculate the velocty again to prevent going inside wall
		info.velocity.x = std::min(0.0f, rect.left - worldTransform_.translation_.x - kLeftWallBlank);
		// record it when hitting the floor
		info.isHitWall = true;
	}
}
void Player::IsCollideMapRight(CollisionMapInfo& info) {
	if (velocity_.x < 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew{};
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType{};
	// Check directly below
	bool hit = false;
	IndexSet indexSet{};

	bool isOnGround = true;
	IndexSet botLeftIndex = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomLeft]);
	IndexSet botRightIndex = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomRight]);
	MapChipType botLeftType = mapChipField_->GetMapChipTypeByIndex(botLeftIndex.xIndex, botLeftIndex.yIndex);
	MapChipType botRightType = mapChipField_->GetMapChipTypeByIndex(botRightIndex.xIndex, botRightIndex.yIndex);
	if (botLeftType == MapChipType::kBlock && botRightType == MapChipType::kBlock) {
		isOnGround = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kTopRight] + Vector3(kAdjustWallDistance, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (!isOnGround) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kBottomRight] + Vector3(kAdjustWallDistance, 0, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition({worldTransform_.translation_.x + kWidth / 2.0f, worldTransform_.translation_.y - kHeight / 2.0f, 0});
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		// calculate the velocty again to prevent going inside wall
		info.velocity.x = std::max(0.0f, rect.right - worldTransform_.translation_.x - kRightWallBlank);
		// record it when hitting the floor
		info.isHitWall = true;
	}
}

void Player::MapCollision(CollisionMapInfo& info) {
	IsCollideMapTop(info);
	WhenHitCeiling(info);

	IsCollideMapBottom(info);
	SwitchToOnGround(info);

	IsCollideMapLeft(info);
	IsCollideMapRight(info);
	WhenHitWall(info);

	MovementByMapCollision(info);
}
