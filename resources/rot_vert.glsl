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
    vec3 base_color = vec3(50/255.0, 91/255.0, 36/255.0);
    
    if (uStyle == 0)
    {
        vec3 updatedNor = (M * vec4(vertNor, 0)).xyz; 

        vec3 light_vec = normalize(uLight);
        float diffuse = max(dot(normalize(updatedNor), light_vec), 0);
        fragColor = base_color * diffuse + 0.2*base_color;
    }
    else
    {
        fragColor = base_color;
    }

	fragNor = (M * vec4(vertNor, 0.0)).xyz;
}
