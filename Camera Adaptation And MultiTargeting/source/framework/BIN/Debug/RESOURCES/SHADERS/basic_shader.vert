#version 400 core
layout(location = 0) in vec3 position;
uniform mat4 MVP;
uniform vec4 COLOR;
out vec4 vertColor;
void main()
{
    gl_Position = MVP * position;
    vertColor = COLOR;
}