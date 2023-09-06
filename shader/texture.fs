#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

// texture 를 쉐이더 안에서 사용하려면, sampler2D라는 타입을 uniform으로 선언해줘야 한다.
uniform sampler2D tex;
uniform sampler2D tex2;

void main() {
    // 두개의 texture를 0.8 : 0.2의 비율로 블랜딩.
    // 두개의 texture가 합쳐져서 보이게 된다.
    fragColor = texture(tex, texCoord) * 0.8 +
        texture(tex2, texCoord) * 0.2;
}
