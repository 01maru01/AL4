#include "Model.h"
#include <string>
#include <sstream>
#include <fstream>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Quaternion.h"
using namespace std;

MyDirectX* Model::dx = MyDirectX::GetInstance();

//	転置して変換
void TransformMatToAiMat(Matrix& mat, const aiMatrix4x4 aiMat)
{
	mat.m[0][0] = aiMat.a1;
	mat.m[1][0] = aiMat.a2;
	mat.m[2][0] = aiMat.a3;
	mat.m[3][0] = aiMat.a4;
	mat.m[0][1] = aiMat.b1;
	mat.m[1][1] = aiMat.b2;
	mat.m[2][1] = aiMat.b3;
	mat.m[3][1] = aiMat.b4;
	mat.m[0][2] = aiMat.c1;
	mat.m[1][2] = aiMat.c2;
	mat.m[2][2] = aiMat.c3;
	mat.m[3][2] = aiMat.c4;
	mat.m[0][3] = aiMat.d1;
	mat.m[1][3] = aiMat.d2;
	mat.m[2][3] = aiMat.d3;
	mat.m[3][3] = aiMat.d4;
}

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

		if (key == "o") {

			// カレントメッシュの情報が揃っているなら
			if (mesh->GetVertexCount() > 0) {
				// 頂点法線の平均によるエッジの平滑化
				if (smoothing) {
					mesh->CalcSmoothedNormals();
				}
				// 次のメッシュ生成
				meshes.emplace_back(new Mesh);
				mesh = meshes.back();
				indexCount = 0;
			}

			//// グループ名読み込み
			//string groupName;
			//line_stream >> groupName;

			//// メッシュに名前をセット
			//mesh->SetName(groupName);
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
			int indexNum = 0;

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

				FBXVertex vertex{};
				vertex.pos = temp_poss[indexPos - 1];
				vertex.normal = temp_normals[indexNormal - 1];
				vertex.uv = temp_uvs[indexUV - 1];
				vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
				mesh->AddVertex(vertex);

				if (smoothing) {
					mesh->AddSmoothData(indexPos, (unsigned short)mesh->GetVertexCount() - 1);
				}

				if (indexNum >= 3) {
					mesh->AddIndex((unsigned short)indexCount - 1);
					mesh->AddIndex((unsigned short)indexCount);
					mesh->AddIndex((unsigned short)indexCount - 3);
				}
				else
				{
					mesh->AddIndex((unsigned short)indexCount);
				}
				indexNum++;
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

	//	頂点生成
	for (UINT i = 0; i < src->mNumVertices; i++)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		//	uv反転
		uv->y = 1 - uv->y;

		FBXVertex vertex = {};
		vertex.pos = Vector3D(position->x, position->y, position->z);
		vertex.normal = Vector3D(normal->x, normal->y, normal->z);
		vertex.uv = Vector2D(uv->x, uv->y);

		dst.AddVertex(vertex);
	}

	//	index設定
	for (UINT i = 0; i < src->mNumFaces; i++)
	{
		const auto& face = src->mFaces[i];

		dst.AddIndex((unsigned short)face.mIndices[0]);
		dst.AddIndex((unsigned short)face.mIndices[1]);
		dst.AddIndex((unsigned short)face.mIndices[2]);
	}
}

void Model::LoadFBXBone(UINT meshIndex, const aiMesh* src)
{
	//	bone情報設定
	for (UINT i = 0; i < src->mNumBones; i++) {
		UINT BoneIndex = 0;
		string BoneName(src->mBones[i]->mName.data);

		if (boneMapping.find(BoneName) == boneMapping.end()) {
			BoneIndex = numBones;
			numBones++;
			BoneInfo bi;
			boneInfo.push_back(bi);
			//	型変換
			TransformMatToAiMat(boneInfo[BoneIndex].boneOffset, src->mBones[i]->mOffsetMatrix);
			boneMapping[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = boneMapping[BoneName];
		}

		for (UINT j = 0; j < src->mBones[i]->mNumWeights; j++) {
			UINT VertexID = src->mBones[i]->mWeights[j].mVertexId;
			float Weight = src->mBones[i]->mWeights[j].mWeight;
			meshes[meshIndex]->SetBone(VertexID, BoneIndex, Weight);
		}
	}
}

const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, std::string name)
{
	for (UINT i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == name) {
			return pNodeAnim;
		}
	}

	return NULL;
}

UINT FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);
	return 0;
}

UINT FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);
	return 0;
}

void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// 補間には最低でも２つの値が必要…
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	UINT RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	UINT NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	UINT ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	UINT NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

