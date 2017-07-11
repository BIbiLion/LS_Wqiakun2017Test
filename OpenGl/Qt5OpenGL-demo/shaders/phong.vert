#version 130
in vec3 vertexPosition;
in vec3 vertexColor;

uniform mat4 transform;

out vec3 outColor;


in vec2 aTexCoord;
out vec2 TexCoord;
void main()
{
    outColor = vertexColor;
    gl_Position =transform* vec4(vertexPosition, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
