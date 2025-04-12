#include "CoreBuffers.h"

#include "../../dto/ubo/GlobalDataUBO.h"
#include "../../service/buffer/BufferService.h"

namespace Metal {
    void CoreBuffers::onInitialize() {
        globalData = bufferService.createBuffer(sizeof(GlobalDataUBO),
                                                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        svoData = bufferService.createBuffer(sizeof(unsigned int) * 1000000,
                                                VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
}
