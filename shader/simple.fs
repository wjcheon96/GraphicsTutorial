#version 330 core
uniform vec4 color; //이와 같은 방식으로 선언 가능.
out vec4 FragColor; // 최종 출력 색상

void main() {
    // FragColor = vertexColor;
    FragColor = color;
}