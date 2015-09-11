#version 330 core
precision highp float;

struct Light
{
  vec3 position;
  vec3 color;
  float intensity;
  float linear_attenuation;
  float quadratic_attenuation;
  float cone_angle;
  vec3 cone_direction;
  float quantization_bandwidth;
  int number_quantized_bands;
};

const int MAX_LIGHTS = 8;

in vec2 uv;
layout(location = 0)out vec4 fragColor;

uniform sampler2D tileset;
uniform int num_lights;
uniform Light lights[MAX_LIGHTS];

uniform vec4 ambient_color;    //ambient RGBA -- alpha is intensity 
uniform vec2 resolution;      //resolution of screen

vec3 applyLight(Light light, vec3 surface_color, vec3 normal, vec2 surface_pos) {
  vec3 surface_to_light;
  float attenuation = 1.0;
  //point light
  surface_to_light = light.position.xyz - vec3(surface_pos, -1.0);
  surface_to_light.x *= resolution.x / resolution.y;
  float distance_to_light = length(surface_to_light);
  if(light.quantization_bandwidth != 0.0) {
    float xy_dist = length(surface_to_light.xy);

    if(xy_dist < light.quantization_bandwidth) {
      distance_to_light = light.quantization_bandwidth;
    }
    for(int i = 1; i < light.number_quantized_bands; i++) {
      if(xy_dist >= float(i) * light.quantization_bandwidth && xy_dist < float(i+1) * light.quantization_bandwidth)
        distance_to_light = light.quantization_bandwidth * float(i);
    }
    if(xy_dist > float(light.number_quantized_bands) * light.quantization_bandwidth) {
      distance_to_light = 500.0;
    }
  }
  attenuation = 1.0 / ( 1.0 + (light.linear_attenuation * distance_to_light) + (light.quadratic_attenuation * distance_to_light * distance_to_light));

  //cone restrictions (affects attenuation)
  // float light_to_surface_angle = degrees(acos(dot(-surface_to_light, normalize(light.cone_direction))));
  // if(light.cone_angle > 0.0 && light_to_surface_angle > light.cone_angle){
  //     attenuation = 0.0;
  // }

  //ambient
  vec3 ambient = ambient_color.rgb * ambient_color.a;

  //diffuse
  float diffuse_coefficient = max(0.0, dot(normal, surface_to_light));
  vec3 diffuse = diffuse_coefficient * light.color * light.intensity;
  
  vec3 intensity = ambient + attenuation * diffuse;
  //linear color (color before gamma correction)
  return clamp(intensity * surface_color.rgb, vec3(0.0), vec3(1.0));
}

void main()
{
  vec4 texel = texture(tileset, uv);

  if(texel.a < 0.1)
    discard;

  vec3 final_color = vec3(0.0, 0.0, 0.0);

  for(int i = 0; i < num_lights; i++) {
    final_color += applyLight(lights[i], texel.rgb, vec3(0.0, 0.0, 1.0), gl_FragCoord.xy / resolution.xy);
  }

  fragColor = vec4(clamp(final_color, vec3(0.0), vec3(1.0)), texel.a);
}