#include "Shaders.h"

#include <stdio.h>
#include <stdlib.h>

void Handle_OGL_errors(const char *const message)
{
    GLenum error_check_value = glGetError();
    if (GL_NO_ERROR != error_check_value)
    {
        do
        {
            fprintf(stderr, "OPENGL ERROR #%d.\n", error_check_value);

            error_check_value = glGetError();
        }
        while (GL_NO_ERROR != error_check_value);

        fprintf(stderr, "%s", message);

        exit(EXIT_FAILURE);
    }
}

void Delete_program_and_shaders(GLuint program_id)
{
    GLint num_attached_shdrs = 0;
    glGetProgramiv(program_id, GL_ATTACHED_SHADERS, &num_attached_shdrs);

    GLuint* shaders = (GLuint*) calloc((unsigned int)num_attached_shdrs, sizeof(GLuint));

    glGetAttachedShaders(program_id, num_attached_shdrs, &num_attached_shdrs, shaders);

    for(int i = 0; i < num_attached_shdrs; i++)
    {
        glDetachShader(program_id, shaders[i]);
        glDeleteShader(shaders[i]);
    }

    glDeleteProgram(program_id);
}

static char* Shader_source_to_buff(const char *const file_name)
{
    FILE* shader_source = fopen(file_name, "rb");
    if (NULL == shader_source)
    {
        fprintf(stderr, "ERROR: Failed to open shader source \"%s\". (Bad file name?)\n", file_name);
        return NULL;
    }

    fseek(shader_source, 0, SEEK_END);

    unsigned long source_size = 0;
    long source_size_l = ftell(shader_source);
    if (-1L == source_size_l)
    {
        fprintf(stderr, "ERROR: Failed to count \"%s\" size.\n", file_name);
        return NULL;
    }
    else
    {
        source_size = (unsigned long) source_size_l;
    }

    char* buffer = (char*) calloc(source_size + 1, 1);
    if (NULL == buffer)
    {
        fprintf(stderr, "ERROR: Failed to allocate memory.\n");
        return NULL;
    }

    fseek(shader_source, 0, SEEK_SET);

    fread(buffer, 1, source_size, shader_source);

    fclose(shader_source);

    buffer[source_size] = '\0';
    return buffer;
}

GLuint Make_program_from_shaders(const char *const veterx_shdr_name, const char *const fragment_shdr_name)
{
    char* vertex_shader_source = Shader_source_to_buff(veterx_shdr_name);
    if (NULL == vertex_shader_source)
    {
        fprintf(stderr, "ERROR: Failed to load vertex shader source.\n");
        return 0;
    }
    char* fragment_shader_source = Shader_source_to_buff(fragment_shdr_name);
    if (NULL == fragment_shader_source)
    {
        fprintf(stderr, "ERROR: Failed to load fragment shader source.\n");
        return 0;
    }

    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    if (0 == vertex_shader_id)
    {
        fprintf(stderr, "ERROR: Failed to create vertex shader.\n");
        return 0;
    }
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    if (0 == fragment_shader_id)
    {
        fprintf(stderr, "ERROR: Failed to create fragment shader.\n");
        glDeleteShader(vertex_shader_id);
        return 0;
    }

    glShaderSource(vertex_shader_id, 1, (const GLchar**)&vertex_shader_source, NULL);
    glShaderSource(fragment_shader_id, 1, (const GLchar**)&fragment_shader_source, NULL);

    free(vertex_shader_source);
    free(fragment_shader_source);

    glCompileShader(vertex_shader_id);
    GLint successfully_compiled = 0;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &successfully_compiled);
    if (GL_FALSE == successfully_compiled)
    {
        GLint log_size = 0;
        glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &log_size);

        GLchar* error_log = (GLchar*) calloc((unsigned int)log_size, sizeof(GLchar));

        glGetShaderInfoLog(vertex_shader_id, log_size, &log_size, error_log);
        fprintf(stderr, "ERROR: Failed to compile vertex shader.\n");
        fprintf(stderr, "LOG: %s\n", error_log);
        free(error_log);

        glDeleteShader(vertex_shader_id);
        return 0;
    }
    glCompileShader(fragment_shader_id);
    successfully_compiled = 0;
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &successfully_compiled);
    if (GL_FALSE == successfully_compiled)
    {
        GLint log_size = 0;
        glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &log_size);

        GLchar* error_log = (GLchar*) calloc((unsigned int)log_size, sizeof(GLchar));

        glGetShaderInfoLog(fragment_shader_id, log_size, &log_size, error_log);
        fprintf(stderr, "ERROR: Failed to compile fragment shader.\n");
        fprintf(stderr, "LOG: %s\n", error_log);
        free(error_log);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        return 0;
    }

    GLuint shader_program_id = glCreateProgram();
    if (0 == shader_program_id)
    {
        fprintf(stderr, "ERROR: Failed to create shader program.\n");
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        return 0;
    }

    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);

    glLinkProgram(shader_program_id);
    GLint successfully_linked = 0;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &successfully_linked);
    if (GL_FALSE == successfully_linked)
    {
        GLint log_size = 0;
        glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &log_size);

        GLchar* error_log = (GLchar*) calloc((unsigned int)log_size, sizeof(GLchar));

        glGetProgramInfoLog(shader_program_id, log_size, &log_size, error_log);
        fprintf(stderr, "ERROR: Failed to link shader program.\n");
        fprintf(stderr, "LOG: %s\n", error_log);
        free(error_log);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        glDeleteProgram(shader_program_id);
        return 0;
    }

    return shader_program_id;
}
