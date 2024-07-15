#include "MapChipField.h"
#include <fstream>
#include <sstream>

namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};
}

void MapChipField::ResetMapChipData() {
	// Reset mapchip data
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipVsc(const std::string& filePath) {
	ResetMapChipData(); // reset mapchip data
	std::ifstream file; // open the file
	file.open(filePath);
	assert(file.is_open());

	std::stringstream mapChipCsv; // mapchip Csv
	mapChipCsv << file.rdbuf();   // copy the file to the stringstream
	file.close();                 // close file

	// Load mapchip data from CSV
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		std::string line;
		std::getline(mapChipCsv, line);
		std::istringstream line_stream(line); // transform string to stream for easier analysis

		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			std::string word;
			std::getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVertical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { 
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), 0); 
}

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet{};
	indexSet.xIndex = uint32_t((position.x + (kBlockWidth / 2)) / kBlockWidth);
	indexSet.yIndex = uint32_t((kBlockHeight * kNumBlockVertical - position.y) / kBlockHeight);
	//indexSet.yIndex = uint32_t(kNumBlockVertical - 1 - ((position.y + (kBlockHeight / 2)) / kBlockHeight));
	//indexSet.yIndex = (((uint32_t)kBlockHeight * (uint32_t)kNumBlockVertical - (uint32_t)position.y) / (uint32_t)kBlockHeight);
	return indexSet;
}

Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);
	Rect rect = {
	    center.x - kBlockWidth  / 2.0f,
	    center.x + kBlockWidth  / 2.0f,
	    center.y + kBlockHeight / 2.0f,
	    center.y - kBlockHeight / 2.0f,
	};
	return rect;
}
