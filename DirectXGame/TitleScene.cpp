#include "TitleScene.h"

TitleScene::TitleScene() { delete fade_; }

TitleScene::~TitleScene() {}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = position_;
	viewProjection_.Initialize();
	model_ = Model::CreateFromOBJ("title", true);

	fade_ = new Fade;
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, kFadeDuration);
}

void TitleScene::Update() {
	if (!startBuffer) {
		t += 1.0f / kEndFrame;
	}
	if (t >= 1.0f && !startBuffer) {
		startBuffer = true;
	}
	if (startBuffer) {
		buffer++;
		if (buffer >= bufferEnd) {
			t = 0.0f;
			buffer = 0;
			startBuffer = false;
			float tempPos = startPos;
			startPos = endPos;
			endPos = tempPos;
		}
	}
	worldTransform_.translation_.y = startPos + (endPos - startPos) * Easing();

	worldTransform_.UpdateMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		fade_->Start(Fade::Status::FadeOut, kFadeDuration);
		fade_->Initialize();
		startChange = true;
	}
	if (fade_->IsFinished() && startChange) {
		isFinished_ = true;
	}

	fade_->Update();

#ifdef _DEBUG
	ImGui::Begin("window");
	ImGui::DragFloat3("worldTransform_", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat("t", &t, 0.1f);
	ImGui::End();
#endif // _DEBUG
}
void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	model_->Draw(worldTransform_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	if (fade_) {
		fade_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//float TitleScene::Easing() const { return t < 0.5f ? 16.0f * t * t * t * t * t : 1 - pow(-2.0f * t + 2.0f, 5.0f) / 2.0f; }
float TitleScene::Easing() const { return -(cos(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f; }
