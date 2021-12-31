#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STRIDE 7

static GLuint program;
static GLuint vertexShader;
static GLuint geometryShader;
static GLuint fragmentShader;

static GLuint createShader(char const* fileName, GLenum type) {
  FILE* file; fopen_s(&file, fileName, "r");
  fseek(file, 0, SEEK_END);
  long length = ftell(file) + 1l;
  rewind(file);
  GLchar* source = calloc(length, sizeof(GLchar));
  fread(source, sizeof(GLchar), length, file);
  fclose(file);

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  free(source);
  glCompileShader(shader);
  GLint status; glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  return shader;
}

static GLuint vbo;
static GLuint vao;

static void attribPointer(GLchar const* name, GLint size) {
  GLint attrib = glGetAttribLocation(program, name);
  static GLint pointer = 0;
  glVertexAttribPointer(attrib, size, GL_FLOAT, GL_FALSE, STRIDE * sizeof(GLfloat), (void*)(pointer * sizeof(GLfloat)));
  glEnableVertexAttribArray(attrib);
  pointer += size;
}

static float color[3];

void setColor(float r, float g, float b) {
  color[0] = r;
  color[1] = g;
  color[2] = b;
}

void prepareDrawing(void) {
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  program = glCreateProgram();
  vertexShader = createShader("back/main.vert", GL_VERTEX_SHADER);
  geometryShader = createShader("back/main.geom", GL_GEOMETRY_SHADER);
  fragmentShader = createShader("back/main.frag", GL_FRAGMENT_SHADER);
  glAttachShader(program, vertexShader);
  glAttachShader(program, geometryShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glUseProgram(program);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  attribPointer("position", 2);
  attribPointer("size", 1);
  attribPointer("is_circle", 1);
  attribPointer("color", 3);

  setColor(1.0f, 1.0f, 1.0f);
}

static GLfloat* vertices = NULL;
static GLint verticesN = 0;
static GLint lastVerticesN;

void eraseDrawing(void) {
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  vertices = realloc(vertices, 0);
  lastVerticesN = verticesN;
  verticesN = 0;
}

void addShape(float x, float y, float size, float isCircle) {
  int i = verticesN;
  verticesN += STRIDE;
  vertices = realloc(vertices, verticesN * sizeof(GLfloat));
  vertices[i] = x;
  vertices[i + 1] = y;
  vertices[i + 2] = size;
  vertices[i + 3] = isCircle;
  vertices[i + 4] = color[0];
  vertices[i + 5] = color[1];
  vertices[i + 6] = color[2];
}

void presentDrawing(void) {
  verticesN != lastVerticesN ?
  glBufferData(GL_ARRAY_BUFFER, verticesN * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW) :
  glBufferSubData(GL_ARRAY_BUFFER, 0, verticesN * sizeof(GLfloat), vertices);

  glDrawArrays(GL_POINTS, 0, verticesN / STRIDE);
}

void scrapDrawing(void) {
  glDeleteProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}