#include "../GlobalDataBuffer.glsl"
#include "../CreateRay.glsl"
#include "../Dithering.glsl"

#define SCALE 1.f
#define THRESHOLD 100
#define THICKNESS .05f
#define N 20.f

layout(location = 0) in vec2 texCoords;
layout(location = 0) out vec4 finalColor;

bool rayMarch(vec3 ro, vec3 rd, out vec3 hitPos) {
    float t = 0.0;
    hitPos = ro;

    for (int i = 0; i < 256; i++) {
        vec3 currentP = ro + t * rd;
        float distToPlanes = min(min(abs(currentP.x), abs(currentP.y)), abs(currentP.z));

        if (distToPlanes < 0.001) {
            hitPos = currentP;
            return true;
        }

        if (t > THRESHOLD) {
            break;
        }

        t += max(distToPlanes, 0.001);
    }

    hitPos = vec3(0.0);
    return false;
}

float getGridLine3D(vec3 pos, float gridScale){
    float scale = gridScale * SCALE;

    float dist_x_plane = min(abs(fract(pos.x / scale) * scale), abs(fract(-pos.x / scale) * scale));
    float dist_y_plane = min(abs(fract(pos.y / scale) * scale), abs(fract(-pos.y / scale) * scale));
    float dist_z_plane = min(abs(fract(pos.z / scale) * scale), abs(fract(-pos.z / scale) * scale));

    float onLineZ = (1.0 - step(THICKNESS, dist_x_plane)) * (1.0 - step(THICKNESS, dist_y_plane));
    float onLineY = (1.0 - step(THICKNESS, dist_x_plane)) * (1.0 - step(THICKNESS, dist_z_plane));
    float onLineX = (1.0 - step(THICKNESS, dist_y_plane)) * (1.0 - step(THICKNESS, dist_z_plane));

    float onAnyLine = max(max(onLineX, onLineY), onLineZ);
    return 1.0 - onAnyLine;
}

void main() {
    bool hasData = false;
    vec3 hitPoint;
    vec3 rayDir = createRay(texCoords, globalData.invProj, globalData.invView);
    hasData = rayMarch(globalData.cameraWorldPosition.xyz, rayDir, hitPoint);

    if (hasData){
        float distanceFromCamera = length(globalData.cameraWorldPosition.xyz - hitPoint);
        float alpha = 1;
        if (distanceFromCamera >= THRESHOLD){
            alpha = 0;
        } else {
            float fadeRange = THRESHOLD * 0.1;
            alpha = smoothstep(THRESHOLD, THRESHOLD - fadeRange, distanceFromCamera);
        }

        float inBounds = step(abs(hitPoint.x), N) * step(abs(hitPoint.y), N) * step(abs(hitPoint.z), N);
        alpha *= inBounds;

        if (alpha > 0){
            vec3 baseColor = vec3(.9);
            vec3 xAxisColor = vec3(1.0, 0.0, 0.0);
            vec3 yAxisColor = vec3(0.0, 1.0, 0.0);
            vec3 zAxisColor = vec3(0.0, 0.0, 1.0);

            float scaleEffect = SCALE;
            float thicknessCheck = THICKNESS / max(scaleEffect, 0.01);

            float isXAxis = step(max(abs(hitPoint.y / scaleEffect), abs(hitPoint.z / scaleEffect)), thicknessCheck);
            float isYAxis = step(max(abs(hitPoint.x / scaleEffect), abs(hitPoint.z / scaleEffect)), thicknessCheck);
            float isZAxis = step(max(abs(hitPoint.x / scaleEffect), abs(hitPoint.y / scaleEffect)), thicknessCheck);

            vec3 axisColorSum = xAxisColor * isXAxis + yAxisColor * isYAxis + zAxisColor * isZAxis;
            axisColorSum = clamp(axisColorSum, 0.0, 1.0);

            float onAnyAxis = max(max(isXAxis, isYAxis), isZAxis);
            vec4 centerLineColor = vec4(axisColorSum, 1.0) * onAnyAxis;

            vec4 gridColor = mix(vec4(baseColor, 1.0), vec4(0), getGridLine3D(hitPoint, 1));
            gridColor = mix(vec4(baseColor * .75, 1.0), gridColor, getGridLine3D(hitPoint, 5));

            finalColor = mix(gridColor, centerLineColor, onAnyAxis);

        } else {
            finalColor = vec4(0.0);
        }

        finalColor.a = min(alpha, finalColor.a);

        if (finalColor.a == 0){
            discard;
        }
        if (isDitherDiscard(finalColor.a)) {
            discard;
        }

    } else {
        discard;
    }
}
