#include "Collision.h"

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
