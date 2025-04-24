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

vec3 randomColor(float seed) {
    float r = rand(vec3(seed));
    float g = rand(vec3(seed + r));
    return vec3(r, g, rand(vec3(seed + g)));
}


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

vec4 getGridColor(vec2 texCoords) {
    bool hasData = false;
    vec3 hitPoint;
    vec3 rayDir = createRay(texCoords, globalData.invProj, globalData.invView);
    hasData = rayMarch(globalData.cameraWorldPosition.xyz, rayDir, hitPoint);

    if (hasData){
        float distanceFromCamera = length(globalData.cameraWorldPosition.xyz - hitPoint);
        float alpha = 1.0;
        if (distanceFromCamera >= THRESHOLD){
            alpha = 0.0;
        } else {
            float fadeRange = THRESHOLD * 0.1;
            alpha = smoothstep(THRESHOLD, THRESHOLD - fadeRange, distanceFromCamera);
        }

        float inBounds = step(abs(hitPoint.x), WORLD_SIZE) * step(abs(hitPoint.y), WORLD_SIZE) * step(abs(hitPoint.z), WORLD_SIZE);
        alpha *= inBounds;

        if (alpha > 0.0){
            vec3 baseColor = vec3(0.9);
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

            vec4 final = mix(gridColor, centerLineColor, onAnyAxis);
            final.a = min(alpha, final.a);

            if (final.a == 0.0 || isDitherDiscard(final.a)) {
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

vec3 colorFromPosition(vec3 position) {
    float time = position.x;
    float magnitude = position.y;
    float amplitude = position.z;

    float normalizedTime = fract(time * 0.1);
    float normalizedMag = clamp(magnitude * 0.1, 0.0, 1.0);
    float normalizedAmp = clamp(amplitude * 0.1, 0.0, 1.0);

    float hue = normalizedTime;
    float saturation = 0.3 + 0.7 * normalizedAmp;
    float value = 0.2 + 0.8 * normalizedMag;

    return hsv2rgb(vec3(hue, saturation, value));
}

void main() {
    finalColor = getGridColor(texCoords);
    if(finalColor.a == 0) {
        finalColor = vec4(.8, .8, .8, 1);
    }

    vec3 rayOrigin = globalData.cameraWorldPosition.xyz;
    vec3 rayDirection = createRay(texCoords, globalData.invProj, globalData.invView);
    ivec2 colorData = ivec2(0);
    Ray ray = Ray(rayOrigin, rayDirection, 1./rayDirection);
    SurfaceInteraction hitData = trace(ray, settings.showRaySearchCount, settings.showRayTestCount, colorData, WORLD_SIZE * 10.f);
    if (hitData.voxel == 0){
        if (settings.showRaySearchCount || settings.showRayTestCount){
            finalColor.rg = colorData/float(settings.searchCountDivisor);
            finalColor.a = 1;
        }
    }else{
        finalColor = vec4(colorFromPosition(hitData.voxelPosition.xyz), 1);
    }
}