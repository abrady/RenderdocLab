#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

layout(binding = 1) uniform CameraUBO {
    mat4 viewProj;
} camera;

void main() {
    gl_Position = camera.viewProj * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}
