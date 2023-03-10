#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "Material.h"
#include "Light.h"
#include "Mesh.h"

struct aiMesh;
struct aiMaterial;
struct aiNode;
struct aiScene;

struct Node
{
	std::string name;
	//	メッシュのインデックス
	std::vector<int> meshIndex;
	//	ローカル変換行列
	Matrix transform;
	//	ワールド変換行列
	Matrix worldTransform;
	//	親
	Node* parent = nullptr;
};

struct Bone
{
	std::string name;

	Matrix invInitialPose;

	//	FBX Bone Inf


	Bone(const std::string& name) {
		this->name = name;
	}
};

class Model
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;
public:
	static const int MAX_BONE_INDICES = 4;

	const aiScene* modelScene;

	std::vector<Node> nodes;

	std::vector<Bone> bones;

	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, Material*> materials;
	Material* defaultMaterial = nullptr;

	Node* meshNode = nullptr;

	float time = 0.0f;
public:
	Model(const char* filename, bool isFBX = false, bool smoothing = false);
	~Model();

	void Initialize(const char* filename, bool isFBX, bool smoothing);
	void Draw();

	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }
	const Matrix& GetModelTransform() { return meshNode->worldTransform; }
	std::vector<Bone>& GetBone() { return bones; }
private:
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void LoadModel(const std::string& modelname, bool smoothing);
	void LoadFBXModel(const std::string& modelname);
	void LoadFBXMesh(Mesh& dst, const aiMesh* src);
	void LoadFBXNode(const aiNode* src, Node* parent = nullptr);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform);
	void LoadFBXTexture(const std::string& filename, Mesh& dst, const aiMaterial* src);
	
	void AddMaterial(Material* material) { materials.emplace(material->name, material); }
};

