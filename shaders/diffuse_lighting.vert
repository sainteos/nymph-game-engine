#version 330 core
precision highp float;

layout(location = 0)in vec3 vert;
layout(location = 1)in vec2 tex;
layout(location = 2)in uint texture_unit;

out vec2 uv;
flat out uint unit;
out vec3 surface_pos;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main()
{
  gl_Position = (projection * view * transform) * vec4(vert, 1.0);
  surface_pos = (transform * vec4(vert, 1.0)).xyz;
  uv = tex;
  unit = texture_unit;
}