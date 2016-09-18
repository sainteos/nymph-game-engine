#version 330 core
precision highp float;

in vec2 uv;
layout(location = 0)out vec4 fragColor;

uniform sampler2D skin0;
uniform vec4 color;

void main()
{
  float gray_value = texture(skin0, uv).r;
  fragColor = vec4(color.rgb * gray_value, color.a);
}