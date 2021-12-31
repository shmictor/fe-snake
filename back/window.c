#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "window.h"

int windowIsOpen = 0;

static GLFWwindow* window;

static int* keyStack;
static size_t keyStackN;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    ++keyStackN;
    keyStack = realloc(keyStack, keyStackN * sizeof(int));
    keyStack[keyStackN - 1] = key;
  }
}

void openWindow(void) {
  glfwInit();

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  window = glfwCreateWindow(512, 512, "snake", NULL, NULL);
  glfwSetKeyCallback(window, keyCallback);
  glfwMakeContextCurrent(window);
  windowIsOpen = 1;

  glfwSwapInterval(1);
}

static double oldTime = 0;

double getDeltaTime(void) {
  double newTime = glfwGetTime();
  double deltaTime = newTime - oldTime;
  oldTime = newTime;
  return deltaTime;
}

char const* pollKeys(void) {
  char const* key = NULL;
  if (keyStackN) {
    switch (keyStack[keyStackN - 1]) {
      case GLFW_KEY_RIGHT:
        key = "right";
        break;
      case GLFW_KEY_LEFT:
        key = "left";
        break;
      case GLFW_KEY_UP:
        key = "up";
        break;
      case GLFW_KEY_DOWN:
        key = "down";
        break;
      case GLFW_KEY_SPACE:
        key = "space";
        break;
      default:
        key = glfwGetKeyName(keyStack[keyStackN - 1], 0);
    }
    --keyStackN;
    keyStack = realloc(keyStack, keyStackN * sizeof(int));
  }
  return key;
}

void updateWindow(void) {
  glfwSwapBuffers(window);
  glfwPollEvents();
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  windowIsOpen = !glfwWindowShouldClose(window);
}

void closeWindow(void) {
  free(keyStack);
  glfwDestroyWindow(window);
  glfwTerminate();
}