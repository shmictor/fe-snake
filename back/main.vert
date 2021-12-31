#version 150

in vec2 position;
in float size;
in float is_circle;
in vec3 color;

out float half_size_g;
out float is_circle_g;
out vec3 color_g;

void main() {
  gl_Position = vec4((position * 2 - 15.0) / 16.0, 0.0, 1.0);
  half_size_g = size / 16.0;
  is_circle_g = is_circle;
  color_g = color;
}