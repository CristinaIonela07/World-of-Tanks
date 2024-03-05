#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;


uniform vec3 object_color;
uniform int hp;
uniform int type;
// Output value to fragment shader
out vec3 color;


void main()
{
    vec3 new_position = v_position;
    if (hp != 0 && type != -1 && (int(length(v_position))) % (hp+1) == 0) {
        new_position = v_position + (0.5 * hp + (sin(-v_position.x) + sin(v_position.z))/2)* vec3(0.2, 0.5, 0);
    }
    vec3 world_pos = (Model * vec4(new_position, 1)).xyz;
    vec3 world_normal = normalize(mat3(Model) * normalize(v_normal)).xyz;

    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_pos);
    vec3 V = normalize(eye_position - world_pos);
    vec3 H = normalize(L + V);

    float ambient_light = 0.25;

    float diffuse_light = material_kd * max(dot(N, L), 0);

    float specular_light = 0;
    vec3 R = reflect(-L, N);

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }


    float dist = distance(world_pos, light_position);
    float att = 1/(dist*dist);

    float light = ambient_light + (diffuse_light + specular_light);

    color = object_color * light;

    gl_Position = Projection * View * Model * vec4(new_position, 1.0);
}
