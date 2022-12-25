#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"

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
        }
    }
}
