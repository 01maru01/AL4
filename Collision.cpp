#include "Collision.h"

void Collision::ClosestPtPoint2Triangle(const Vector3D& point, const Triangle& triangle, Vector3D* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	Vector3D p0_p1 = triangle.p1 - triangle.p0;
	Vector3D p0_p2 = triangle.p2 - triangle.p0;
	Vector3D p0_pt = point - triangle.p0;

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	Vector3D p1_pt = point - triangle.p1;

	float d3 = p0_p1.dot(p1_pt);
	float d4 = p0_p2.dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0;
		*closest += v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3D p2_pt = point - triangle.p2;

	float d5 = p0_p1.dot(p2_pt);
	float d6 = p0_p2.dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0;
		*closest += w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1;
		*closest += w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0;
	*closest += p0_p1 * v;
	*closest += p0_p2 * w;
}

bool Collision::CheclSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3D* inter)
{
    float dis = sphereA.radius + sphereB.radius;
    Vector3D disV = sphereA.center;
    disV -= sphereB.center;
    
    if (disV.length() > dis) return false;

    return true;
}

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3D* inter)
{
    Vector3D distV = sphere.center;
    float dist = distV.dot(plane.normal) - plane.distance;

    if (fabsf(dist) > sphere.radius) return false;

    if (inter) {
        *inter = -dist * plane.normal;
        *inter += sphere.center;
    }

    return true;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3D* inter)
{
    Vector3D p;
    ClosestPtPoint2Triangle(sphere.center, triangle, &p);
    Vector3D v = p - sphere.center;
    if (v.dot(v) > sphere.radius * sphere.radius) return false;

    if (inter) *inter = p;
    return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector3D* inter)
{
    const float epsilon = 1.0e-5f;
    Vector3D vec = plane.normal;
    float d1 = vec.dot(ray.dir);
    if (d1 > -epsilon) return false;

    float d2 = vec.dot(ray.start);
    float dis = d2 - plane.distance;
    float t = dis / -d1;
    if (t < 0) return false;

    if (distance) { *distance = t; }
    if (inter) {
        *inter = ray.start;
        *inter += t * ray.dir;
    }
    return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3D* inter)
{
    Vector3D m = ray.start;
    m -= sphere.center;
    float b = m.dot(ray.dir);
    float c = m.dot(m) - sphere.radius * sphere.radius;
    if (c > 0.0f && b > 0.0f) return false;

    float discr = b * b - c;
    if (discr < 0.0f) return false;

    float t = -b - sqrtf(discr);
    if (t < 0) t = 0.0f;
    if (distance) *distance = t;
    if (inter) {
        *inter = ray.start;
        *inter += t * ray.dir;
    }
    return true;
}
