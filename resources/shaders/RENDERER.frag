#include "../GlobalDataBuffer.glsl"
#include "../CreateRay.glsl"
#include "../util/RayTracer.glsl"
#include "../Dithering.glsl"

#define SCALE 1.f
#define THRESHOLD 100
#define THICKNESS .05f

layout(location = 0) in vec2 texCoords;
layout(location = 0) out vec4 finalColor;

layout(push_constant) uniform Push {
    bool showRaySearchCount;
    bool showRayTestCount;
    uint searchCountDivisor;
} settings;

float sdBoxFrame(vec3 p, vec3 boxWidth, float thickness) {
    p = abs(p)-boxWidth;
    vec3 q = abs(p+thickness)-thickness;
    return min(min(
    length(max(vec3(p.x, q.y, q.z), 0.0))+min(max(p.x, max(q.y, q.z)), 0.0),
    length(max(vec3(q.x, p.y, q.z), 0.0))+min(max(q.x, max(p.y, q.z)), 0.0)),
    length(max(vec3(q.x, q.y, p.z), 0.0))+min(max(q.x, max(q.y, p.z)), 0.0));
}

bool rayMarchCombined(vec3 ro, vec3 rd, out vec3 hitPos, out uint hitType) {
    float t = 0.0;
    hitType = 0;// 0 = no hit, 1 = box frame, 2 = ground

    vec3 boxSize = vec3(
    globalData.axisLengths.x / 2.0,
    globalData.axisLengths.y / 2.0,
    globalData.axisLengths.z / 2.0);

    for (int i = 0; i < 256; i++) {
        hitPos = ro + t * rd;

        // Signed distances to each object
        float dBox = sdBoxFrame(hitPos - boxSize, boxSize, 0.1);
        float dGround = hitPos.y;

        // Minimum distance
        float d = min(dBox, dGround);

        if (d < 0.001) {
            // Determine what was hit
            if (dBox < dGround) hitType = 1;
            else hitType = 2;
            return true;
        }

        if (t > THRESHOLD || d < 0.0) break;
        t += max(d, 0.001);
    }

    return false;
}

float getGridLine3D(vec3 pos, float gridScale){
    float scale = gridScale * SCALE;

    float dist_x_plane = min(abs(fract(pos.x / scale) * scale), abs(fract(-pos.x / scale) * scale));
    float dist_y_plane = min(abs(fract(pos.y / scale) * scale), abs(fract(-pos.y / scale) * scale));
    float dist_z_plane = min(abs(fract(pos.z / scale) * scale), abs(fract(-pos.z / scale) * scale));

    float onLineZ = (1.0 - step(THICKNESS, dist_x_plane)) * (1.0 - step(THICKNESS, dist_y_plane));
    float onLineX = (1.0 - step(THICKNESS, dist_y_plane)) * (1.0 - step(THICKNESS, dist_z_plane));

    float onAnyLine = max(onLineX, onLineZ);
    return 1.0 - onAnyLine;
}

