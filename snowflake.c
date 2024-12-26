#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to load and compile shaders
GLuint loadShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
    return shader;
}

// Function to create shader program
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main() {\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    TexCoord = aTexCoord;\n"
"}\n";

const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform float time;\n"
"uniform vec2 resolution;\n"
"float snakePattern(vec2 uv) {\n"
"    float scale = 10.0;\n"
"    uv *= scale;\n"
"    vec2 p = floor(uv);\n"
"    vec2 f = fract(uv);\n"
"    float snake = 0.0;\n"
"    snake += sin((p.x + p.y) * 10.0 + time) * 0.5 + 0.5;\n"
"    snake += sin(f.x * 10.0 + time) * 0.5 + 0.5;\n"
"    snake += sin(f.y * 10.0 + time) * 0.5 + 0.5;\n"
"    return clamp(snake, 0.0, 1.0);\n"
"}\n"
"void main() {\n"
"    vec2 uv = TexCoord.xy / resolution.xy;\n"
"    uv.y = 1.0 - uv.y;\n"
"    float pattern = snakePattern(uv);\n"
"    vec3 color = vec3(pattern);\n"
"    FragColor = vec4(color, 1.0);\n"
"}\n";

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 800, "Snakeflake Shader", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    float vertices[] = {
        // positions      // texture coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        glUniform1f(glGetUniformLocation(shaderProgram, "time"), timeValue);
        glUniform2f(glGetUniformLocation(shaderProgram, "resolution"), 800.0f, 800.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
