#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct color {
  int r, g, b;
} color;

// read color RGB from hex
void color_from_hex(color *c, uint32_t hex) {
  c->r = (hex >> 16) & 0xFF;
  c->g = (hex >> 8) & 0xFF;
  c->b = hex & 0xFF;
}

// convert color to hex
uint32_t color_to_hex(color *c) { return (c->r << 16) | (c->g << 8) | c->b; }

// c3 = (c1 * y + c2 * x) / (x + y)
void cal_color(color *c3, color *c1, color *c2, int x, int y) {
  c3->r = (c1->r * y + c2->r * x) / (x + y);
  c3->g = (c1->g * y + c2->g * x) / (x + y);
  c3->b = (c1->b * y + c2->b * x) / (x + y);
}

color color_list[8];
color now_color;
int start_i = 0;
int end_i = 1;
uint64_t interval = 500; // units: 10ms
uint64_t start_time = 0;
uint64_t now_time = 0;
int is_speed_up = 0;

void draw(color *c) {
  // printf("Start draw #%06X\n", color_to_hex(c));
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
  int x, y;
  int hex = color_to_hex(c);
  for (x = 0; x < w; x++)
    for (y = 0; y < h; y++)
      io_write(AM_GPU_FBDRAW, x, y, &hex, 1, 1, false);
  io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
}

void update_color() {
  if (io_read(AM_TIMER_UPTIME).us / 10000 == now_time) {
    return;
  }
  now_time = io_read(AM_TIMER_UPTIME).us / 10000;
  int x = now_time - start_time;
  int y = interval - x;
  if (x > interval) {
    draw(&color_list[end_i]);
    start_i = end_i;
    end_i = (end_i + (rand() % 6) + 1) % 8;
    start_time = now_time;
    printf("next color is %d:#%06X\n", end_i, color_to_hex(&color_list[end_i]));
  } else {
    cal_color(&now_color, &color_list[start_i], &color_list[end_i], x, y);
    draw(&now_color);
  }
}

void init_color_list() {
  color_from_hex(&(color_list[0]), 0x000000);
  color_from_hex(&(color_list[1]), 0x0000FF);
  color_from_hex(&(color_list[2]), 0x00FF00);
  color_from_hex(&(color_list[3]), 0xFF0000);
  color_from_hex(&(color_list[4]), 0xFFFF00);
  color_from_hex(&(color_list[5]), 0xFF00FF);
  color_from_hex(&(color_list[6]), 0x00FFFF);
  color_from_hex(&(color_list[7]), 0xFFFFFF);
}

void speed_up() {
  interval = 100;
  start_time = now_time - (now_time - start_time) * 100 / 500;
  is_speed_up = 1;
  printf("speed up\n");
}

void slow_down() {
  interval = 500;
  start_time = now_time - (now_time - start_time) * 500 / 100;
  is_speed_up = 0;
  printf("slow down\n");
}

int detect_keybrd() {
  if (!io_read(AM_INPUT_CONFIG).present) {
    return 0;
  }
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE)
    return 0;
  if (ev.keycode == AM_KEY_ESCAPE && ev.keydown)
    return 1;
  else if (ev.keydown && !is_speed_up) {
    speed_up();
  } else if (!ev.keydown && is_speed_up) {
    slow_down();
  }
  return 0;
}

int main() {
  ioe_init(); // initialization for GUI
  init_color_list();
  // int i = 0;
  // for (i = 0; i < 8; i++) {
  //   printf("#%08X\n", color_to_hex(&color_list[i]));
  // }
  while (!detect_keybrd()) {
    update_color();
  }
  return 0;
}
