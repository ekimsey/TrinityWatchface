#pragma once
#include <pebble.h>

#define SETTINGS_KEY 1

// A structure containing our settings
typedef struct ClaySettings {
  GColor BackgroundColor;
  GColor TextColor;
  char ImageColor;
} __attribute__((__packed__)) ClaySettings;

static void update_time();
static void battery_callback(BatteryChargeState state);
static void battery_update_proc(Layer *layer, GContext *ctx);
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void config_handler(DictionaryIterator *iter, void *context);
static void default_settings();
static void load_settings();
static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void init();
static void deinit();