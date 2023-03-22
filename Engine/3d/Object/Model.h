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

struct BoneInfo
{
	Matrix boneOffset;
	Matrix finalTransformation;
};

class Model
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int MAX_BONE_INDICES = 4;
	static MyDirectX* dx;
	//	fbxÇÃèÓïÒéÊìæópïœêî
	const aiScene* modelScene;
	Assimp::Importer importer;

	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, Material*> materials;
	Material* defaultMaterial = nullptr;

	Matrix globalInverseTransform;
	std::map<std::string, UINT> boneMapping;
	UINT numBones = 0;
	std::vector<BoneInfo> boneInfo;
public:
	Model(const char* filename, bool isFBX = false, bool smoothing = false);
	~Model();

	void Initialize(const char* filename, bool isFBX, bool smoothing);
	void BoneTransform(float TImeInSeconds, std::vector<Matrix>& transforms);
	void Draw();

	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }
	const Matrix& GetModelTransform() { return globalInverseTransform; }
	UINT GetNumBones() { return numBones; }
	std::vector<BoneInfo> GetBoneInfo() { return boneInfo; }
private:
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void LoadModel(const std::string& modelname, bool smoothing);

	void LoadFBXMaterial(Mesh* dst, const aiMaterial* src, int index);
	void LoadFBXModel(const std::string& modelname);
	void LoadFBXMesh(Mesh& dst, const aiMesh* src);
	void LoadFBXBone(UINT meshIndex, const aiMesh* src);
	void LoadFBXTexture(const std::string& filename, Mesh& dst, const aiMaterial* src);

	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform);
	
	void AddMaterial(Material* material) { materials.emplace(material->name, material); }
};