UINT FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}

void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	UINT PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	UINT NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform)
{
	string NodeName(pNode->mName.data);

	//	複数アニメーションあるならここで変更
	const aiAnimation* pAnimation = modelScene->mAnimations[0];

	Matrix NodeTransformation;
	TransformMatToAiMat(NodeTransformation, pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// スケーリングを補間し、スケーリング変換行列を生成する
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		MyMath::ObjMatrix mat;
		mat.scale = Vector3D(Scaling.x, Scaling.y, Scaling.z);
		mat.SetMatScaling();

		// 回転を補間し、回転変換行列を生成する
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Quaternion rotQ(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
		mat.matRot = rotQ.GetRotMatrix();

		// 移動を補間し、移動変換行列を生成する
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		mat.trans = Vector3D(Translation.x, Translation.y, Translation.z);
		mat.SetMatTransform();

		// これら上記の変換を合成する
		NodeTransformation = mat.matScale;
		NodeTransformation *= mat.matRot;
		NodeTransformation *= mat.matTrans;
	}

	Matrix GlobalTransformation = NodeTransformation;
	GlobalTransformation *= ParentTransform;

	if (boneMapping.find(NodeName) != boneMapping.end()) {
		UINT BoneIndex = boneMapping[NodeName];
		boneInfo[BoneIndex].finalTransformation = globalInverseTransform;
		boneInfo[BoneIndex].finalTransformation *= boneInfo[BoneIndex].boneOffset;
		boneInfo[BoneIndex].finalTransformation *= GlobalTransformation;
	}

	for (UINT i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
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

void Model::LoadFBXMaterial(Mesh* dst, const aiMaterial* src, int index)
{
	Material* material = Material::Create();

	material->name += to_string(index);
	
	//	Diffuse
	aiColor3D difcolor(0.f, 0.f, 0.f);
	src->Get(AI_MATKEY_COLOR_DIFFUSE, difcolor);
	material->diffuse.x = difcolor.r;
	material->diffuse.y = difcolor.g;
	material->diffuse.z = difcolor.b;
	//	AMBIENT
	aiColor3D amcolor(0.f, 0.f, 0.f);
	src->Get(AI_MATKEY_COLOR_AMBIENT, amcolor);
	material->ambient.x = amcolor.r;
	material->ambient.y = amcolor.g;
	material->ambient.z = amcolor.b;
	//	SPECULAR
	aiColor3D specolor(0.f, 0.f, 0.f);
	src->Get(AI_MATKEY_COLOR_SPECULAR, specolor);
	material->specular.x = specolor.r;
	material->specular.y = specolor.g;
	material->specular.z = specolor.b;

	if (material) {
		// マテリアルを登録
		AddMaterial(material);
	}

	dst->SetMaterial(material);
}

void Model::LoadFBXModel(const std::string& modelname)
{
	//	パスの設定
	const string filename = modelname + ".fbx";
	const string directoryPath = "Resources/Model/" + modelname + "/";

	//	シーンのロード
	modelScene = importer.ReadFile(directoryPath + filename, aiProcess_Triangulate);	//	三角面化
	
	//	読み込み失敗したら
	if (modelScene == nullptr) { return; }

	//	GlobalInverseTransform設定
	TransformMatToAiMat(globalInverseTransform, modelScene->mRootNode->mTransformation);

	//	mesh情報設定
	meshes.reserve(modelScene->mNumMeshes);
	for (UINT i = 0; i < modelScene->mNumMeshes; i++)
	{
		//	aiMesh型の情報取得
		const auto pMesh = modelScene->mMeshes[i];

		//	型変換
		meshes.emplace_back(new Mesh);
		Mesh* mesh = meshes.back();
		LoadFBXMesh(*mesh, pMesh);
		//	ボーン取得
		LoadFBXBone(i, pMesh);

		//	material取得
		aiMaterial* pMaterial = modelScene->mMaterials[i];

		LoadFBXMaterial(mesh, pMaterial, i);

		LoadFBXTexture(directoryPath + filename, *mesh, pMaterial);
	}
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
		m.second->LoadTexture();
	}
}

void Model::BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms)
{
	Matrix Identity;

	double TicksPerSecond = modelScene->mAnimations[0]->mTicksPerSecond != 0 ? modelScene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * (float)TicksPerSecond;
	float AnimationTime = (float)fmod(TimeInTicks, modelScene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(AnimationTime, modelScene->mRootNode, Identity);

	transforms.resize(numBones);

	for (UINT i = 0; i < numBones; i++) {
		transforms[i] = boneInfo[i].finalTransformation;
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
