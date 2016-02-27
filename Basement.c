#include "Shaders.h"
#include "My_math_utils.h"
#include "Load_utils.h"
#include "View_matrix_utils.h"
#include "Meshes.h"
#include "Physics.h"

#include "GL/gl3w.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


static void Error_callback_for_glfw(int error, const char* description)
{
    fprintf(stderr, "ERROR #%d: %s\n", error, description);
}

static void Key_callback_for_glfw(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (NULL == window)
    {
        ;
    }
    scancode = 0;
    mods = 0;

    if (GLFW_KEY_ESCAPE == key && GLFW_RELEASE == action)
    {
        glfwSetWindowShouldClose(window, 1);
    }
    if (GLFW_KEY_UP == key && GLFW_PRESS == action)
    {
    }
    if (GLFW_KEY_DOWN == key && GLFW_PRESS == action)
    {
    }
}

static void Framebuffer_size_callback_for_glfw(GLFWwindow* window, int width, int height)
{
    if (NULL == window)
    {
        ;
    }

    glViewport(0, 0, width, height);
}

static void Mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (NULL == window)
    {
        ;
    }
    mods = 0;

    if (GLFW_MOUSE_BUTTON_LEFT == button && GLFW_PRESS == action)
    {
    }
    if (GLFW_MOUSE_BUTTON_RIGHT == button && GLFW_PRESS == action)
    {
    }
}

static GLFWwindow* Initializing_glfw()
{
    glfwSetErrorCallback(Error_callback_for_glfw);

    if(GL_FALSE == glfwInit())
    {
        fprintf(stderr, "ERROR: Failed to init glfw.\n");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    int start_width = 1024;
    int start_height = 768;
    GLFWwindow* window = glfwCreateWindow(start_width, start_height, "My window name", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "ERROR: Failed to create a window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, Key_callback_for_glfw);

    glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback_for_glfw);

    glfwSetMouseButtonCallback(window, Mouse_button_callback);

    glfwSwapInterval(1);

    return window;
}

static void Initializing_gl3w()
{
    if (gl3wInit())
    {
        fprintf(stderr, "ERROR: Failed to initialize OpenGL extensions loading library.\n");
        exit(EXIT_FAILURE);
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("Max GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

static void Initializing_opengl()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

int main(void)
{
    GLFWwindow* my_window = Initializing_glfw();
    if (NULL == my_window)
    {
        return -1;
    }
    Initializing_gl3w();
    Initializing_opengl();
    printf("Libraries loaded.\n");
////////////////////////////////////////////////////////////////////////////////

    Initialize_mesh_struct();

    struct loaded_model* rc2 = Load_model_ff_and_name_it("objects/Cube3.obj", "First cube");
    if (NULL == rc2)
    {
        fprintf(stderr, "ERROR: Failed to load model from file.\n");
        return -1;
    }

    printf("Meshes loaded.\n");
////////////////////////////////////////////////////////////////////////////////

    GLuint main_shdr_program_id = Make_program_from_shaders("shaders/main_shader.vert", "shaders/main_shader.frag");
    if (0 == main_shdr_program_id)
    {
        return -1;
    }
    glUseProgram(main_shdr_program_id);
    printf("Shaders loaded.\n");
////////////////////////////////////////////////////////////////////////////////

    int model_matrix_loc = glGetUniformLocation(main_shdr_program_id, "model_matrix");

    Set_view_matrix_uniform_location( glGetUniformLocation(main_shdr_program_id, "view_matrix") );
    Initialize_camera_struct();       //My own invention
    Translate_camera((vec4_type){0.0f, 0.0f, -4.0f, 0.0f});

    int projection_matrix_loc = glGetUniformLocation(main_shdr_program_id, "projection_matrix");
    int old_window_width = 0;
    int old_window_height = 0;
    glfwGetWindowSize(my_window, &old_window_width, &old_window_height);
    struct mat4_type projection_matrix = Create_perspective_projection_mat(60.0f, old_window_width / (float) old_window_height, 1.0f, 50.0f);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, &projection_matrix.col[0].x);
    printf("Matrices loaded.\n");
////////////////////////////////////////////////////////////////////////////////

    Initialize_physics_struct();

    physics_object* rc3 = Add_phys_obj(Get_model_pointer_by_name("First cube"));
    if (NULL == rc3)
    {
        fprintf(stderr, "ERROR: Failed to load phys object.\n");
        return -1;
    }

    printf("Physics loaded.\n");
////////////////////////////////////////////////////////////////////////////////

    Handle_OGL_errors("ERROR: Failure during initialization.\n");
    printf("Game cycle started.\n");

    while(!glfwWindowShouldClose(my_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        struct loaded_model* model = Get_model_pointer_by_name("First cube");
        glBindVertexArray(model->vao_id);
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model->links_to_phys_objects[0]->model_matrix.col[0].x);
        glDrawElements(GL_TRIANGLES, model->num_of_indices, GL_UNSIGNED_INT, (GLvoid*) 0);


        Physics_cycle();
////////////////////////////////////////////////////////////////////////////////

// GLuint query;
// glGenQueries(1, &query);
// GLuint64 time_elapsed = 0;
// GLuint64 time_summ = 0;
// short frame_count = 0;
//
// glBeginQuery(GL_TIME_ELAPSED, query);
//
// glEndQuery(GL_TIME_ELAPSED);
//
// glGetQueryObjectui64v(query, GL_QUERY_RESULT, &time_elapsed);
//
// frame_count++;
// time_summ += time_elapsed;
//
// if (frame_count >= 60)
// {
//     printf("Average ms per frame: %f\n", time_summ / (60 * 1e6f));
//     frame_count = 0;
//     time_summ = 0;
// }
////////////////////////////////////////////////////////////////////////////////

        int win_wid = 0;
        int win_hei = 0;
        glfwGetWindowSize(my_window, &win_wid, &win_hei);
        if (old_window_width != win_wid || old_window_height != win_hei)
        {
            projection_matrix = Create_perspective_projection_mat(60.0f, win_wid / (float) win_hei, 1.0f, 100.0f);
            glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, &projection_matrix.col[0].x);

            old_window_width = win_wid;
            old_window_height = win_hei;
        }

        Handle_OGL_errors("ERROR: Problems with rendering cycle.\n");

        glfwSwapBuffers(my_window);

        glfwPollEvents();
    }
////////////////////////////////////////////////////////////////////////////////

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
    //
    // glDisableVertexArrayAttrib(vao_id, vertex_pos_location);
    // glDeleteBuffers(1, &vbo_id);
    // glDeleteBuffers(1, &index_buff_id);
    // glDeleteVertexArrays(1, &vao_id);
    //
    glUseProgram(0);
    Delete_program_and_shaders(main_shdr_program_id);

    Handle_OGL_errors("ERROR: Failed to destroy vbo or vao or shaders.\n");

    glfwTerminate();
    return 0;
}
