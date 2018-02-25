#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 fragColor;

void main()
{
	gl_Position = P * V * M * vertPos;
    vec3 base_color = vec3(20/255.0, 51/255.0, 6/255.0);
    
    vec3 updatedNor = (M * vec4(vertNor, 1)).xyz; 

    vec3 light_vec = normalize(vec3(-2, 1, 4));
    float diffuse = max(dot(vertNor, light_vec), 0.1);
    fragColor = base_color * diffuse;
}
