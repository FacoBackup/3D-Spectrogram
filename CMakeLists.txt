cmake_minimum_required(VERSION 3.30)
project(tcc_iesb CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

add_executable(
        tcc_iesb
        src/main.cpp
        src/context/editor/EditorPanel.cpp
        src/context/editor/EditorPanel.h
        src/context/editor/abstract/IPanel.h
        src/context/engine/EngineContext.cpp
        src/context/engine/EngineContext.h
        src/util/VulkanUtils.cpp
        src/util/VulkanUtils.h
        src/context/vulkan/VulkanContext.cpp
        src/context/vulkan/VulkanContext.h
        src/context/glfw/GLFWContext.cpp
        src/context/glfw/GLFWContext.h
        src/context/gui/GuiContext.cpp
        src/context/gui/GuiContext.h
        src/context/editor/dock-spaces/viewport/ViewportPanel.cpp
        src/context/editor/dock-spaces/viewport/ViewportPanel.h
        src/context/ApplicationContext.cpp
        src/context/ApplicationContext.h
        src/context/editor/abstract/AbstractPanel.h
        src/context/editor/abstract/AbstractPanel.cpp
        src/common/interface/Synchornizable.h
        src/service/camera/CameraService.cpp
        src/service/camera/CameraService.h
        src/service/camera/Camera.h
        src/util/Util.h
        src/common/inspection/Inspectable.h
        src/common/inspection/Inspectable.cpp
        src/common/inspection/InspectedField.h
        src/context/editor/dock-spaces/inspector/InspectorPanel.cpp
        src/context/editor/dock-spaces/inspector/InspectorPanel.h
        src/context/editor/abstract/form/FormPanel.cpp
        src/context/editor/abstract/form/FormPanel.h
        src/context/editor/abstract/form/AccordionPanel.cpp
        src/context/editor/abstract/form/AccordionPanel.h
        src/common/inspection/InspectableMember.h
        src/common/AbstractRuntimeComponent.h
        src/context/editor/abstract/form/types/BooleanField.cpp
        src/context/editor/abstract/form/types/BooleanField.h
        src/context/editor/abstract/form/types/IntField.cpp
        src/context/editor/abstract/form/types/IntField.h
        src/context/editor/abstract/form/types/FloatField.cpp
        src/context/editor/abstract/form/types/FloatField.h
        src/context/editor/abstract/form/types/MethodField.cpp
        src/context/editor/abstract/form/types/MethodField.h
        src/util/UIUtil.h
        src/common/interface/Icons.h
        src/service/theme/ThemeService.cpp
        src/service/theme/ThemeService.h
        src/repository/editor/EditorRepository.h
        src/enum/engine-definitions.h
        src/repository/abstract/RuntimeResource.h
        src/util/ShaderUtil.cpp
        src/util/ShaderUtil.h
        src/service/pipeline/ShaderModule.cpp
        src/service/pipeline/ShaderModule.h
        src/context/engine/render-pass/AbstractRenderPass.cpp
        src/context/engine/render-pass/AbstractRenderPass.h
        src/repository/abstract/ResourceRepository.h
        src/service/abstract/AbstractResourceService.cpp
        src/service/abstract/AbstractResourceService.h
        src/service/framebuffer/FrameBufferAttachment.h
        src/repository/framebuffer/CoreFrameBuffers.cpp
        src/repository/framebuffer/CoreFrameBuffers.h
        src/repository/abstract/AbstractCoreRepository.h
        src/repository/abstract/AbstractCoreRepository.cpp
        src/service/framebuffer/FrameBufferService.cpp
        src/service/framebuffer/FrameBufferService.h
        src/service/framebuffer/FrameBufferInstance.h
        src/service/pipeline/PipelineService.cpp
        src/service/pipeline/PipelineService.h
        src/service/pipeline/PipelineInstance.h
        src/repository/buffers/CoreBuffers.cpp
        src/repository/buffers/CoreBuffers.h
        src/service/buffer/BufferService.cpp
        src/service/buffer/BufferService.h
        src/service/descriptor/DescriptorService.cpp
        src/service/descriptor/DescriptorService.h
        src/service/descriptor/DescriptorInstance.cpp
        src/service/descriptor/DescriptorInstance.h
        src/dto/ubo/GlobalDataUBO.h
        src/repository/runtime/RuntimeRepository.h
        src/repository/descriptors/CoreDescriptorSets.cpp
        src/repository/descriptors/CoreDescriptorSets.h
        src/service/buffer/BufferInstance.cpp
        src/service/buffer/BufferInstance.h
        src/service/pipeline/PipelineInstance.cpp
        src/util/FilesUtil.h
        src/util/FileDialogUtil.h
        src/context/engine/passes/CommandBufferRecorder.cpp
        src/context/engine/passes/CommandBufferRecorder.h
        src/context/editor/abstract/form/types/ColorField.cpp
        src/context/editor/abstract/form/types/ColorField.h
        src/context/editor/abstract/form/types/Vec4Field.cpp
        src/context/editor/abstract/form/types/Vec4Field.h
        src/context/editor/abstract/form/types/Vec3Field.cpp
        src/context/editor/abstract/form/types/Vec3Field.h
        src/context/editor/abstract/form/types/Vec2Field.cpp
        src/context/editor/abstract/form/types/Vec2Field.h
        src/context/editor/abstract/form/types/StringField.cpp
        src/context/editor/abstract/form/types/StringField.h
        src/enum/ResourceType.h
        src/context/editor/abstract/form/types/QuatField.cpp
        src/context/editor/abstract/form/types/QuatField.h
        src/service/voxel/impl/SparseVoxelOctreeBuilder.cpp
        src/service/voxel/impl/SparseVoxelOctreeBuilder.h
        src/service/voxel/impl/OctreeNode.cpp
        src/service/voxel/impl/OctreeNode.h
        src/service/voxel/impl/VoxelData.h
        src/service/pipeline/PipelineBuilder.h
        src/service/framebuffer/FrameBufferAttachment.cpp
        src/util/serialization-definitions.h
        src/service/voxel/impl/SparseVoxelOctreeData.h
        src/context/engine/render-pass/impl/tools/VisualizerPass.cpp
        src/context/engine/render-pass/impl/tools/VisualizerPass.h
        src/dto/push-constant/VoxelDebugSettingsPushConstant.h
        src/repository/editor/EditorRepository.cpp
        src/context/engine/passes/AbstractPass.cpp
        src/context/engine/passes/AbstractPass.h
        src/context/engine/passes/PassesService.cpp
        src/context/engine/passes/PassesService.h
        src/util/ImageUtils.h
        src/context/editor/dock-spaces/inspector/FilePickerUtil.h
        src/service/audio/AudioData.h
        src/service/audio/AudioProcessorService.h
        src/context/editor/dock-spaces/inspector/components/TimestampPickerPanel.cpp
        src/context/editor/dock-spaces/inspector/components/TimestampPickerPanel.h
        src/service/audio/AudioProcessorService.cpp
        src/context/editor/NotificationPanel.cpp
        src/context/editor/NotificationPanel.h
        src/repository/notification/NotificationRepository.h
        src/repository/notification/Notification.h
        src/repository/notification/NotificationRepository.cpp
        src/definitions.h
)


# ------ LIBS
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Vulkan)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(glslang CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(meshoptimizer CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)

set(COMMON_LIBRARIES
        SndFile::sndfile
        glfw
        nfd::nfd
        cereal::cereal
        meshoptimizer::meshoptimizer
        assimp::assimp
        Vulkan::Headers
        vk-bootstrap
        vk-bootstrap-compiler-warnings
        GPUOpen::VulkanMemoryAllocator
        glm::glm
        imgui::imgui
        imguizmo::imguizmo
        spdlog::spdlog
        nlohmann_json::nlohmann_json

        glslang::glslang
        glslang::glslang-default-resource-limits
        glslang::SPVRemapper
)
target_link_libraries(tcc_iesb PRIVATE ${COMMON_LIBRARIES})
target_include_directories(tcc_iesb PRIVATE dependencies/stb)
add_subdirectory(dependencies/vk-bootstrap)
# ------ LIBS

# ------ STATIC RESOURCES
file(COPY "${CMAKE_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}/")
# ------ STATIC RESOURCES

# ------ VCPKG
set(CMAKE_TOOLCHAIN_FILE "/Users/gustavoroque/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_JSON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json")

# Include vcpkg toolchain
if (EXISTS "${VCPKG_JSON_PATH}")
    set(VCPKG_TOOLCHAIN "${CMAKE_TOOLCHAIN_FILE}")
    if (EXISTS "${VCPKG_TOOLCHAIN}")
        message(STATUS "Using Vcpkg toolchain file: ${VCPKG_TOOLCHAIN}")
        include("${VCPKG_TOOLCHAIN}")
    else ()
        message(FATAL_ERROR "Vcpkg toolchain file not found. Please install Vcpkg and create a vcpkg.json file for your project.")
    endif ()
else ()
    message(FATAL_ERROR "Vcpkg JSON file not found. Please create a vcpkg.json file for your project.")
endif ()
# ------ VCPKG