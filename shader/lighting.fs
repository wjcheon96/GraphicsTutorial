#version 330 core
in vec3 normal;
in vec2 texCoord;
// fragment shader로 넘어온 vertex의 world coordinate 상의 position.
in vec3 position;
out vec4 fragColor;

uniform vec3 viewPos;

// specularStrength로 반사광의 정도를 조절
uniform float specularStrength;
// specularShininess로 반사광의 면적 조절
uniform float specularShininess;

uniform vec3 lightPos;
// 광원의 색상
uniform vec3 lightColor;
// 내가 그리려는 object의 색상.
uniform vec3 objectColor;
// lightColor를 ambientStrength를 통해 조절한다.
uniform float ambientStrength;

void main() {

    vec3 ambient = ambientStrength * lightColor;
    // 빛의 방향
    vec3 lightDir = normalize(lightPos - position);
    // 새로 입력된 vertex 의 normal vector
    vec3 pixelNorm = normalize(normal);
    // 빛의 크기를 구하되, pixelNorm과 lightDir의 내적 값을 계산하는데, 이 수치가 음수일때는 0으로 둠.
    vec3 diffuse = max(dot(pixelNorm, lightDir), 0.0) * lightColor;

    // light direction을 구한 방식과 동일.
    // 현재 카메라의 world space 상의 좌표와 픽셀 좌표 간의 차를 통해 시선 벡터 viewDir 계산.
    vec3 viewDir = normalize(viewPos - position);
    // light 벡터 방향의 광선이 normal 벡터 방향의 표면에 부딪혔을 때 반사되는 벡터를 출력하는 내장함수인 reflect 함수를 이용.
    vec3 reflectDir = reflect(-lightDir, pixelNorm);
    // reflectDir과 viewDir 간의 내적을 통해 반사광을 많이 보는 정도를 계산
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess);
    vec3 specular = specularStrength * spec * lightColor;

    // 최종적으로 분산광과 반사광의 합을 통해 실제 색상을 결정한다. 
    vec3 result = (ambient + diffuse + specular) * objectColor;

    fragColor = vec4(result, 1.0);
}
