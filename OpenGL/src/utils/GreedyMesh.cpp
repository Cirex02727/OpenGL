#include "GreedyMesh.h"

#include "utils/Maths.h"
#include "chunks/Chunk.h"
#include "chunks/Block.h"
#include "chunks/Blocks.h"

#define MERGE_INDEX(x, y, w) x + (y * w)

void GreedyMesh::Calculate(std::unordered_map<unsigned int, std::size_t>* memoryMap, Chunk* chunk)
{
    bool* merged{};

    unsigned short *startPos, *currPos, *quadSize, *m, *n, *offsetPos;
    
    Block* startBlock;
    unsigned char direction, workAxis1, workAxis2;

    unsigned short* Dimensions = new unsigned short[3] { Chunk::c_ChunkSize, Chunk::c_ChunkHeight, Chunk::c_ChunkSize };

    for (unsigned char face = 0; face < 6; face++) {
        bool isBackFace = face > 2;
        direction = face % 3;
        workAxis1 = (direction + 1) % 3;
        workAxis2 = (direction + 2) % 3;

        startPos = new unsigned short[3];
        currPos = new unsigned short[3];

        // Iterate over the chunk layer by layer.
        for (startPos[direction] = 0; startPos[direction] < Dimensions[direction]; startPos[direction]++) {
            memset(merged, false, sizeof(bool) * Dimensions[workAxis1] * Dimensions[workAxis2]);

            // Build the slices of the mesh.
            for (startPos[workAxis1] = 0; startPos[workAxis1] < Dimensions[workAxis1]; startPos[workAxis1]++) {
                for (startPos[workAxis2] = 0; startPos[workAxis2] < Dimensions[workAxis2]; startPos[workAxis2]++) {
                    startBlock = Blocks::GetBlock(*chunk->GetBlockType(startPos[0], startPos[1], startPos[2]));

                    // If this block has already been merged, is air, or not visible skip it.
                    bool hasMerged = merged[ MERGE_INDEX(startPos[workAxis1], startPos[workAxis2], Dimensions[workAxis1]) ];
                    if (startBlock == nullptr || hasMerged ||
                        (startBlock != nullptr && startBlock->IsLightPass()) ||
                        !IsBlockFaceVisible(chunk, startPos, direction, isBackFace)) {
                        continue;
                    }

                    // Reset the work var
                    quadSize = new unsigned short[3];

                    // Figure out the width, then save it
                    for (currPos = startPos, currPos[workAxis2]++; currPos[workAxis2] < Dimensions[workAxis2] && CompareStep(chunk, startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1], currPos[workAxis2]]; currPos[workAxis2]++) {}
                    quadSize[workAxis2] = currPos[workAxis2] - startPos[workAxis2];

                    // Figure out the height, then save it
                    for (currPos = startPos, currPos[workAxis1]++; currPos[workAxis1] < Dimensions[workAxis1] && CompareStep(chunk, startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1], currPos[workAxis2]]; currPos[workAxis1]++) {
                        for (currPos[workAxis2] = startPos[workAxis2]; currPos[workAxis2] < Dimensions[workAxis2] && CompareStep(chunk, startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1], currPos[workAxis2]]; currPos[workAxis2]++) {}

                        // If we didn't reach the end then its not a good add.
                        if (currPos[workAxis2] - startPos[workAxis2] < quadSize[workAxis2]) {
                            break;
                        }
                        else {
                            currPos[workAxis2] = startPos[workAxis2];
                        }
                    }
                    quadSize[workAxis1] = currPos[workAxis1] - startPos[workAxis1];

                    // Now we add the quad to the mesh
                    m = new unsigned short[3];
                    m[workAxis1] = quadSize[workAxis1];

                    n = new unsigned short[3];
                    n[workAxis2] = quadSize[workAxis2];

                    // We need to add a slight offset when working with front faces.
                    offsetPos = startPos;
                    offsetPos[direction] += isBackFace ? 0 : 1;

                    //Draw the face to the mesh
                    AddFace(
                        memoryMap,
                        offsetPos,
                        Maths::AddVectors(3, offsetPos, m),
                        Maths::AddVectors(3, offsetPos, m, n),
                        Maths::AddVectors(3, offsetPos, n),
                        startBlock, isBackFace);

                    // Mark it merged
                    for (int f = 0; f < quadSize[workAxis1]; f++) {
                        for (int g = 0; g < quadSize[workAxis2]; g++) {
                            merged[startPos[workAxis1] + f, startPos[workAxis2] + g] = true;
                        }
                    }
                }
            }
        }
    }
}

bool GreedyMesh::IsBlockFaceVisible(Chunk* chunk, unsigned short* blockPosition, int axis, bool backFace)
{
    blockPosition[axis] += backFace ? -1 : 1;
    return chunk->GetBlock(blockPosition[0], blockPosition[1], blockPosition[2])->IsLightPass();
}

bool GreedyMesh::CompareStep(Chunk* chunk, unsigned short* a, unsigned short* b, int direction, bool backFace)
{
    Block* blockA = chunk->GetBlock(a[0], a[1], a[2]);
    Block* blockB = chunk->GetBlock(b[0], b[1], b[2]);

    return blockA == blockB && !blockB->IsLightPass() && IsBlockFaceVisible(chunk, b, direction, backFace);
}

void GreedyMesh::AddFace(std::unordered_map<unsigned int, std::size_t>* memoryMap, unsigned short* v1, unsigned short* v2, unsigned short* v3, unsigned short* v4, Block* block, bool backFace)
{

}
