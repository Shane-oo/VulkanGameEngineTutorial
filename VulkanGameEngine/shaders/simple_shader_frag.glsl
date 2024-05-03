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
    mat4 inverseViewMatrix;
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
    vec3 specularLight = vec3(0.0);
    vec3 surfaceNormal = normalize(fragNormalWorld);
    
    vec3 cameraPositionWorld = ubo.inverseViewMatrix[3].xyz;
    vec3 viewDirection = normalize(cameraPositionWorld - fragPositionWorld);

    for (int i = 0; i < ubo.numLights; i++) {
        PointLight light = ubo.pointLights[i];

        vec3 directionToLight = light.position.xyz - fragPositionWorld;
        float attenuation = 1.0f / dot(directionToLight, directionToLight); // distance squared
        directionToLight = normalize(directionToLight);
        
        float cosAngleOfIncidence = max(dot(surfaceNormal, directionToLight), 0);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;

        diffuseLight += intensity * cosAngleOfIncidence;
        
        // specular lighting
        vec3 halfAngle = normalize(directionToLight + viewDirection);
        float blinnTerm = dot(surfaceNormal, halfAngle);
        blinnTerm = clamp(blinnTerm, 0,1);
        blinnTerm = pow(blinnTerm, 512.0);   // higher values -> sharper highlight
        specularLight += light.color.xyz * (intensity * blinnTerm);
    }

    outColour = vec4((diffuseLight * fragColour) + (specularLight * fragColour), 1.0);
}