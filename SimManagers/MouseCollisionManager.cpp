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
            // Trigger hover events
            if(_lastClickable != clickable){
                if(_lastClickable)
                    _lastClickable->HoverExit();
                _lastClickable = clickable;
                clickable->HoverEnter();
            }
            return clickable;
        }
    }
    // Trigger hover events
    if(_lastClickable){
        _lastClickable->HoverExit();
        _lastClickable = nullptr;
    }
    return nullptr;
}

Wire* MouseCollisionManager::CheckMouseWireCollision(int mouseX, int mouseY, const std::vector<std::unique_ptr<Wire>>& wires) {
    for(auto& wire : wires){
        float lineLength = Vector2::Distance(wire->NodeA().Position(), wire->NodeB().Position());
        float d1 = Vector2::Distance({mouseX, mouseY}, wire->NodeA().Position());
        float d2 = Vector2::Distance({mouseX, mouseY}, wire->NodeB().Position());

        if(d1+d2 >= lineLength-MouseBufferSize && d1+d2 <= lineLength+MouseBufferSize){
            return wire.get();
        }
    }

    return nullptr;
}
