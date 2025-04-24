#ifndef VOXELDATA_H
#define VOXELDATA_H
#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Metal {
    struct VoxelData {
        unsigned int data{};

        explicit VoxelData(const glm::ivec3 &albedo) {
            unsigned int red = (albedo.r / 2u) & 0x7Fu;
            unsigned int green = albedo.g & 0xFFu;
            unsigned int blue = (albedo.b / 2u) & 0x7Fu;

            data = (red << 16) | (green << 8) | blue;
        }

    private:
        // THANKS TO https://www.shadertoy.com/view/llfcRl
        static unsigned int CompressNormal(const glm::vec3 &nor) {
            glm::vec3 mor = nor;
            uint32_t id = 0;

            if (std::abs(nor.y) > std::abs(mor.x)) {
                mor = glm::vec3(nor.y, nor.z, nor.x);
                id = 1u;
            }
            if (std::abs(nor.z) > std::abs(mor.x)) {
                mor = glm::vec3(nor.z, nor.x, nor.y);
                id = 2u;
            }

            uint32_t is = (mor.x < 0.0f) ? 1u : 0u;
            glm::vec2 uv = 0.5f + 0.5f * glm::vec2(mor.y, mor.z) / std::abs(mor.x);
            glm::uvec2 iuv = glm::uvec2(glm::round(uv * glm::vec2(32767.0f, 16383.0f)));

            return iuv.x | (iuv.y << 15u) | (id << 29u) | (is << 31u);
        }
    };
}

#endif //VOXELDATA_H
