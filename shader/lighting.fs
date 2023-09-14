#version 330 core
in vec3 normal;
in vec2 texCoord;
// fragment shader로 넘어온 vertex의 world coordinate 상의 position.
in vec3 position;
out vec4 fragColor;

uniform vec3 viewPos;

// 구조체에 담에서 하나로 묶는다.
struct Light {
    // 광원의 위치를 나타내므로 light struture에만 들어간다.
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 표면의 반짝거림을 표현하기에 meterial로 들어간다.
    float shininess;
};
uniform Material material;

void main() {
    // 주변광 계산
    vec3 ambient = material.ambient * light.ambient;

    // 빛의 방향
    vec3 lightDir = normalize(light.position - position);
    // 새로 입력된 vertex 의 normal vector
    vec3 pixelNorm = normalize(normal);
    // 빛의 크기를 구하되, pixelNorm과 lightDir의 내적 값을 계산하는데, 이 수치가 음수일때는 0으로 둠.
    // 이후 material과 light의 diffuse와 곱해서 값을 구함.
    float diff = max(dot(pixelNorm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;

    // light direction을 구한 방식과 동일.
    // 현재 카메라의 world space 상의 좌표와 픽셀 좌표 간의 차를 통해 시선 벡터 viewDir 계산.
    vec3 viewDir = normalize(viewPos - position);
    // light 벡터 방향의 광선이 normal 벡터 방향의 표면에 부딪혔을 때 반사되는 벡터를 출력하는 내장함수인 reflect 함수를 이용.
    vec3 reflectDir = reflect(-lightDir, pixelNorm);
    // reflectDir과 viewDir 간의 내적을 통해 반사광을 많이 보는 정도를 계산
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * light.specular;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
