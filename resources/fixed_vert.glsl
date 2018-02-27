#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform int uStyle;
uniform vec3 uLight;
out vec3 fragColor;
out vec3 fragNor;

void main()
{
	gl_Position = P * V * M * vertPos;

    vec3 base_color = vec3(0, 89/255.0, 179/255.0);

    if (uStyle == 0)
    {
        vec3 light_vec = normalize(uLight);
        float diffuse = max(dot(vertNor, light_vec), 0.1);
        fragColor = base_color * diffuse;
    }
    else
    {
        fragColor = base_color;
    }

    fragNor = vertNor;
}
