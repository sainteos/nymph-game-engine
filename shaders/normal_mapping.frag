#version 330 core
precision highp float;

in vec2 uv;
layout(location = 0)out vec4 fragColor;

uniform sampler2D tileset;
uniform sampler2D normal_texture;

uniform vec2 resolution;      //resolution of screen
uniform vec3 light_position;        //light position, normalized
uniform vec4 light_color;      //light RGBA -- alpha is intensity
uniform vec4 ambient_color;    //ambient RGBA -- alpha is intensity 
uniform float linear_attenuation;
uniform float quadratic_attenuation;
uniform float quantization_bandwidth;
uniform int number_quantized_bands;

void main()
{
  vec4 texel = texture(tileset, uv);

  if(texel.a < 0.1)
    discard;

  vec3 normal_texel = texture(normal_texture, uv).rgb;

  vec3 light_direction = vec3(light_position.xy - (gl_FragCoord.xy / resolution.xy), light_position.z);
  //Correct for aspect ratio
  light_direction.x *= resolution.x / resolution.y;
  //Determine distance (used for attenuation) BEFORE we normalize our LightDir
  float dist = length(light_direction);
  
  if(quantization_bandwidth != 0.0) {
    float xy_dist = length(light_direction.xy);

    if(xy_dist < quantization_bandwidth) {
      dist = quantization_bandwidth;
    }
    for(int i = 1; i < number_quantized_bands; i++) {
      if(xy_dist >= float(i) * quantization_bandwidth && xy_dist < float(i+1) * quantization_bandwidth)
        dist = quantization_bandwidth * float(i);
    }
    if(dist > float(number_quantized_bands) * quantization_bandwidth)
      dist = 500.0;
  }
  //normalize our vectors
  vec3 normal = normalize(normal_texel * 2.0 - 1.0);
  normal = vec3(0.0, 0.0, -1.0);
  light_direction = normalize(light_direction);
  //Pre-multiply light color with intensity
  //Then perform "N dot L" to determine our diffuse term
  vec3 diffuse = (light_color.rgb * light_color.a) * max(dot(normal, light_direction), 0.0);    //pre-multiply ambient color with intensity
  vec3 ambient = ambient_color.rgb * ambient_color.a;
  //calculate attenuation
  float attenuation = 1.0 / ( 1.0 + (linear_attenuation*dist) + (quadratic_attenuation*dist*dist) );
  
  //if(length(light_direction.xy) > 1.0)
  //  diffuse = vec3(0.0, 0.0, 0.0);
  
  vec3 intensity = ambient + diffuse * attenuation;
  vec3 final_color = texel.rgb * intensity;

  fragColor = vec4(final_color, texel.a);
}