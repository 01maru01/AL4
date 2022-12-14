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
    std::string diffuseMap; // テクスチャのファイルパス

public:
    void CreateBuff();
    void Draw();
    void CalcSmoothedNormals();

    void SetMaterial(Material* material) { mtl = material; }

    Material* GetMaterial() { return mtl; }
    size_t GetVertexCount() { return vertices.size(); }

    inline const std::vector<Vertex>& GetVertices() { return vertices; }
    inline const std::vector<unsigned short>& GetIndices() { return indices; }

    void AddIndex(unsigned short index) { indices.emplace_back(index); }
    void AddVertex(const Vertex& vertex) { vertices.emplace_back(vertex); }
    void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex) { smoothData[indexPosition].emplace_back(indexVertex); }
    void SetTextureFilePath(const std::string& filePath) {
        int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, mtl->wfilepath, _countof(mtl->wfilepath));
    }
private:
    void SetVertices() override;
};

