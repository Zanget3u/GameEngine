#version 330 core

//in vec2 TexCoords;
out vec4 FragColor;

//uniform sampler2D tex;
uniform vec3 color;

void main()
{
    //FragColor = vec4(color, 1.0f) * texture(tex, TexCoords);
    FragColor = vec4(color, 1.0f);
}