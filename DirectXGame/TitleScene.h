#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <imgui.h>
#include <iostream>
#include <numbers>
#include <cmath>
#include <Input.h>
#include "Fade.h"

class TitleScene 
{
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	TitleScene();
	~TitleScene();

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return isFinished_; }

	float Easing() const;

private:
	DirectXCommon* dxCommon_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Model* model_ = nullptr;

	Fade* fade_ = nullptr;
	static inline const float kFadeDuration = 1.0f;
	Phase phase_ = Phase::kFadeIn;

	static inline const float kEndFrame = 45.0f;

	float t = 0;
	bool startBuffer = false;
	uint32_t buffer = 0;
	uint32_t bufferEnd = 3;
	float startPos = 12.0f;
	float endPos = 6.0f;
	static inline const Vector3 position_ = {0, 10.0f, 0};
	
	bool isFinished_ = false;
	bool startChange = false;
};
