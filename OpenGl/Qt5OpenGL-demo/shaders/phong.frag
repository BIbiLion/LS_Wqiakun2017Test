#version 130

in vec3 outColor;

in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D outTexture;
void main()
{
    FragColor=texture2D(outTexture,TexCoord)*vec4(outColor, 1.0);
}
