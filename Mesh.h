#pragma once
#include "VertIdxBuff.h"
#include "Material.h"
#include "DirectX.h"
#include <unordered_map>

class Mesh :public VertIdxBuff
{
private:
    static MyDirectX* dx;
private:
    std::vector<Vertex> vertices; // 頂点データの配列
    std::vector<uint16_t> indices; // インデックスの配列
    Material* mtl = nullptr;
    std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
    std::wstring DiffuseMap; // テクスチャのファイルパス

public:
    void CreateBuff();
    void Draw();
    void CalcSmoothedNormals();

    void SetMaterial(Material* material) { mtl = material; }

    Material* GetMaterial() { return mtl; }
    size_t GetVertexCount() { return vertices.size(); }

    void AddIndex(unsigned short index) { indices.emplace_back(index); }
    void AddVertex(const Vertex& vertex) { vertices.emplace_back(vertex); }
    void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex) { smoothData[indexPosition].emplace_back(indexVertex); }
private:
    void SetVertices() override;
};

