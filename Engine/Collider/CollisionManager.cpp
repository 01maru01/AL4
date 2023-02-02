#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"

CollisionManager* CollisionManager::GetInstance()
{
    static CollisionManager instance;
    return &instance;
}

void CollisionManager::CheckAllCollisions()
{
    std::forward_list<BaseCollider*>::iterator itA;
    std::forward_list<BaseCollider*>::iterator itB;

    itA = colliders.begin();

    for (; itA != colliders.end(); ++itA) {
        itB = itA;
        ++itB;
        for (; itB != colliders.end(); ++itB) {
            BaseCollider* colA = *itA;
            BaseCollider* colB = *itB;

            if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
                colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
                //  球同士の当たり判定
                Sphere* sphereA = dynamic_cast<Sphere*>(colA);
                Sphere* sphereB = dynamic_cast<Sphere*>(colB);
                Vector3D inter;
                if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter)) {
                    colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
                    colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
                colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
                MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
                Sphere* sphere = dynamic_cast<Sphere*>(colB);
                Vector3D inter;
                if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
                    colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
                    colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
                colB->GetShapeType() == COLLISIONSHAPE_MESH) {
                MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
                Sphere* sphere = dynamic_cast<Sphere*>(colA);
                Vector3D inter;
                if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
                    colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
                    colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
                colB->GetShapeType() == COLLISIONSHAPE_PLANE) {
                Plane* plane = dynamic_cast<Plane*>(colB);
                Sphere* sphere = dynamic_cast<Sphere*>(colA);
                Vector3D inter;
                if (Collision::CheckSphere2Plane(*sphere, *plane, &inter)) {
                    colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
                    colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
                }
            }
            else if (colB->GetShapeType() == COLLISIONSHAPE_SPHERE &&
                colA->GetShapeType() == COLLISIONSHAPE_PLANE) {
                Plane* plane = dynamic_cast<Plane*>(colA);
                Sphere* sphere = dynamic_cast<Sphere*>(colB);
                Vector3D inter;
                if (Collision::CheckSphere2Plane(*sphere, *plane, &inter)) {
                    colA->OnCollision(CollisionInfo(colB->GetObject3D(), colB, inter));
                    colB->OnCollision(CollisionInfo(colA->GetObject3D(), colA, inter));
                }
            }
        }
    }
}

bool CollisionManager::Raycast(const Ray& ray, RayCast* hitinfo, float maxDistance)
{
    return Raycast(ray, 0xFFFF, hitinfo, maxDistance);
}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RayCast* hitinfo, float maxDistance)
{
    bool ans = false;
    std::forward_list<BaseCollider*>::iterator itr;
    std::forward_list<BaseCollider*>::iterator itr_hit;
    float distance = maxDistance;
    Vector3D inter;

    itr = colliders.begin();
    for (; itr != colliders.end(); ++itr) {
        BaseCollider* colA = *itr;

        if (!(colA->attribute & attribute)) continue;

        if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            Sphere* sphere = dynamic_cast<Sphere*>(colA);
            float tempDis;
            Vector3D tempInter;

            if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDis, &tempInter)) continue;
            if (tempDis >= distance) continue;

            ans = true;
            distance = tempDis;
            inter = tempInter;
            itr_hit = itr;
        }
        else if (colA->GetShapeType() == COLLISIONSHAPE_MESH) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

            float tempDistance;
            Vector3D tempInter;
            if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
            if (tempDistance >= distance) continue;

            ans = true;
            distance = tempDistance;
            inter = tempInter;
            itr_hit = itr;
        }
    }

    if (ans && hitinfo) {
        hitinfo->distance = distance;
        hitinfo->inter = inter;
        hitinfo->collider = *itr_hit;
        hitinfo->object = hitinfo->collider->GetObject3D();
    }

    return ans;
}

void CollisionManager::QuerySphere(const Sphere& sphere, QueryCallBack* callback, unsigned short attribute)
{
    assert(callback);

    std::forward_list<BaseCollider*>::iterator it;

    it = colliders.begin();
    for (; it != colliders.end(); ++it) {
        BaseCollider* col = *it;

        if (!(col->attribute & attribute)) continue;

        // 球
        if (col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            Sphere* sphereB = dynamic_cast<Sphere*>(col);

            Vector3D tempInter;
            Vector3D tempReject;
            if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

            QueryHit info;
            info.collider = col;
            info.object = col->GetObject3D();
            info.inter = tempInter;
            info.reject = tempReject;

            if (!callback->OnQueryHit(info)) return;
        }
        // メッシュ
        else if (col->GetShapeType() == COLLISIONSHAPE_MESH) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

            Vector3D tempInter;
            Vector3D tempReject;
            if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

            QueryHit info;
            info.collider = col;
            info.object = col->GetObject3D();
            info.inter = tempInter;
            info.reject = tempReject;

            if (!callback->OnQueryHit(info)) return;
        }
        else if (col->GetShapeType() == COLLISIONSHAPE_PLANE) {
            Plane* planeB = dynamic_cast<Plane*>(col);

            Vector3D tempInter;
            if (!Collision::CheckSphere2Plane(sphere, *planeB, &tempInter)) continue;

            QueryHit info;
            info.collider = col;
            info.object = col->GetObject3D();
            info.inter = tempInter;

            if (!callback->OnQueryHit(info)) return;
        }
    }
}

bool CollisionManager::CheckCollision(const Sphere& sphere, unsigned short attribute)
{
    std::forward_list<BaseCollider*>::iterator it;

    it = colliders.begin();
    for (; it != colliders.end(); ++it) {
        BaseCollider* col = *it;

        if (!(col->attribute & attribute)) continue;

        // 球
        if (col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            Sphere* sphereB = dynamic_cast<Sphere*>(col);

            if (!Collision::CheckSphere2Sphere(sphere, *sphereB)) continue;

            return false;
        }
        // メッシュ
        else if (col->GetShapeType() == COLLISIONSHAPE_MESH) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

            if (!meshCollider->CheckCollisionSphere(sphere)) continue;

            return true;
        }
        else if (col->GetShapeType() == COLLISIONSHAPE_PLANE) {
            Plane* planeB = dynamic_cast<Plane*>(col);

            if (!Collision::CheckSphere2Plane(sphere, *planeB)) continue;

            return true;
        }
        else if (col->GetShapeType() == COLLISIONSHAPE_TRIANGLE) {
            Triangle* triangle = dynamic_cast<Triangle*>(col);

            if (!Collision::CheckSphere2Triangle(sphere, *triangle)) continue;

            return true;
        }
    }

    return false;
}
