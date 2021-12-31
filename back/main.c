#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <fe.h>
#include "window.h"
#include "drawing.h"

static fe_Object* f_random(fe_Context* ctx, fe_Object* args) {
  int lower = (int)fe_tonumber(ctx, fe_nextarg(ctx, &args));
  int upper = (int)fe_tonumber(ctx, fe_nextarg(ctx, &args));
  int n = (rand() % (upper - lower + 1)) + lower;
  return fe_number(ctx, (float)n);
}

static fe_Object* f_color(fe_Context* ctx, fe_Object* args) {
  float r = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  float g = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  float b = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  setColor(r, g, b);

  return fe_bool(ctx, 0);
}

static fe_Object* f_square(fe_Context* ctx, fe_Object* args) {
  float x = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  float y = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  float size = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  addShape(x, y, size, 0.0f);

  return fe_bool(ctx, 0);
}

static fe_Object* f_circle(fe_Context* ctx, fe_Object* args) {
  float x = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  float y = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  float size = fe_tonumber(ctx, fe_nextarg(ctx, &args));
  addShape(x, y, size, 1.0f);

  return fe_bool(ctx, 0);
}

int main(void) {
  srand((unsigned int)time(0));

  openWindow();
  prepareDrawing();

  int size = 1024 * 1024;
  void* data = malloc(size);
  fe_Context* ctx = fe_open(data, size);
  int gc = fe_savegc(ctx);

  fe_set(ctx, fe_symbol(ctx, "random"), fe_cfunc(ctx, f_random));
  fe_set(ctx, fe_symbol(ctx, "color"), fe_cfunc(ctx, f_color));
  fe_set(ctx, fe_symbol(ctx, "square"), fe_cfunc(ctx, f_square));
  fe_set(ctx, fe_symbol(ctx, "circle"), fe_cfunc(ctx, f_circle));

  FILE* fp; fopen_s(&fp, "snake.fe", "r");
  fe_Object* load = NULL;
  fe_Object* input = NULL;
  fe_Object* tick = NULL;
  fe_Object* draw = NULL;
  for (;;) {
    fe_Object* obj = fe_readfp(ctx, fp);
    if (!obj) { break; }
    fe_eval(ctx, obj);
    fe_restoregc(ctx, gc);

    fe_Object* sym = fe_car(ctx, fe_cdr(ctx, obj));
    char* symStr = calloc(6, sizeof(char));
    fe_tostring(ctx, sym, symStr, 6);
    if (!strcmp(symStr, "load")) { load = sym; }
    else if (!strcmp(symStr, "input")) { input = sym; }
    else if (!strcmp(symStr, "tick")) { tick = sym; }
    else if (!strcmp(symStr, "draw")) { draw = sym; }
    free(symStr);
  }

  fe_eval(ctx, fe_list(ctx, &load, 1));
  fe_restoregc(ctx, gc);

  double const tickTime = 0.1;
  double tickTimer = tickTime;

  while (windowIsOpen) {
    for (;;) {
      char const* key = pollKeys();
      if (!key) { break; }
      fe_Object* objs[2] = {input, fe_string(ctx, key)};
      fe_eval(ctx, fe_list(ctx, objs, 2));
      fe_restoregc(ctx, gc);
    }

    double deltaTime = getDeltaTime();
    tickTimer -= deltaTime;
    if (tickTimer <= 0) {
      fe_eval(ctx, fe_list(ctx, &tick, 1));
      fe_restoregc(ctx, gc);
      tickTimer = tickTime;
    }

    eraseDrawing();
    fe_eval(ctx, fe_list(ctx, &draw, 1));
    fe_restoregc(ctx, gc);
    presentDrawing();

    updateWindow();
  }

  scrapDrawing();
  closeWindow();

  return 0;
}