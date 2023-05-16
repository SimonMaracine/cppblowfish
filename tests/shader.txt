#version 430 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texture_coordinate;
layout(location = 2) in float a_texture_index;

out vec2 v_texture_coordinate;
out flat float v_texture_index;

layout(binding = 4) uniform Projection {
    mat4 u_projection_matrix;
};

void main() {
    v_texture_coordinate = a_texture_coordinate;
    v_texture_index = a_texture_index;

    gl_Position = u_projection_matrix * vec4(a_position, 0.0, 1.0);
}
