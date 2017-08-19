#include <stdio.h>
#include <GLFW/glfw3.h>
#include <include/diamond/all.hpp>
#include <iostream>

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;


std::string vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
std::string fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glewInit();


    // create shaders
    dgl::shader frag = dgl::shader_class::create(GL_FRAGMENT_SHADER);
    frag->source(fragmentShaderSource);
    frag->compile();
    if (!frag->get_val<int>(GL_COMPILE_STATUS)) {
        std::cerr << frag->info_log() << std::endl;
    }

    dgl::shader vert = dgl::shader_class::create(GL_VERTEX_SHADER);
    vert->source(vertexShaderSource);
    vert->compile();
    if (!vert->get_val<int>(GL_COMPILE_STATUS)) {
        std::cerr << vert->info_log() << std::endl;
    }

    dgl::program program = dgl::program_class::create();
    program->attach(vert);
    program->attach(frag);
    program->link();


    // create vertices and buffer
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
        0.5f, -0.5f, 0.0f, // right 
        0.0f,  0.5f, 0.0f  // top   
    };

    dgl::buffer vbo = dgl::buffer_class::create();
    vbo->data(sizeof(vertices), vertices);


    // create VAO
    dgl::vertex_array vao = dgl::vertex_array_class::create();

    auto binding = vao->create_binding(0);
    binding->vertex_buffer(vbo, 0, sizeof(float)*3);

    auto attribute = vao->create_attribute(0);
    attribute->attrib_format(3, GL_FLOAT, GL_FALSE);
    attribute->binding(binding);


    // program uniform constant example
    auto uniform = program->get_uniform(0u);
    uniform->set<int>(0);


    // SSBO binding example 
    auto ssbo_binding = dgl::buffer_target::shader_storage.create_binding(0); // get SSBO binding by ID
    ssbo_binding->bind(vbo);


    // create texture
    auto texture = dgl::texture_target::t_2d.create();
    texture->storage(1, GL_RGBA32F, glm::uvec2(1, 1)); // 2d texture
    texture->parameter_val<int>(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->parameter_val<int>(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->parameter_val<int>(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->parameter_val<int>(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    // create GL texture binding
    auto textureBinding = dgl::texture_binding_class::create(0);
    textureBinding->bind_texture(texture);



    // drawing
    while (!glfwWindowShouldClose(window))
    {
        // managment
        dgl::managment::program::use(program);
        dgl::managment::vertex_array::bind(vao);

        // states
        dgl::state::clear::color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        dgl::state::clear::depth(1.f);

        // commands
        dgl::commands::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        dgl::commands::draw::arrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}