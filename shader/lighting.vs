	#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// 주변광
uniform mat4 transform;
// 분산광
// local coordinate을 world coordinate으로 바꾸는 matrix
uniform mat4 modelTransform;

// 주변광
out vec3 normal;
out vec2 texCoord;
// 분산광
out vec3 position;


void main() {
    // 모델의 local coordinate을 기준으로 있는 vertex가 화면상에 어디에 있는지를, canonical coordinate(-1 ~ +1)으로 전환된다.
    gl_Position = transform * vec4(aPos, 1.0);
    // modelTransform * vec4(aNormal, 0.0).xyz가 아니라, 점이 아닌 벡터이기에 제대로 변환된 값을 계산할 수 있다.
    // matrix의 inverse transpose는 모든 점에서 동일하므로 별도의 uniform으로 입력함.
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
    texCoord = aTexCoord;
    // world coordinate 상의 정점의 위치.
    position = (modelTransform * vec4(aPos, 1.0)).xyz;
}