vec4 getGridColor(vec2 texCoords, inout vec3 hitPoint) {
    bool hasData = false;
    vec3 rayDir;
    vec3 rayOrigin;
    createRay(texCoords, globalData.invProj, globalData.invView, globalData.isOrtho, rayOrigin, rayDir);
    uint hitType;
    hasData = rayMarchCombined(rayOrigin, rayDir, hitPoint, hitType);

    vec3 xAxisColor = vec3(1.0, 0.0, 0.0);
    vec3 yAxisColor = vec3(0.0, 1.0, 0.0);
    vec3 zAxisColor = vec3(0.0, 0.0, 1.0);

    if (hitType == 1) {

        float isXAxis = step(max(abs(hitPoint.y), abs(hitPoint.z)), .25);
        float isYAxis = step(max(abs(hitPoint.x), abs(hitPoint.z)), .25);
        float isZAxis = step(max(abs(hitPoint.x), abs(hitPoint.y)), .25);

        vec3 axisColorSum = xAxisColor * isXAxis + yAxisColor * isYAxis + zAxisColor * isZAxis;
        axisColorSum = clamp(axisColorSum, 0.0, 1.0);

        if (length(axisColorSum.rgb) == 0) return vec4(1);

        return vec4(axisColorSum.rgb, 1);
    }

    if (globalData.isStaticCurve){
        return vec4(0);
    }

    if (hasData){
        float distanceFromCamera = length(globalData.cameraWorldPosition.xyz - hitPoint);
        float alpha = 1.0;
        if (distanceFromCamera >= THRESHOLD){
            alpha = 0.0;
        } else {
            float fadeRange = THRESHOLD * 0.1;
            alpha = smoothstep(THRESHOLD, THRESHOLD - fadeRange, distanceFromCamera);
        }

        float inBounds = hitPoint.x < 0 || hitPoint.z < 0 ? 0 : step(hitPoint.x, float(globalData.axisLengths.x)) * step(hitPoint.z, float(globalData.axisLengths.z));
        alpha *= inBounds;

        if (alpha > 0.0){
            vec3 baseColor = vec3(0.9);

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

            vec4 final = mix(gridColor, centerLineColor, onAnyAxis);
            final.a = min(alpha, final.a);

            if (final.a == 0.0 || isDitherDiscard(final.a) && !globalData.isOrtho) {
                return vec4(0);
            }

            return final;
        } else {
            return vec4(0);
        }
    } else {
        return vec4(0);
    }
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 colorFromPosition(vec3 position, bool sat0) {
    if (globalData.isStaticCurve){
        return vec3(1, 0, 1) * (abs(position.y) / globalData.axisLengths.y + abs(position.x) / globalData.axisLengths.x + abs(position.z) / globalData.axisLengths.z) / 3.f;
    }

    float time = position.x;
    float magnitude = position.y;
    float amplitude = position.z;

    float normalizedTime = fract(time * 0.1);
    float normalizedMag = clamp(magnitude * 0.1, 0.0, 1.0);
    float normalizedAmp = clamp(amplitude * 0.1, 0.0, 1.0);

    float hue = normalizedTime;
    float saturation = 0.3 + 0.7 * normalizedAmp;
    float value = 0.2 + 0.8 * normalizedMag;

    return hsv2rgb(vec3(hue, sat0 ? 0. : saturation, value));
}

void main() {
    finalColor = vec4(0);
    vec3 gridHitPoint;
    finalColor = getGridColor(texCoords, gridHitPoint);

    if (finalColor.a == 0) {
        finalColor = vec4(vec3(.8), 1);
    }

    vec3 rayDirection;
    vec3 rayOrigin;
    createRay(texCoords, globalData.invProj, globalData.invView, globalData.isOrtho, rayOrigin, rayDirection);

    ivec2 colorData = ivec2(0);
    Ray ray = Ray(rayOrigin, rayDirection, 1./rayDirection);
    SurfaceInteraction hitData = trace(ray, settings.showRaySearchCount, settings.showRayTestCount, colorData, WORLD_VOXEL_SCALE);

    bool isDebugMode = settings.showRaySearchCount || settings.showRayTestCount;

    if (isDebugMode){
        if (isDebugMode){
            finalColor.rg = colorData/float(settings.searchCountDivisor);
            finalColor.a = 1;
        }
    } else if(hitData.voxel != 0){
        bool shouldBlend = hitData.voxelPosition.x < 0 || hitData.voxelPosition.y < 0 || hitData.voxelPosition.z < 0;
        bool isBehind = globalData.cameraWorldPosition.y > 0 && (length(globalData.cameraWorldPosition - hitData.voxelPosition) > length(globalData.cameraWorldPosition - gridHitPoint));
        float blend =(shouldBlend ? .5 : isBehind ? 0: 1);
        finalColor = vec4(mix(finalColor.rgb, colorFromPosition(hitData.voxelPosition.xyz, hitData.voxelPosition.y - hitData.voxelSize == 0), blend), 1);
    }
}