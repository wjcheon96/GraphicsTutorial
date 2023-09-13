#version 330 core
in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

// 광원의 색상
uniform vec3 lightColor;
// 내가 그리려는 objectd의 색상.
uniform vec3 objectColor;
// lightColor를 ambientStrength를 통해 조절한다.
uniform float ambientStrength;

void main() {
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient * objectColor;
    fragColor = vec4(result, 1.0);
}
