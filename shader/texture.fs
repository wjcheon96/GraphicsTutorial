#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

// texture 를 쉐이더 안에서 사용하려면, sampler2D라는 타입을 uniform으로 선언해줘야 한다.
uniform sampler2D tex;

void main() {
    fragColor = texture(tex, texCoord);
}
