#pragma once
#define NOMINMAX
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "VertBuff.h"

struct AssimpVertex
{
    DirectX::XMFLOAT3 Position; // �ʒu���W
    DirectX::XMFLOAT3 Normal; // �@��
    DirectX::XMFLOAT2 UV; // uv���W
    DirectX::XMFLOAT3 Tangent; // �ڋ��
    DirectX::XMFLOAT4 Color; // ���_�F
};
struct Mesh
{
    std::vector<Vertex> Vertices; // ���_�f�[�^�̔z��
    std::vector<uint32_t> Indices; // �C���f�b�N�X�̔z��
    std::wstring DiffuseMap; // �e�N�X�`���̃t�@�C���p�X
};

struct aiMesh;
struct aiMaterial;

struct ImportSettings // �C���|�[�g����Ƃ��̃p�����[�^
{
    const wchar_t* filename = nullptr; // �t�@�C���p�X
    std::vector<Mesh>& meshes; // �o�͐�̃��b�V���z��
    bool inverseU = false; // U���W�𔽓]�����邩
    bool inverseV = false; // V���W�𔽓]�����邩
};

class AssimpLoader
{
public:
    bool Load(ImportSettings setting); // ���f�������[�h����

private:
    void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
    void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};
