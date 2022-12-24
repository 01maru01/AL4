#pragma once
#include "VertIdxBuff.h"
#include "Material.h"
#include "VertexStruct.h"
#include <unordered_map>

class Mesh :public VertIdxBuff
{
private:
	// 頂点データ配列
	std::vector<Vertex> vertices;
	// 頂点インデックス配列
	std::vector<uint16_t> indices;
	// 頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	// マテリアル
	Material* material = nullptr;

public:

	void Draw();
private:
	void SetVertices() override;
};

