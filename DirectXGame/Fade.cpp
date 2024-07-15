#include "Fade.h"

void Fade::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	sprite_ = sprite_->Create(TextureManager::Load("blackfade.png"), {0, 0});
	sprite_->SetSize({1280.0f, 720.0f});
	if (status_ == Status::FadeIn) {
		sprite_->SetColor({0, 0, 0, 1});
	} else if (status_ == Status::FadeOut) {
		sprite_->SetColor({0, 0, 0, 0});
	}
}

void Fade::Update() {
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
		counter_ += 1.0f / 60.0f;
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1 - (counter_ / duration_), 0.0f, 1.0f)));

		break;
	case Status::FadeOut:
		counter_ += 1.0f / 60.0f;
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

		break;
	}
}

void Fade::Draw() {
	if (status_ == Status::None) {
		return;
	}
	sprite_->Draw();
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const {
	switch (status_) {
	case Status::FadeIn:
	case Status::FadeOut:
		return counter_ >= duration_;
	}
	return true;
}
