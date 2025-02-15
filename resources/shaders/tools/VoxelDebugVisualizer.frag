#include "../GlobalDataBuffer.glsl"
#include "../CreateRay.glsl"
#define DEBUG_VOXELS
#include "../util/RayTracer.glsl"
#include "../DebugFlags.glsl"

layout(location = 0) in vec2 texCoords;
layout(location = 0) out vec4 finalColor;

layout(push_constant) uniform Push {
    int voxelDebugFlag;
    bool showRaySearchCount;
    bool showRayTestCount;
    uint searchCountDivisor;
} settings;

vec3 randomColor(float seed) {
    float r = rand(vec3(seed));
    float g = rand(vec3(seed + r));
    return vec3(r, g, rand(vec3(seed + g)));
}

void main() {
    vec3 rayOrigin = globalData.cameraWorldPosition.xyz;
    vec3 rayDirection = createRay(texCoords, globalData.invProj, globalData.invView);
    ivec2 colorData = ivec2(0);
    Ray ray = Ray(rayOrigin, rayDirection, 1./rayDirection);
    SurfaceInteraction hitData = traceAllTiles(ray, settings.showRaySearchCount, settings.showRayTestCount, colorData);
    if (hitData.anyHit){
        MaterialInfo matData;
        unpackVoxel(hitData, matData);
        finalColor = vec4(randomColor(rand(hitData.voxelPosition.xyz)), 1);
    }else{
        discard;
    }

    if (length(finalColor.rgb) == 0){
        finalColor.rg = colorData/float(settings.searchCountDivisor);
        finalColor.a = 1;
    }
}