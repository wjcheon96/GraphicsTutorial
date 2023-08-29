#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

int main() {

    // glfwInit();
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SPDLOG_INFO("initialize glfw");
    if (!glfwInit()) {
        const char* description = NULL;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(600, 400, "scop", NULL, NULL);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}


// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <iostream>

// int main() {

//     std::cout << "hello world" << std::endl;

//     // Initialize GLFW
//     glfwInit();

//     // Tell GLFW what version of OpenGL we are using
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);`
//     // Tell GLFW we are using the CORE profile
//     // So that means we only have the modern function
//     // Tell it that we don't use the useless function of the lower version.
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     // When making GLFW window, without this window hint, doesn't run when using Mac OS
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

//     // Create a GLFW window object. width, height, name
//     GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
//     if (window == NULL) {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }

//     // Introduce the window into the current context
//     glfwMakeContextCurrent(window);

//     // Load GLAD so it configures OpenGL
//     gladLoadGL();

//     // Set the ViewPoint. Tell OpenGL the size of the window.
//     glViewport(0, 0, 800, 800);

//     // Specify the color of the background
//     glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
//     // Clean the back buffer and assign the new color to it. GL_COLOR_BUFFER_BIT means the buffer which is used for write the color
//     glClear(GL_COLOR_BUFFER_BIT);

//     // Swap the back buffer with the front buffer.
//     glfwSwapBuffers(window);

//     // Main loop
//     while (!glfwWindowShouldClose(window)) {
//         // take car of all GLFW Events
//         glfwPollEvents();
//     }

//     // Delete the window before ending the program.
//     glfwDestroyWindow(window);
//     // Terminate GLFW before ending the program.
//     glfwTerminate();

//     return 0;
// }