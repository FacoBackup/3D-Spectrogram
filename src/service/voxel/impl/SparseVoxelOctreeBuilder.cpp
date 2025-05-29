#include "SparseVoxelOctreeBuilder.h"

namespace Metal {
    SparseVoxelOctreeBuilder::SparseVoxelOctreeBuilder(unsigned int size, int maxDepth)
        : size(size), maxDepth(maxDepth) {
        voxelSize = static_cast<float>(size) / std::pow(2.0f, maxDepth);
    }

    void SparseVoxelOctreeBuilder::insert(glm::vec3 point) {
        worldToChunkLocal(point);
        glm::ivec3 pos(0);
        insertInternal(&root, point, pos, 0);
    }

    void SparseVoxelOctreeBuilder::insertInternal(OctreeNode *node, glm::vec3 &point,
                                                  glm::ivec3 &position, int depth) {
        node->depth = depth;

        if (depth == maxDepth) {
            node->isLeaf = true;
            ++leafVoxelQuantity;
            return;
        }

        const auto size = static_cast<float>(this->size / std::pow(2, depth));
        const auto childPos = glm::ivec3{
            point.x >= ((size * position.x) + (size / 2)) ? 1 : 0,
            point.y >= ((size * position.y) + (size / 2)) ? 1 : 0,
            point.z >= ((size * position.z) + (size / 2)) ? 1 : 0
        };
        const int childIndex = (childPos.x << 0) | (childPos.y << 1) | (childPos.z << 2);

        position.x = (position.x << 1) | childPos.x;
        position.y = (position.y << 1) | childPos.y;
        position.z = (position.z << 1) | childPos.z;

        if (node->children[childIndex] != nullptr) {
            insertInternal(node->children[childIndex], point, position, depth + 1);
        } else {
            auto *child = new OctreeNode;
            node->addChild(child, childIndex);
            insertInternal(child, point, position, depth + 1);

            if (child->depth == maxDepth - 1) {
                leafVoxelQuantity++;
            }
            if (!child->isLeaf) {
                nodeQuantity++;
            }
        }
    }

    void SparseVoxelOctreeBuilder::worldToChunkLocal(glm::vec3 &worldCoordinate) const {
        const glm::vec3 min = -glm::vec3(size) * 0.5f;
        worldCoordinate -= min;
    }

    const std::vector<uint32_t> &SparseVoxelOctreeBuilder::buildBuffer() {
        bufferIndex = 0;
        voxels.resize(nodeQuantity);
        putData(&root);
        fillStorage(&root);
        return voxels;
    }

    void SparseVoxelOctreeBuilder::putData(OctreeNode *node) {
        node->dataIndex = bufferIndex++;
    }

    void SparseVoxelOctreeBuilder::fillStorage(OctreeNode *node) {
        if (node->isLeaf) return;

        voxels[node->dataIndex] = node->packVoxelData(bufferIndex);
        bool isParentOfLeaf = true;

        for (OctreeNode *child: node->children) {
            if (child && !child->isLeaf) {
                putData(child);
                isParentOfLeaf = false;
            }
        }

        for (OctreeNode *child: node->children) {
            if (child) {
                fillStorage(child);
            }
        }

        if (isParentOfLeaf) {
            voxels[node->dataIndex] = node->packVoxelData(0);
        }
    }

    void SparseVoxelOctreeBuilder::dispose() {
        root.dispose();
        voxels.clear();
    }
} // Metal
