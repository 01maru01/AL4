#pragma once
#include "VertIdxBuff.h"
#include "Material.h"
#include "VertexStruct.h"
#include <unordered_map>

class Mesh :public VertIdxBuff
{
private:
	static MyDirectX* dx;

	// ���_�f�[�^�z��
	std::vector<Vertex> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<uint16_t> indices;
	// ���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	// �}�e���A��
	Material* material = nullptr;
public:
	void CreateBuff();
	void Draw();

	void AddSmoothData(unsigned short index, unsigned short vertex);
	void CalcSmoothedVertexNormals();
private:
	void SetVertices() override;
};

