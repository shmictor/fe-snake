#version 150

in vec2 center_f;
in float radius_f;
in vec3 color_f;

out vec4 frag_color;

void main() {
  if (center_f.x > -1.0) {
    vec2 p = gl_FragCoord.xy / 512;
    if (distance(center_f, p) > radius_f) {
      discard;
    }
  }
  frag_color = vec4(color_f, 1.0);
}