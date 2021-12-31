#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in float half_size_g[];
in float is_circle_g[];
in vec3 color_g[];

out vec2 center_f;
out float radius_f;
out vec3 color_f;

void main() {
  center_f = bool(is_circle_g[0]) ? (gl_in[0].gl_Position.xy + 1.0) / 2.0 : vec2(-1.0);
  radius_f = half_size_g[0] / 2.0;
  color_f = color_g[0];

  for (float i = 0; i < 4; ++i) {
    float j = float(i > 1.0) * 2.0 - 1.0;
    float k = mod(i, 2.0) * 2.0 - 1.0;
    gl_Position = gl_in[0].gl_Position + vec4(half_size_g[0] * j, half_size_g[0] * k, 0.0, 0.0);
    EmitVertex();
  }
  EndPrimitive();
}