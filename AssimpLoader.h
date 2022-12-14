#pragma once
#define NOMINMAX
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "VertBuff.h"

struct AssimpVertex
{
    DirectX::XMFLOAT3 Position; // 位置座標
    DirectX::XMFLOAT3 Normal; // 法線
    DirectX::XMFLOAT2 UV; // uv座標
    DirectX::XMFLOAT3 Tangent; // 接空間
    DirectX::XMFLOAT4 Color; // 頂点色
};
struct Mesh
{
    std::vector<Vertex> Vertices; // 頂点データの配列
    std::vector<uint32_t> Indices; // インデックスの配列
    std::wstring DiffuseMap; // テクスチャのファイルパス
};

struct aiMesh;
struct aiMaterial;

struct ImportSettings // インポートするときのパラメータ
{
    const wchar_t* filename = nullptr; // ファイルパス
    std::vector<Mesh>& meshes; // 出力先のメッシュ配列
    bool inverseU = false; // U座標を反転させるか
    bool inverseV = false; // V座標を反転させるか
};

class AssimpLoader
{
public:
    bool Load(ImportSettings setting); // モデルをロードする

private:
    void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
    void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};
