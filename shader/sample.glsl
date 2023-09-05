// pre define macro.
// 버전명 등을 명시한다.
#version 330 core

// in, out, uniform과 같은 것들을 한정자(qaulifier)라고 부르며, 
// 해당 변수가 어떤 형태로 쓰이는지를 제한하는 역할을 한다.
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

void main() {
    //
    out variable_name = weird_stuff_we_processed;
}