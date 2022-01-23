#include "Move.h"

void move::rotate(Triangle2D& tri, const PositionVertex2D& pointOfRotation, const float deg) 
{
    // TODO: implement pointOfRotation
    for (size_t index = 0; index < 3; index++) 
    {
        tri.verts[index].posX *= cos(deg);
        tri.verts[index].posY *= sin(deg);   
    }
}