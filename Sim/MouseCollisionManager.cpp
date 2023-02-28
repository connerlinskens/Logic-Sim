//
// Created by Conner on 2/28/2023.
//

#include "MouseCollisionManager.h"

IClickable *MouseCollisionManager::CheckMouseCollision(int mouseX, int mouseY) {
    for(auto& clickable : _currentClickables){
        Vector2 extA = {2,2};
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
