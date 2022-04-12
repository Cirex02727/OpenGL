#pragma once

class BlockFace
{
public:
    /* Back Face -Z */
    static unsigned char* BACK;

    /* Front Face +Z */
    static unsigned char* FRONT;

    /* Right Face -X */
    static unsigned char* RIGHT;

    /* Left Face +X */
    static unsigned char* LEFT;

    /* Bottom Face -Y */
    static unsigned char* BOTTOM;

    /* Top Face +Y */
    static unsigned char* TOP;

    static unsigned char* INDICES;
};