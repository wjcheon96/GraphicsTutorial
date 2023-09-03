#version 330 core
// location 0은 VAO attribute 0번을 의미한다.
layout (location = 0) in vec3 aPos;

void main() {
  // aPos로 들어오는 data를 vec4에 넣어준다.
  gl_Position = vec4(aPos, 1.0);
}
