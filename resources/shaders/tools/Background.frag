layout(push_constant) uniform Push {
    vec3 color;
} push;

layout(location = 0) out vec4 finalColor;

void main() {
    finalColor.rgb = push.color;
    finalColor.a = 1;
}