#version 400 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D shadowMap;

void main() {
    float depth = texture(shadowMap, TexCoords).r;
    // For orthographic projection:
    FragColor = vec4(vec3(depth), 1.0);
    
    // For debugging NaN values:
    // if (isnan(depth)) FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // else FragColor = vec4(vec3(depth), 1.0);
}