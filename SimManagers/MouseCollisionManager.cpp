//
// Created by Conner on 2/28/2023.
//

#include "MouseCollisionManager.h"

#define MouseBufferSize 2

IClickable *MouseCollisionManager::CheckMouseAABBCollision(int mouseX, int mouseY) {
    for(auto& clickable : _activeClickables){
        Vector2 extA = {MouseBufferSize,MouseBufferSize};
        // Set mouse position to top left for AABB collision checking
        Vector2 posA = {mouseX - extA.x/2, mouseY - extA.y/2};

        Vector2 posB = clickable->AABBPosition();
        Vector2 extB = clickable->AABBExtends();

        if( posA.x < posB.x + extB.x &&
            posA.x + extA.x > posB.x &&
            posA.y < posB.y + extB.y &&
            posA.y + extA.y > posB.y) {
            return clickable;
        }
    }
    return nullptr;
}
