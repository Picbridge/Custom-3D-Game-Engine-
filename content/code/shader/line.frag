#version 400 core
uniform vec3 color;
out vec4 FragColor;
in float posColor;
void main()
{
    FragColor = vec4(color, 1.0);
}
