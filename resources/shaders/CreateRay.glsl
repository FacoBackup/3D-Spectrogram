void createRay(
in vec2 texCoords,
in mat4 invProjectionMatrix,
in mat4 invViewMatrix,
in bool isOrtho,
out vec3 rayOrigin,
out vec3 rayDirection
) {
    vec2 pxNDS = texCoords * 2.0 - 1.0;

    if (isOrtho) {
        // Project texCoord to world-space origin using inverse projection + view matrix
        vec4 ndc = vec4(pxNDS, 0.0, 1.0); // Z = 0 (usually near plane)
        vec4 originView = invProjectionMatrix * ndc;
        vec4 originWorld = invViewMatrix * originView;

        vec4 dirView = vec4(0.0, 0.0, -1.0, 0.0); // Fixed direction
        vec4 dirWorld = invViewMatrix * dirView;

        rayOrigin = originWorld.xyz;
        rayDirection = normalize(dirWorld.xyz);
    } else {
        vec4 ndc = vec4(pxNDS, -1.0, 1.0); // z = -1 for near plane
        vec4 pointEye = invProjectionMatrix * ndc;
        pointEye /= pointEye.w;

        vec4 dirView = vec4(pointEye.xyz, 0.0);
        vec4 dirWorld = invViewMatrix * dirView;

        rayOrigin = (invViewMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
        rayDirection = normalize(dirWorld.xyz);
    }
}
