#include "Model.h"
#include "ObjFile.h"
#include <string>
#include <sstream>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

MyDirectX* Model::dx = MyDirectX::GetInstance();

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	std::ifstream file;
	file.open(directoryPath + filename);
	if (file.fail()) { assert(0); }

	Material* material = nullptr;
	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		if (key == "newmtl") {
			// 既にマテリアルがあれば
			if (material) {
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			// マテリアル名読み込み
			line_stream >> material->name;
		}

		if (key == "Ka") {
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}

		if (key == "Kd") {
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}

		if (key == "Ks") {
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}

		if (key == "map_Kd") {
			line_stream >> material->textureFilename;

			string filepath = directoryPath + material->textureFilename;
			MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, material->wfilepath, _countof(material->wfilepath));
		}
	}
	file.close();

	if (material) {
		// マテリアルを登録
		AddMaterial(material);
	}
}

void Model::LoadModel(const std::string& modelname, bool smoothing)
{
	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3D> temp_poss;
	std::vector<Vector2D> temp_uvs;
	std::vector<Vector3D> temp_normals;

	std::ifstream file_;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/Model/" + modelname + "/";
	file_.open(directoryPath + filename);
	assert(!file_.fail());

	meshes.emplace_back(new Mesh);
	Mesh* mesh = meshes.back();
	int indexCount = 0;

	string line;
	while (getline(file_, line)) {
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib") {
			string filename_;
			line_stream >> filename_;
			LoadMaterial(directoryPath, filename_);
		}

		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr) {
				// マテリアルの名読み込み
				string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials.find(materialName);
				if (itr != materials.end()) {
					mesh->SetMaterial(itr->second);
				}
			}
		}

		if (key == "v") {
			Vector3D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			temp_poss.emplace_back(pos);
		}

		if (key == "vt") {
			Vector2D uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uv.y = 1.0f - uv.y;

			temp_uvs.emplace_back(uv);
		}

		if (key == "vn") {
			Vector3D normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			temp_normals.emplace_back(normal);
		}

		if (key == "f") {
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				std::istringstream index_stream(index_string);
				unsigned short indexPos, indexNormal, indexUV;
				index_stream >> indexPos;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexUV;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				Vertex vertex{};
				vertex.pos = temp_poss[indexPos - 1];
				vertex.normal = temp_normals[indexNormal - 1];
				vertex.uv = temp_uvs[indexUV - 1];

				mesh->AddVertex(vertex);

				if (smoothing) {
					mesh->AddSmoothData(indexPos, (unsigned short)mesh->GetVertexCount() - 1);
				}

				mesh->AddIndex((unsigned short)indexCount);
				indexCount++;
			}
		}
	}

	file_.close();

	if (smoothing) {
		mesh->CalcSmoothedNormals();
	}
}

std::string GetDirectoryPath(const std::string& origin)
{
    int ind = (int)origin.find_last_of('/');
    ind++;
    return origin.substr(0, ind);
}

std::wstring ToWideString(const std::string& str)
{
    auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

    std::wstring wstr;
    wstr.resize(num1);

    auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

    assert(num1 == num2);
    return wstr;
}

void Model::LoadFBXMesh(Mesh& dst, const aiMesh* src)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);

	for (auto i = 0u; i < src->mNumVertices; ++i)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		//auto tangent = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &zero3D;
		//auto color = (src->HasVertexColors(0)) ? &(src->mColors[0][i]) : &zeroColor;

		uv->y = 1 - uv->y;

		Vertex vertex = {};
		vertex.pos = Vector3D(position->x, position->y, position->z);
		vertex.normal = Vector3D(normal->x, normal->y, normal->z);
		vertex.uv = Vector2D(uv->x, uv->y);
		//vertex.Tangent = DirectX::XMFLOAT3(tangent->x, tangent->y, tangent->z);
		//vertex.Color = DirectX::XMFLOAT4(color->r, color->g, color->b, color->a);

		dst.AddVertex(vertex);
	}

	for (auto i = 0u; i < src->mNumFaces; ++i)
	{
		const auto& face = src->mFaces[i];

		dst.AddIndex((unsigned short)face.mIndices[0]);
		dst.AddIndex((unsigned short)face.mIndices[1]);
		dst.AddIndex((unsigned short)face.mIndices[2]);
	}
}

void Model::LoadFBXTexture(const std::string& filename, Mesh& dst, const aiMaterial* src)
{
	aiString path;
	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		auto dir = GetDirectoryPath(filename);
		auto file = std::string(path.C_Str());
		dst.SetTextureFilePath(dir + file);
		dst.GetMaterial()->name = file;
	}
}

void Model::LoadFBXModel(const std::string& filename)
{
	const string directoryPath = "Resources/Model/";

	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

	auto result = importer.ReadFile(directoryPath + filename, flag);

	if (result == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < result->mNumMeshes; ++i)
	{
		meshes.emplace_back(new Mesh);
		Mesh* mesh = meshes.back();

		const auto pMesh = result->mMeshes[i];
		LoadFBXMesh(*mesh, pMesh);
		const auto pMaterial = result->mMaterials[i];
		mesh->SetMaterial(Material::Create());
		LoadFBXTexture(directoryPath + filename, *mesh, pMaterial);
		mesh->GetMaterial()->name += to_string(i);
		if (mesh->GetMaterial()) {
			// マテリアルを登録
			AddMaterial(mesh->GetMaterial());
		}
	}

	result = nullptr;
}

Model::~Model()
{
	for (auto m : meshes) {
		delete m;
	}
	meshes.clear();

	for (auto m : materials) {
		delete m.second;
	}
	materials.clear();
}

void Model::Initialize(const char* filename, bool isFBX, bool smoothing)
{
	if (isFBX) {
		LoadFBXModel(filename);
	}
	else {
		LoadModel(filename, smoothing);
	}

	//// メッシュのマテリアルチェック
	//for (auto& m : meshes) {
	//	// マテリアルの割り当てがない
	//	if (m->GetMaterial() == nullptr) {
	//		if (defaultMaterial == nullptr) {
	//			// デフォルトマテリアルを生成
	//			defaultMaterial = Material::Create();
	//			defaultMaterial->name = "no material";
	//			materials.emplace(defaultMaterial->name, defaultMaterial);
	//		}
	//		// デフォルトマテリアルをセット
	//		m->SetMaterial(defaultMaterial);
	//	}
	//}

	for (auto& m : meshes) {
		m->CreateBuff();
	}

	for (auto& m : materials) {
		m.second->Update();
	}

	for (auto& m : materials) {
		m.second->LoadTexture(isFBX);
	}
}

Model::Model(const char* filename, bool isFBX, bool smoothing)
{
	Initialize(filename, isFBX, smoothing);
}

void Model::Draw()
{
	for (auto& mesh : meshes) {
		mesh->Draw();
	}
}
