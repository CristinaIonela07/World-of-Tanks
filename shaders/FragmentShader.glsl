#version 330

// Input
// TODO(student): Get color value from vertex shader
uniform int type;
uniform int hp;

in vec3 color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    vec3 int_color = color;
    if (hp != 0 && type != -1 && (int(length(color))) % (int(hp)+1) == 0) {
        int_color = color + (0.5 * (-hp) + (sin(color.x) + sin(color.z))/2) * vec3(0.3, 0.5, 0.6);
    }
    out_color = vec4(int_color, 1);

}
