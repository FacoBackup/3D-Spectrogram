#ifndef SPARSEVOXELOCTREE_H
#define SPARSEVOXELOCTREE_H
#include <vector>

#include "OctreeNode.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace Metal {
    class SparseVoxelOctreeBuilder {
        OctreeNode root{};
        unsigned int nodeQuantity = 1;
        unsigned int leafVoxelQuantity = 0;
        float voxelSize = 1.0f;
        unsigned int size{};
        int maxDepth = 0;
        std::vector<uint32_t> voxels;
        unsigned int bufferIndex = 0;

        void insertInternal(OctreeNode *node, glm::vec3 &point,
                            glm::ivec3 &position, int depth);

        void fillStorage(OctreeNode *node);

        void putData(OctreeNode *node);

        void worldToChunkLocal(glm::vec3 &worldCoordinate) const;

    public:
        std::unordered_map<std::string, OctreeNode *> repeatedStructures;

        SparseVoxelOctreeBuilder(unsigned int size, int maxDepth);

        [[nodiscard]] unsigned int getVoxelQuantity() const {
            return nodeQuantity;
        }

        [[nodiscard]] unsigned int getLeafVoxelQuantity() const {
            return leafVoxelQuantity;
        }

        [[nodiscard]] float getVoxelSize() const {
            return voxelSize;
        }

        [[nodiscard]] const std::vector<uint32_t> &buildBuffer();

        OctreeNode &getRoot() {
            return root;
        }

        void insert(glm::vec3 point);

        void dispose();
    };
} // Metal

#endif //SPARSEVOXELOCTREE_H
