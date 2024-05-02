#version 450

layout (location = 0) in vec3 fragColour;
layout (location = 1) in vec3 fragPositionWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColour;

struct PointLight {
    vec4 position; // ignore w
    vec4 color; // w is intensity
};

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec4 ambientLightColour;
    PointLight pointLights[10]; // 10 is MAX_LIGHTS
    int numLights;
} ubo;

layout (push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;


void main() {

    vec3 diffuseLight = ubo.ambientLightColour.xyz * ubo.ambientLightColour.w;
    vec3 surfaceNormal = normalize(fragNormalWorld);

    for (int i = 0; i < ubo.numLights; i++) {
        PointLight light = ubo.pointLights[i];

        vec3 directionToPointLight = light.position.xyz - fragPositionWorld;
        float attenuation = 1.0f / dot(directionToPointLight, directionToPointLight); // distance squared
        float cosAngleOfIncidence = max(dot(surfaceNormal, normalize(directionToPointLight)), 0);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;

        diffuseLight += intensity * cosAngleOfIncidence;
    }

    outColour = vec4(diffuseLight * fragColour, 1.0);
}