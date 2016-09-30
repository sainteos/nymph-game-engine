#version 330 core
precision highp float;

in vec2 uv;
layout(location = 0)out vec4 fragColor;

uniform sampler2D text_texture;
uniform vec4 color;

void main()
{
  vec4 texel = vec4(color.rgb, color.a * texture(text_texture, uv).r); 
  if(texel.a == 0)
    discard;
  fragColor = texel;
}