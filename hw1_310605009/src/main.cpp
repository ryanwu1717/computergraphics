
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>

#include "camera/quat_camera.h"
#include "context_manager.h"
#include "shape/cube.h"

using namespace glm;

// Unnamed namespace for global variables
namespace {
std::vector<std::unique_ptr<graphics::shape::Cube>> cubes;
}  // namespace

void keyCallback(GLFWwindow* window, int key, int original_front, int action, int mode) {
  // There are three actions: press, release, hold
  if (action != GLFW_PRESS) return;
  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }
  // Check all cubes are not rotating
  bool idle = std::all_of(cubes.begin(), cubes.end(), [](const auto& cube) { return cube->isIdle(); });
  if (!idle) return;
  // There are three Axis: X, Y, Z
  using Axis = graphics::shape::Axis;
  
    for (auto& cube : cubes) {
      switch (key) { 
         case 'U':
            cube->rotate(Axis::X);
          break;
        case 'R':    
          if (cube->getPosition(Axis::X) == 1) {
              
            cube->rotate(Axis::X);
          }
          break;
        case 'F':
          if (cube->getPosition(Axis::X) == 0) {
            cube->rotate(Axis::X);
          }
          break;
        case 'V':
          if (cube->getPosition(Axis::X) == -1) {
            cube->rotate(Axis::X);
          }
          break;
        case 'J':
          cube->rotate(Axis::Y);
          break;
        case 'T':
          if (cube->getPosition(Axis::Y) == 1) {
            cube->rotate(Axis::Y);
          }
          break;
        case 'G':
          if (cube->getPosition(Axis::Y) == 0) {
            cube->rotate(Axis::Y);
          }
          break;
        case 'B':
          if (cube->getPosition(Axis::Y) == -1) {
            cube->rotate(Axis::Y);
          }
          break;
        case 'M':
          cube->rotate(Axis::Z);
          break;
        case 'Y':
          if (cube->getPosition(Axis::Z) == 1) {
            cube->rotate(Axis::Z);
          }
          break;
        case 'H':
          if (cube->getPosition(Axis::Z) == 0) {
            cube->rotate(Axis::Z);
          }
          break;
        case 'N':
          if (cube->getPosition(Axis::Z) == -1) {
            cube->rotate(Axis::Z);
          }
          break;
      }
    }
   
  
  // TODO: Detect key-events, perform rotation
  //       1. Use switch/case to find the key you want.
  //       2. Iterate through all cubes, perform rotation on those you want to rotate.
  // Hint:
  //       1. You can get the cube's layer by calling getPosition with Axis ( cube->getPosition(Axis::X) ),
  //          the return value can be {-1, 0, 1}, representing three layers
  //       2. You can rotate it by calling rotate with Axis ( cube->rotate(Axis::X) )
  //       3. glfw3's key list (https://www.glfw.org/docs/3.3/group__keys.html)
  // Note:
  //       You should finish your cube rendering first.
  //       Otherwise you will spend a lot of time debugging this with a black screen.
  
}

void resizeCallback(GLFWwindow* window, int width, int height) {
  OpenGLContext::framebufferResizeCallback(window, width, height);
  auto ptr = static_cast<graphics::camera::Camera*>(glfwGetWindowUserPointer(window));
  if (ptr != nullptr) {
    ptr->updateProjection(OpenGLContext::getAspectRatio());
  }
}

int main() {
 
  // Initialize OpenGL context, details are wrapped in class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  OpenGLContext::createContext(43, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();
  glfwSetWindowTitle(window, "HW1");
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // This is useful if you want to debug your OpenGL API calls.
  OpenGLContext::enableDebugCallback();
#endif
  // Setup camera.
  graphics::camera::QuaternionCamera camera(glm::vec3(0, 0, 15));
  camera.initialize(OpenGLContext::getAspectRatio());
  glfwSetWindowUserPointer(window, &camera);
  // Generate all mini-cubes
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      for (int k = -1; k <= 1; ++k) {
        cubes.emplace_back(std::make_unique<graphics::shape::Cube>(glm::vec3(i, j, k)));
        //cubes-> draw(color(0.502f, 0.0f, 0.502f, 1.0f));
      }
    }
  }
  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    glDisable(GL_CULL_FACE);
    // Polling events.
    glfwPollEvents();
    camera.move(window);
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera.getProjectionMatrix());
    // ModelView Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(camera.getViewMatrix());
 
    /*for (int i = 0; i < cubes.size(); i++) {
      graphics::shape::Cube& cube = *cubes[i];
      glPushMatrix();
      cube->setupModel();
      cube->draw();
      glPopMatrix();
    }*/
    for (auto& cube : cubes)  // access by reference to avoid copying
    {
      glPushMatrix();
      cube->setupModel();
      
      cube->draw();
      glPopMatrix();
    }

    // TODO: Render Rubik's cube
    //       1. Iterate through all cube in cubes.
    //       2. For each cube, call cube->setupModel() and cube->draw().
    //       3. You can use glPushMatrix, glPopMatrix to 'cache' the current matrix.
    // Hint:
    //       glPushMatrix/glPopMatrix   (https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml)
    // Note:
    //       You can implement this section using less than 10 lines of code.
#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  return 0;
}
