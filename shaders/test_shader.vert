#version 330

layout(location = 0)in vec3 vert;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vert, 1.0);
}