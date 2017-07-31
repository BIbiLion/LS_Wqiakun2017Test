attribute vec3 qt_Vertex;
attribute vec3 qt_Normal;

uniform mat4 qt_NormalMatrix;
uniform mat4 qt_ModelViewMatrix;
uniform mat4 qt_ProjectionMatrix;

varying vec3 v_Position;
varying vec3 v_Normal;
varying vec3 v_TexCoord;

const float c_zero = 0.0;
const float c_one = 1.0;

void main(void)
{
    vec4 normal = normalize( qt_NormalMatrix * vec4(qt_Normal, c_zero) );
    v_Normal = normal.xyz;

    vec4 position = qt_ModelViewMatrix * vec4(qt_Vertex, c_one);
    v_Position = position.xyz;

    v_TexCoord = v_Normal;

    mat4 mat = qt_ProjectionMatrix * qt_ModelViewMatrix;
    gl_Position = mat * vec4(qt_Vertex, c_one);
}
