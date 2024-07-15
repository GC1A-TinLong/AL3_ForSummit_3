#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include <algorithm>

class Fade {
public:
	// status of fade
	enum class Status {
		None,
		FadeIn,
		FadeOut,
	};

	void Initialize();
	void Update();
	void Draw();

	void Start(Status status, float duration);
	void Stop();

	bool IsFinished() const;

private:
	DirectXCommon* dxCommon_ = nullptr;

	Sprite* sprite_ = nullptr;

	Status status_ = Status::None;
	float duration_ = 0.0f;
	float counter_ = 0.0f;
};
