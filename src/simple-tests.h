#ifndef __SIMPLE_TESTS_H__
#define __SIMPLE_TESTS_H__

/**
 * This file is meant to do a simple parsing of the args
 * to check if we have:
 * - "--test": to print the self test "Hello tests"
 * - TODO: "--test first-window": to play with glfw, opengl, ...
 * - TODO: and more stuff that I will forget to document
 */

// Disable clang-format because we must include glad before GLFW
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string.h>

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/*
 * Global variables for the camera
 */
glm::vec3 global_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 global_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 global_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool global_firstMouseInteraction = true;
float global_yaw =
    -90.0f;  // yaw is initialized to -90.0 degrees since a yaw of 0.0 results
             // in a direction vector pointing to the right so we initially
             // rotate a bit to the left.
float global_pitch = 0.0f;
float global_lastX = 800.0f / 2.0f;
float global_lastY = 600.0f / 2.0f;
float global_fov = 45.0f;

// CBR: to draw different things dynamically with keyboard pressed
bool global_cbrFlag = true;
glm::vec3 global_cbrHorizontalOffset = glm::vec3(0.0f, 0.0f, 0.0f);

/**
 * Some timing variables to stabilize camera speed
 */
float global_deltaTime = 0.0f;  // Time between current frame and last framce
float global_lastFrame = 0.0f;

/**
 * A simple helper class to parse arguments and run some tests
 */
class SimpleTests {
 public:
  /**
   * Constants
   */
  static const int MIN_ARG_FOR_TEST = 2;
  static constexpr const char* TEST_ARG = "--test";
  static constexpr const char* FIRST_WINDOW_TEST_ARG = "first-window";

  /**
   * Check if arguments from main function include "--test"
   * @param argc the argc from main function
   * @param argv the argv from main function
   * @return true if "--test" was found in arg, false otherwise
   */
  static bool ArgHasTest(int argc, char** argv) {
    if (argc < MIN_ARG_FOR_TEST) {
      return false;
    }

    if (strcmp(argv[1], TEST_ARG) == 0) {
      return true;
    }
    return false;
  }

  /**
   * Parse argv & argc to determine which test to run
   * @param argc the argc from main function
   * @param argv the argv from main function
   * @return 0 if a test was run, -1 if an error occured or no test found
   */
  static int ParseArgAndRun(int argc, char** argv) {
    if (argc < MIN_ARG_FOR_TEST) {
      // This function should not be called without enough arguments
      return -1;
    }
    // Check if we ONLY have "--test" & run SelfTest()
    if (argc == MIN_ARG_FOR_TEST && strcmp(argv[1], TEST_ARG) == 0) {
      SelfTest();
      return 0;
    }
    // Refactor needed if we add more tests
    // Check if we ONLY have "--test first-window"
    if (argc > MIN_ARG_FOR_TEST && strcmp(argv[1], TEST_ARG) == 0 &&
        strcmp(argv[2], FIRST_WINDOW_TEST_ARG) == 0) {
      return FirstWindowTest();
    }

    return -1;
  }

 private:
  /**
   * Run the self test to print "Hello tests"
   */
  static void SelfTest() { std::cout << "Hello tests" << std::endl; }

