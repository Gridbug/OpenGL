#version 330

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_color;

out vec4 ex_color;

void main(void)
{
    gl_Position = (projection_matrix * view_matrix * model_matrix) * in_position;

    ex_color = in_color;
}
