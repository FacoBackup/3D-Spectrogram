struct SurfaceInteraction {
    vec3 incomingRayDir;
    vec3 point;
    vec3 voxelPosition;
    float voxelSize;
    uint voxel;
};

#define MAX_VOXEL_SIZE 10000000

layout(std430, set = 1, binding = 0) readonly buffer VoxelBuffer {
    uint voxels[MAX_VOXEL_SIZE];
} voxelBuffer;


struct Ray { vec3 o, d, invDir; };
struct Stack { uint index;vec3 center;float scale; uint currentDepth; };

bool intersect(inout vec3 boxMin, inout vec3 boxMax, inout Ray r) {
    vec3 t1 = (boxMin - r.o) * r.invDir;
    vec3 t2 = (boxMax - r.o) * r.invDir;

    vec3 tMin = min(t1, t2);
    vec3 tMax = max(t1, t2);

    float tEnter = max(max(tMin.x, tMin.y), tMin.z);
    float tExit = min(min(tMax.x, tMax.y), tMax.z);

    return tEnter <= tExit && tExit > 0.0;
}

bool intersectWithDistance(inout vec3 boxMin, inout vec3 boxMax, inout Ray r, out float entryDist) {
    vec3 t1 = (boxMin - r.o) * r.invDir;
    vec3 t2 = (boxMax - r.o) * r.invDir;

    vec3 tMin = min(t1, t2);
    vec3 tMax = max(t1, t2);

    entryDist = max(max(tMin.x, tMin.y), tMin.z);// Closest entry point along the ray
    float exitDist = min(min(tMax.x, tMax.y), tMax.z);// Furthest exit point along the ray

    return entryDist <= exitDist && exitDist > 0.0;// Ensure valid intersection and that exit is in front of the ray origin
}

uint countSetBitsBefore(inout uint mask, inout uint childIndex) {
    uint maskBefore = mask & ((1u << childIndex) - 1u);
    return bitCount(maskBefore);
}

vec4 intersectRayAABB(in Ray ray, vec3 boxMin, vec3 boxMax) {
    vec3 tMin = (boxMin - ray.o) * ray.invDir;
    vec3 tMax = (boxMax - ray.o) * ray.invDir;

    // Swap tMin and tMax for negative directions
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    // Find the largest t1 and the smallest t2
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);

    // Check if the ray intersects the AABB
    if (tNear > tFar || tFar < 0.0) {
        return vec4(0);// No intersection
    }

    // Compute intersection point
    float t = tNear > 0.0 ? tNear : tFar;// Use tNear if it's in front
    return vec4(ray.o + t * ray.d, 1);
}

uint getLevelOfDetail(float distanceFromRayOrigin){
    if (distanceFromRayOrigin < 3){
        return 12;
    }
    if (distanceFromRayOrigin < 5){
        return 11;
    }
    if (distanceFromRayOrigin < 10){
        return 10;
    }
    return 9;
}

#define CHILD_MASK(V) (V >> 1) & 0xFFu
#define CHILD_GROUP_INDEX(V) (V >> 9) & 0x7FFFFFu

SurfaceInteraction trace(
    in Ray ray,
    bool showRaySearchCount,
    bool showRayTestCount,
    inout ivec2 debugColor
) {
    SurfaceInteraction hitData;

    vec3 center = vec3(0);
    float scale = TILE_SIZE / 2;
    vec3 minBox = center - scale;
    vec3 maxBox = center + scale;
    float minDistance = 1e10;// Large initial value

    if (!intersect(minBox, maxBox, ray)) return hitData;
    Stack stack[10];
    scale *= 0.5f;
    stack[0] = Stack(0u, center, scale, 1);

    uint index = 0u;
    int stackPos = 1;
    uint hitIndex;
    uint currentDepth = 1;
    while (stackPos-- > 0) {
        if (showRaySearchCount){
            debugColor.r++;
        }
        center = stack[stackPos].center;
        index = stack[stackPos].index;
        scale = stack[stackPos].scale;
        currentDepth = stack[stackPos].currentDepth;
        uint voxel_node = voxelBuffer.voxels[index];

        bool isLeafGroup = ((voxel_node & 0x1u) == 1u);
        uint childMask = CHILD_MASK(voxel_node);
        uint childGroupIndex = CHILD_GROUP_INDEX(voxel_node);

        for (uint i = 0u; i < 8u; ++i) {
            if ((childMask & (1u << i)) == 0u){
                continue;
            }
            vec3 offset = vec3(
            (i & 1u) != 0u ? 1.0 : -1.0,
            (i & 2u) != 0u ? 1.0 : -1.0,
            (i & 4u) != 0u ? 1.0 : -1.0
            );
            vec3 newCenter = center + scale * offset;
            vec3 minBox = newCenter - scale;
            vec3 maxBox = newCenter + scale;

            if (showRayTestCount){
                debugColor.g++;
            }

            float entryDist;
            if (!intersectWithDistance(minBox, maxBox, ray, entryDist)) {
                continue;
            }
            if (entryDist < minDistance) {
                if (isLeafGroup) {
                    hitData.voxel = voxel_node;
                    hitData.voxelPosition = newCenter;
                    hitData.voxelSize = scale;
                    hitIndex = childGroupIndex;

                    index++;
                    minDistance = entryDist;
                } else {
                    stack[stackPos++] = Stack(childGroupIndex + countSetBitsBefore(childMask, i), newCenter, scale * 0.5f, currentDepth + 1);
                }
            }
        }
    }

    return hitData;
}