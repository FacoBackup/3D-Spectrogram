#ifndef BACKGROUNDPUSHCONSTANT_H
#define BACKGROUNDPUSHCONSTANT_H
#include <glm/glm.hpp>

namespace Metal {
    struct BackgroundPushConstant final {
        alignas(16) glm::vec3 color;
    };
}

#endif //BACKGROUNDPUSHCONSTANT_H
