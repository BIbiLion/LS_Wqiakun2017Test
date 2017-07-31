/*struct directional_light
{
    vec3 direction;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
};

struct material_properties
{
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
    float specularPower;
    float opacity;
    float brightness;
};

uniform directional_light qt_Light;
uniform material_properties qt_Material;
uniform vec3 qt_EyePosition;
uniform samplerCube qt_Environment;

varying vec3 v_Position;
varying vec3 v_Normal;
varying vec3 v_TexCoord;

const float c_zero = 0.0;
const float c_one = 1.0;

vec4 evaluateColor(in vec3 normal, in vec3 texCoord)
{
    // Start with black color
    vec3 finalColor = vec3(c_zero, c_zero, c_zero);

    // Upgrade black color to the base ambient color
    finalColor += qt_Light.ambientColor.rgb * qt_Material.ambientColor.rgb;

    // Add diffuse component to it
    vec3 lightDir = normalize(qt_Light.direction);
    float diffuseFactor = max( c_zero, dot(lightDir, normal) );
    if(diffuseFactor > c_zero)
    {
        finalColor += qt_Light.diffuseColor.rgb *
                qt_Material.diffuseColor.rgb *
                diffuseFactor * qt_Material.brightness;
    }

    // Add specular component to it
    vec3 viewDir = normalize(qt_EyePosition - v_Position);
    vec3 reflectionVec = reflect(viewDir, normal);
    const vec3 blackColor = vec3(c_zero, c_zero, c_zero);
    if( !(qt_Material.specularColor.rgb == blackColor ||
          qt_Light.specularColor.rgb == blackColor ||
          qt_Material.specularPower == c_zero) )
    {
        float specularFactor = max( c_zero, dot(reflectionVec, -viewDir) );
        if(specularFactor > c_zero)
        {
            specularFactor = pow( specularFactor, qt_Material.specularPower );
            finalColor += qt_Light.specularColor.rgb *
                    qt_Material.specularColor.rgb *
                    specularFactor;
        }
    }

    // Texture mapping
    finalColor *= textureCube(qt_Environment, reflectionVec).rgb;

    // All done!
    return vec4( finalColor, c_one );
}

void main(void)
{
    gl_FragColor = evaluateColor(v_Normal, v_TexCoord);
}*/
#version 330 core
#extension GL_NV_shadow_samplers_cube: enable//添加扩展 启用
in vec3 v_TexCoord;
uniform samplerCube qt_Environment;
out vec4 FragColor;
void main()
{
  FragColor= textureCube (qt_Environment,v_TexCoord);
}

