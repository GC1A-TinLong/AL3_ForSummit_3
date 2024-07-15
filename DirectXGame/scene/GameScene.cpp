#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	if (fade_) {
		delete fade_;
	}
	delete mapChipField_;
	delete skydome_;
	delete modelSkydome_;
	delete modelBlock_;
	if (player_) {
		delete playerModel_;
		delete player_;
	}
	if (deathParticles_) {
		delete deathParticles_;
		delete deathParticlesModel_;
	}
	delete enemy_;
	delete enemyModel_;
	delete cameraController_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

#ifdef _DEBUG
	delete debugCamera_;
#endif // _DEBUG

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

#ifdef _DEBUG
	debugCamera_ = new DebugCamera(kWindowWidth, kWindowHeight);
#endif _DEBUG

	// Game Phase
	phase_ = Phase::kFadeIn;
	fade_ = new Fade;
	fade_->Start(Fade::Status::FadeIn, kFadeDuration);
	fade_->Initialize();

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipVsc("Resources/blocks.csv");

	modelBlock_ = Model::Create();
	// Block
	GenerateBlocks();
	// Skydome
	skydome_ = new Skydome;
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_, &viewProjection_);
	// Player
	player_ = new Player();
	playerModel_ = Model::CreateFromOBJ("player", true);
	playerTexture_ = TextureManager::Load("sample.png");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(playerModel_, &viewProjection_, playerPosition, playerMovableArea);
	player_->SetMapChipField(mapChipField_);
	// Death Particles
	// deathParticles_->Initialize(deathParticlesModel_, &viewProjection_, playerPosition);
	// Enemy
	enemy_ = new Enemy;
	enemyModel_ = Model::CreateFromOBJ("enemy", true);

	for (int32_t i = 0; i < kEnemyNum; i++) {
		Enemy* newEnemy = new Enemy;
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12 + (i * 2), 18 - (i * 2));
		newEnemy->Initialize(enemyModel_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}
	// Camera
	cameraController_ = new CameraController;
	cameraController_->Initialize(&viewProjection_, cameraMovableArea);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
}

void GameScene::Update() {

	CurrentPhase();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F)) {
		isDebugCameraActive_ ^= true;
	}
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
#endif // _DEBUG
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	skydome_->Draw();

	if (player_) {
		player_->Draw();
	}
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	if (enemy_) {
		for (Enemy* enemy : enemies_) {
			enemy->Draw();
		}
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	if (fade_) {
		fade_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVertical);
	for (uint32_t i = 0; i < numBlockVertical; i++) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// Generate blocks
	for (uint32_t i = 0; i < numBlockVertical; i++) {
		for (uint32_t j = 0; j < numBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
#pragma region player & enemy collision
	AABB aabb1{}, aabb2{};
	// Player position
	aabb1 = player_->GetAABB();

	// Player & all enemy collisions
	for (Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();

		if (IsCollideAABB(aabb1, aabb2)) {
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	}

#pragma endregion

#pragma region player & item collision

#pragma endregion

#pragma region player bullet & enemy collision

#pragma endregion
}

void GameScene::CurrentPhase() {

	switch (phase_) {
	case Phase::kFadeIn:
		skydome_->Update(); // skydome update
		cameraController_->Update();
		// block update
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}

		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}

		break;

	case Phase::kPlay:
		skydome_->Update(); // skydome update
		player_->Update();  // player update
		if (enemy_) {       //	enemy update
			for (Enemy* enemy : enemies_) {
				enemy->Update();
			}
		}
		// camera update
		cameraController_->Update();
		// block update
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}
		CheckAllCollisions(); // checking all collision

		if (player_->IsDead()) {
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();
			// initialize particle effect at player position
			deathParticles_ = new DeathParticles;
			deathParticlesModel_ = Model::CreateFromOBJ("deathParticles", true);
			deathParticles_->Initialize(deathParticlesModel_, &viewProjection_, deathParticlesPosition);

			if (player_) {
				delete playerModel_;
				playerModel_ = nullptr;
				delete player_;
				player_ = nullptr;
			}

			phase_ = Phase::kDeath;
		}

		break;

	case Phase::kDeath:
		skydome_->Update(); // skydome update
		if (enemy_) {       //	enemy update
			for (Enemy* enemy : enemies_) {
				enemy->Update();
			}
		}
		if (deathParticles_) { // death particles update
			deathParticles_->Update();
		}
		// camera update
		cameraController_->Update();
		// block update
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}

		if (deathParticles_ && deathParticles_->IsFinished()) {
			fade_->Start(Fade::Status::FadeOut, kFadeDuration);
			fade_->Initialize();

			phase_ = Phase::kFadeOut;
		}

		break;

	case Phase::kFadeOut:
		fade_->Update();

		if (fade_->IsFinished()) {
			isFinished_ = true;
		}
		break;
	}
}
