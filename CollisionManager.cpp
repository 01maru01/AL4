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
                //  ‹…“¯Žm‚Ì“–‚½‚è”»’è
                Sphere* sphereA = dynamic_cast<Sphere*>(colA);
                Sphere* sphereB = dynamic_cast<Sphere*>(colB);
                Vector3D inter;
                if (Collision::CheclSphere2Sphere(*sphereA, *sphereB, &inter)) {
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
