#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "Material.h"
#include "Light.h"
#include "Mesh.h"

class Model
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;
public:
	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, Material*> materials;
	Material* defaultMaterial = nullptr;

	void Initialize(const char* filename, bool smoothing);
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void LoadModel(const std::string& modelname, bool smoothing);
public:
	Model() {};
	~Model();
	Model(const char* filename, bool smoothing = false);
	void Draw();
private:
	void AddMaterial(Material* material) { materials.emplace(material->name, material); }
};

