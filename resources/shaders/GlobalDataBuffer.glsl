layout(set = 0, binding = 0) uniform GlobalDataBlock {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 invView;
    mat4 invProj;
    vec3 cameraWorldPosition;
    bool isOrtho;
    ivec3 axisLengths;
    bool isStaticCurve;
} globalData;

float rand(vec3 co) {
    return fract(sin(dot(co, vec3(12.9898, 71.9898, 78.233))) * 43758.5453);
}
