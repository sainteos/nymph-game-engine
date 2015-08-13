#version 330 core
precision highp float;

in vec2 uv;
layout(location = 0)out vec4 fragColor;

uniform sampler2D tileset;

void main()
{
  fragColor = vec4(1.0, 0.0, 0.0, 1.0);
  //fragColor = texture(tileset, uv);
}