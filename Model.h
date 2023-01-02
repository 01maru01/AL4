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

class Model
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;
public:
	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, Material*> materials;
	Material* defaultMaterial = nullptr;

public:
	Model(const char* filename, bool isFBX = false, bool smoothing = false);
	~Model();

	void Initialize(const char* filename, bool isFBX, bool smoothing);
	void Draw();

	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }
private:
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void LoadModel(const std::string& modelname, bool smoothing);
	void LoadFBXModel(const std::string& filename);
	void LoadFBXMesh(Mesh& dst, const aiMesh* src);
	void LoadFBXTexture(const std::string& filename, Mesh& dst, const aiMaterial* src);
	
	void AddMaterial(Material* material) { materials.emplace(material->name, material); }
};