  /**
   * Run the first window test to print "Hello first window"
   */
  static int FirstWindowTest() {
    int initialized = glfwInit();
    if (initialized != GLFW_TRUE) {
      std::cout << "Failed to glfwInit" << std::endl;
      return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For Apple
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "FirstWindow", NULL, NULL);
    if (window == NULL) {
      std::cout << "Failed to glfwCreateWindow" << std::endl;
      glfwTerminate();
      return -1;
    }
    glfwMakeContextCurrent(window);

    GLFWframebuffersizefun callback = glfwSetFramebufferSizeCallback(
        window, SimpleTests::framebuffer_size_callback);
    if (callback == NULL) {
      // Only warning for now, since it seems to be working anyway
      std::cout << "Warning: glfwSetFramebufferSizeCallback: " << callback
                << std::endl;
    }

    glfwSetCursorPosCallback(window, SimpleTests::mouse_callback);
    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to gladLoadGLLoader" << std::endl;
      glfwTerminate();
      return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Shaders
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &SimpleTests::vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for shader compile erros
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "Failed to compile vertex shader:\n" << infoLog << std::endl;
    }
    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &SimpleTests::fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for shader compile erros
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "Failed to compile fragment shader:\n"
                << infoLog << std::endl;
    }
    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "Failed to link shaders:\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // clang-format off
    // Set up vertex data
    float vertices[] = {
      // Top rectangle
      // first triangle
      0.5f, 0.5f, 0.1f,   // top right
      0.5f, -0.5f, 0.1f,  // bottom right
      -0.5f, 0.5f, 0.1f,  // top left
      // second triangle
      0.5f, -0.5f, 0.1f,   // bottom right
      -0.5f, -0.5f, 0.1f,  // bottom left
      -0.5f, 0.5f, 0.1f,    // top left

      // Front rectangle
      -0.5f, -0.5f, 0.1f,
      -0.5f, -0.5f, -0.1f,
      0.5f, -0.5f, 0.1f,

      -0.5f, -0.5f, -0.1f,
      0.5f, -0.5f, -0.1f,
      0.5f, -0.5f, 0.1f,

      // Left side rectangle
      -0.5f, -0.5f, 0.1f,
      -0.5f, -0.5f, -0.1f,
      -0.5f, 0.5f, 0.1f,

      -0.5f, -0.5f, -0.1f,
      -0.5f, 0.5f, -0.1f,
      -0.5f, 0.5f, 0.1f,

      // Right side rectangle
      0.5f, -0.5f, 0.1f,
      0.5f, -0.5f, -0.1f,
      0.5f, 0.5f, 0.1f,

      0.5f, -0.5f, -0.1f,
      0.5f, 0.5f, -0.1f,
      0.5f, 0.5f, 0.1f,

      // Back rectangle
      0.5f, 0.5f, 0.1f,
      0.5f, 0.5f, -0.1f,
      -0.5f, 0.5f, 0.1f,

      0.5f, 0.5f, -0.1f,
      -0.5f, 0.5f, -0.1f,
      -0.5f, 0.5f, 0.1f,

      // No Bottom rectangle
    };
    // clang-format on

    unsigned int numberOfTiles = 5;
    glm::vec3 tilePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
    };

    glm::vec3 tilePositionsOther[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(3.0f, 0.0f, 0.0f),
        glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(0.0f, -3.0f, 0.0f),
    };

    // Buffer(s) and configure vertex attributes
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // CBR wants to only loop for 3 seconds
    auto start = std::chrono::system_clock::now();
    std::cout << "Hello first window" << std::endl;
    auto end = std::chrono::system_clock::now();
    auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    while (!glfwWindowShouldClose(window) && elapsed_seconds < 3) {
      // while (!glfwWindowShouldClose(window)) {
      // Logic to compute per-frame time
      float currentFrame = static_cast<float>(glfwGetTime());
      global_deltaTime = currentFrame - global_lastFrame;
      global_lastFrame = currentFrame;

      SimpleTests::processInput(window);

      glClearColor(0.3f, 0.4f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Activate shader
      glUseProgram(shaderProgram);
      // Create transformations matrix
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = glm::mat4(1.0f);
      view =
          glm::lookAt(global_cameraPos, global_cameraPos + global_cameraFront,
                      global_cameraUp);
      glm::mat4 projection = glm::mat4(1.0f);
      // Rotation model
      // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
      //                     glm::vec3(0.5f, 1.0f, 0.0f));
      // Static model
      // model =
      //     glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f,
      //     0.0f));
      // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      projection = glm::perspective(glm::radians(global_fov),
                                    (float)800.0 / (float)600.0, 0.1f, 100.0f);
      // Pass matrix to shaders
      unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
      unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
      unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
      // Render
      glBindVertexArray(VAO);

      // Debug: when I print "x" it seems to range from 100/-100
      // But once I comment this out, my check seems to need 1000/-1000
      // instead... std::cout << "Horizontal offset: x = " <<
      // global_cbrHorizontalOffset.x
      //           << " / y = " << global_cbrHorizontalOffset.y
      //           << " / z = " << global_cbrHorizontalOffset.z << std::endl;

      if (global_cbrHorizontalOffset.x > 1000.0 ||
          global_cbrHorizontalOffset.x < -1000.0) {
        global_cbrFlag = false;
      } else {
        global_cbrFlag = true;
      }

      for (unsigned int i = 0; i < numberOfTiles; i++) {
        model = glm::mat4(1.0f);

        // if (global_cbrFlag) {
        //   model = glm::rotate(model, glm::radians(-75.0f),
        //                       glm::vec3(1.0f, 0.0f, 0.0f));
        // } else {
        //   model =
        //       glm::rotate(model, (float)glfwGetTime() * glm::radians(-75.0f),
        //                   glm::vec3(1.0f, 0.0f, 0.0f));
        // }

        model = glm::rotate(model, glm::radians(-75.0f),
                            glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(
            model, global_cbrFlag ? tilePositions[i] : tilePositionsOther[i]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 6 * 5);
      }

      glfwSwapBuffers(window);
      glfwPollEvents();

      // End of rendering, for testing we only run it for 3 seconds
      end = std::chrono::system_clock::now();
      elapsed_seconds =
          std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
  }

  /**
   * Callback for glfwSetFramebufferSizeCallback
   */
  static void framebuffer_size_callback(GLFWwindow* window, int width,
                                        int height) {
    // std::cout << "DEBUG: framebuffer_size_callback " << width << "/" <<
    // height << std::endl;
    glViewport(0, 0, width, height);
  }

  /**
   * Helper function to check if any key/input has been pressed by users
   */
  static void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed =
        static_cast<float>(2.5 * global_deltaTime);  // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      global_cameraPos += cameraSpeed * global_cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      global_cameraPos -= cameraSpeed * global_cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      // CBR keep track of x position
      global_cbrHorizontalOffset -=
          glm::normalize(glm::cross(global_cameraFront, global_cameraUp));
      global_cameraPos -=
          glm::normalize(glm::cross(global_cameraFront, global_cameraUp)) *
          cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      // CBR keep track of x position
      global_cbrHorizontalOffset +=
          glm::normalize(glm::cross(global_cameraFront, global_cameraUp));
      global_cameraPos +=
          glm::normalize(glm::cross(global_cameraFront, global_cameraUp)) *
          cameraSpeed;
    }
  }

  /**
   * Callback for mouse positioning
   */
  static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (global_firstMouseInteraction) {
      global_lastX = xpos;
      global_lastY = ypos;
      global_firstMouseInteraction = false;
    }

    float xoffset = xpos - global_lastX;
    float yoffset = global_lastY -
                    ypos;  // Reversed since y-coordinates go from bottom to top
    global_lastX = xpos;
    global_lastY = ypos;

    float sensitivity = 0.1f;  // Change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    global_yaw += xoffset;
    global_pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (global_pitch > 89.0f) global_pitch = 89.0f;
    if (global_pitch < -89.0f) global_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(global_yaw)) * cos(glm::radians(global_pitch));
    front.y = sin(glm::radians(global_pitch));
    front.z = sin(glm::radians(global_yaw)) * cos(glm::radians(global_pitch));
    global_cameraFront = glm::normalize(front);
  }

  /**
   * Vertex shader source
   */
  static constexpr const char* vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "uniform mat4 model;\n"
      "uniform mat4 view;\n"
      "uniform mat4 projection;\n"
      "out vec3 posVec;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
      "   posVec = aPos;\n"
      "}\0";
  /**
   * Fragment shader source
   */
  static constexpr const char* fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "in vec3 posVec;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) * vec4(posVec, 1.0f);\n"
      "}\n\0";
};

#endif
