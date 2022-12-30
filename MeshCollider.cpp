#include "MeshCollider.h"
#include "Collision.h"

void MeshCollider::ConstructTriangles(Model* model)
{
	triangles.clear();
	const std::vector<Mesh*>& meshes = model->GetMeshes();
	int start = 0;
	std::vector<Mesh*>::const_iterator itr = meshes.cbegin();

	for (; itr != meshes.cend(); ++itr) {
		Mesh* mesh = *itr;
		const std::vector<Vertex>& vertices = mesh->GetVertices();
		const std::vector<unsigned short>& indices = mesh->GetIndices();

		size_t triangleNum = indices.size() / 3;
		triangles.resize(triangles.size() + triangleNum);

		for (int i = 0; i < triangleNum; i++) {
			Triangle& tri = triangles[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];

			tri.p0 = {
				vertices[idx0].pos.x,
				vertices[idx0].pos.y,
				vertices[idx0].pos.z};

			tri.p1 = {
				vertices[idx1].pos.x,
				vertices[idx1].pos.y,
				vertices[idx1].pos.z};

			tri.p2 = {
				vertices[idx2].pos.x,
				vertices[idx2].pos.y,
				vertices[idx2].pos.z};

			tri.ComputeNormal();
		}

		start += (int)triangleNum;
	}
}

void MeshCollider::Update()
{
	InverseMatrix(GetObject3D()->GetMatWorld(), invMatWorld);
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3D* inter)
{
	Sphere localSphere;
	localSphere.center = Vec3Transform(sphere.center, invMatWorld);
	Vector3D vec(invMatWorld.m[0][0], invMatWorld.m[0][1], invMatWorld.m[0][2]);
	localSphere.radius *= vec.length();

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle& triangle = *it;

		if (Collision::CheckSphere2Triangle(localSphere, triangle, inter)) {
			if (inter) {
				const Matrix& matWorld = GetObject3D()->GetMatWorld();

				*inter = Vec3Transform(*inter, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* dis, Vector3D* inter)
{
	// オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = Vec3Transform(ray.start, invMatWorld);
	localRay.dir = Vec3TransformNormal(ray.dir, invMatWorld);

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle& triangle = *it;
		Vector3D tempInter;

		if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter)) {

			const Matrix& matWorld = GetObject3D()->GetMatWorld();
			tempInter = Vec3Transform(tempInter, matWorld);

			if (dis) {
				Vector3D sub = tempInter - ray.start;
				*dis = sub.dot(ray.dir);
			}
			if (inter) {
				*inter = tempInter;
			}
			return true;
		}
	}

	return false;
}
