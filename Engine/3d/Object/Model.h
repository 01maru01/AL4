#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "Material.h"
#include "Light.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <map>

struct aiMesh;
struct aiMaterial;
struct aiNode;
struct aiScene;

//struct Node
//{
//	std::string name;
//	//	���b�V���̃C���f�b�N�X
//	std::vector<int> meshIndex;
//	//	���[�J���ϊ��s��
//	Matrix transform;
//	//	���[���h�ϊ��s��
//	Matrix worldTransform;
//	//	�e
//	Node* parent = nullptr;
//};
//
//struct Bone
//{
//	std::string name;
//
//	Matrix invInitialPose;
//
//	//	FBX Bone Inf
//
//
//	Bone(const std::string& name) {
//		this->name = name;
//	}
//};

struct BoneInfo
{
	Matrix boneOffset;
	Matrix finalTransformation;
};

class Model
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;

	const aiScene* modelScene;
	Assimp::Importer importer;
public:
	static const int MAX_BONE_INDICES = 4;


	//std::vector<Node> nodes;

	//std::vector<Bone> bones;

	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, Material*> materials;
	Material* defaultMaterial = nullptr;

	//Node* meshNode = nullptr;


	//	3/15
	Matrix m_GlobalInverseTransform;
	std::map<std::string, UINT> boneMapping;
	UINT numBones = 0;
	std::vector<BoneInfo> boneInfo;

	float time = 0.0f;
public:
	Model(const char* filename, bool isFBX = false, bool smoothing = false);
	~Model();

	void Initialize(const char* filename, bool isFBX, bool smoothing);
	void BoneTransform(float TImeInSeconds, std::vector<Matrix>& transforms);
	void Draw();

	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }
	const Matrix& GetModelTransform() { return m_GlobalInverseTransform; }
	//std::vector<Bone>& GetBone() { return bones; }
	UINT GetNumBones() { return numBones; }
	std::vector<BoneInfo> GetBoneInfo() { return boneInfo; }
private:
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void LoadModel(const std::string& modelname, bool smoothing);

	void LoadFBXMaterial(Mesh* dst, const aiMaterial* src, int index);
	void LoadFBXModel(const std::string& modelname);
	void LoadFBXMesh(Mesh& dst, const aiMesh* src);
	void LoadFBXBone(UINT meshIndex, const aiMesh* src);
	//void LoadFBXNode(const aiNode* src, Node* parent = nullptr);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform);
	void LoadFBXTexture(const std::string& filename, Mesh& dst, const aiMaterial* src);
	
	void AddMaterial(Material* material) { materials.emplace(material->name, material); }
};

