#include "BlockFace.h"

/* Back Face -Z */
unsigned char* BlockFace::BACK = new unsigned char[] {
    0, 1, 0,  // 0
    0, 0, 0,  // 1
    1, 1, 0,  // 2
    0, 0, 0,  // 1
    1, 0, 0,  // 3
    1, 1, 0,  // 2
};

unsigned char* BlockFace::FRONT = new unsigned char[] {
    1, 1, 1,  // 0
    1, 0, 1,  // 1
    0, 1, 1,  // 2
    1, 0, 1,  // 1
    0, 0, 1,  // 3
    0, 1, 1,  // 2
};

/* Right Face -X */
unsigned char* BlockFace::LEFT = new unsigned char[] {
    0, 1, 1,  // 0
    0, 0, 1,  // 1
    0, 1, 0,  // 2
    0, 0, 1,  // 1
    0, 0, 0,  // 3
    0, 1, 0,  // 2
};

/* Left Face +X */
unsigned char* BlockFace::RIGHT = new unsigned char[] {
    1, 1, 0,  // 0
    1, 0, 0,  // 1
    1, 1, 1,  // 2
    1, 0, 0,  // 1
    1, 0, 1,  // 3
    1, 1, 1,  // 2
};

/* Bottom Face -Y */
unsigned char* BlockFace::BOTTOM = new unsigned char[] {
    1, 0, 1,  // 0
    1, 0, 0,  // 1
    0, 0, 1,  // 2
    1, 0, 0,  // 1
    0, 0, 0,  // 3
    0, 0, 1,  // 2
};

/* Top Face +Y */
unsigned char* BlockFace::TOP = new unsigned char[] {
    0, 1, 1,  // 0
    0, 1, 0,  // 1
    1, 1, 1,  // 2
    0, 1, 0,  // 1
    1, 1, 0,  // 3
    1, 1, 1,  // 2
};

unsigned char* BlockFace::INDICES = new unsigned char[] {
    0, 1, 2,
    1, 3, 2,
};

unsigned char* BlockFace::UVS = new unsigned char[] {
    0, 0,  // 0
    0, 1,  // 1
    1, 0,  // 2
    0, 1,  // 1
    1, 1,  // 3
    1, 0,  // 2
};
