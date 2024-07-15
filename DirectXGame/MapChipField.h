#pragma once
#include <cstdint>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <Vector3.h>

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct Rect {
	float left;
	float right;
	float bottom;
	float top;
};

class MapChipField {
public:
	MapChipData mapChipData_;
	/// <summary>
	/// reset mapchip data
	/// </summary>
	void ResetMapChipData();
	/// <summary>
	/// load mapchip data from CSV
	/// </summary>
	/// <param name="filePath"></param>
	void LoadMapChipVsc(const std::string& filePath);
	/// <summary>
	/// get type from mapchip data
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	/// <summary>
	/// get mapchip position
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVertical() { return kNumBlockVertical; }
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	static inline const float kBlockHeight = 2.0f;
	static inline const float kBlockWidth = 2.0f;

	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
};
