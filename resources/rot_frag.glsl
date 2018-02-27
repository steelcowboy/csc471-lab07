#version 330 core 
uniform int uStyle;
uniform vec3 uLight;
in vec3 fragColor;
in vec3 fragNor;
out vec4 color;

void main()
{
    if (uStyle == 1)
    {
        vec3 light_vec = normalize(uLight);
        float diffuse = max(dot(normalize(fragNor), light_vec), 0);
        color = vec4(fragColor * diffuse + 0.4*fragColor, 1.0);
    }
    else
    {
        color = vec4(fragColor, 1.0);
    }
}
