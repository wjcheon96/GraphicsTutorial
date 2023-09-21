#version 330 core
in vec3 normal;
in vec2 texCoord;
// fragment shader로 넘어온 vertex의 world coordinate 상의 position.
in vec3 position;
out vec4 fragColor;

uniform vec3 viewPos;

// blinn shading 방식을 사용할 것인지를 체크.
uniform int blinn;

// 구조체에 담에서 하나로 묶는다.
struct Light {
    // 광원의 위치를 나타내므로 light struture에만 들어간다.
    vec3 position;
    vec3 direction;
    vec2 cutoff;
    vec3 attenuation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

struct Material {
    // diffuse map을 사용한다.
    // material의 diffuse color와 ambient color는 통상적으로 동일한 형태로 많이 쓴다.
    sampler2D diffuse;
    sampler2D specular;
    // 표면의 반짝거림을 표현하기에 meterial로 들어간다.
    float shininess;
};
uniform Material material;

void main() {
    vec3 texColor = texture(material.diffuse, texCoord).xyz;
    // 주변광 계산
    vec3 ambient = texColor * light.ambient;

    // 광원과 3D 위치간의 거리를 계산.
    float dist = length(light.position - position);
    // 각각 0차, 1차, 2차 항을 나타낸다.
    vec3 distPoly = vec3(1.0, dist, dist * dist);
    // distPoly와 light.attenuation을 내적한다.
    // 실질적으로는 Kc, Kl, Kq가 들어가는 형태가 된다.
    float attenuation = 1.0 / dot(distPoly, light.attenuation);
    vec3 lightDir = (light.position - position) / dist;
    vec3 result = ambient;
    // 새로 입력된 vertex 의 normal vector
    float theta = dot(lightDir, normalize(-light.direction));
    // 내분점 공식을 쓰고, clamp 함수를 통해 0~1 사이의 값으로 만들어준다.
    float intensity = clamp((theta - light.cutoff[1]) / (light.cutoff[0] - light.cutoff[1]), 0.0, 1.0);

    // theta값은 광원의 방향과 lightDir과 실제 광원의 direction을 내적한 값, 즉 cos 값을 계산한다.
    // 해당 cos값이 지정한 cos 값보다 크단 말은 지정한 각도보다 작다는 말이며, 이 경우에만 spotlight에 대한 조명을 계산한다.
    if (intensity > 0.0) {
        vec3 pixelNorm = normalize(normal);
        // 빛의 크기를 구하되, pixelNorm과 lightDir의 내적 값을 계산하는데, 이 수치가 음수일때는 0으로 둠.
        // 이후 material과 light의 diffuse와 곱해서 값을 구함.
        float diff = max(dot(pixelNorm, lightDir), 0.0);
        vec3 diffuse = diff * texColor * light.diffuse;

        vec3 specColor = texture(material.specular, texCoord).xyz;
        float spec = 0.0;
        if (blinn == 0) {
            // light direction을 구한 방식과 동일.
            // 현재 카메라의 world space 상의 좌표와 픽셀 좌표 간의 차를 통해 시선 벡터 viewDir 계산.
            vec3 viewDir = normalize(viewPos - position);
            // light 벡터 방향의 광선이 normal 벡터 방향의 표면에 부딪혔을 때 반사되는 벡터를 출력하는 내장함수인 reflect 함수를 이용.
            vec3 reflectDir = reflect(-lightDir, pixelNorm);
            // reflectDir과 viewDir 간의 내적을 통해 반사광을 많이 보는 정도를 계산
            spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        }
        // blinn 방식을 사용.
        else {
            vec3 viewDir = normalize(viewPos - position);
            vec3 halfDir = normalize(lightDir + viewDir);
            spec = pow(max(dot(halfDir, pixelNorm), 0.0), material.shininess);
        }
        vec3 specular = spec * specColor * light.specular;
        result += (diffuse + specular) * intensity;
    }
    result *= attenuation;
    fragColor = vec4(result, 1.0);
}
