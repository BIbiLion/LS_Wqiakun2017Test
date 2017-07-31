#version 130
in vec3 vPosition;
uniform mat4 transform;
void main(void)
{

    gl_Position = transform*vec4(vPosition,1.0f);
}
