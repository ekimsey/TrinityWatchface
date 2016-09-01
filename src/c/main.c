#include <pebble.h>
#include "main.h"

static Window *s_main_window;
static Layer *window_layer, *s_battery_layer;
static BitmapLayer *s_bitmap_layer;
static TextLayer *s_date_layer, *s_time_layer;
static char s_date_buffer[10];
static int s_battery_level;
// Image size: 140 x 136
static GBitmap *s_bitmap;
ClaySettings settings;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Create long-lived buffers for date and time
  static char time_buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true)
  {
    // Use 24 hour format
    strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(time_buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  strftime(s_date_buffer, sizeof(s_date_buffer), "%b %e", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, time_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

static void battery_callback(BatteryChargeState state) {
  // Record the new battery level
  s_battery_level = state.charge_percent;
  // Update meter
  layer_mark_dirty(s_battery_layer);
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bg_bar_bounds = layer_get_bounds(layer);
  GRect fg_bar_bounds = GRect(bg_bar_bounds.origin.x, bg_bar_bounds.origin.y, 
                              bg_bar_bounds.size.w, bg_bar_bounds.size.h);
  
  // Find the width of the foreground bar
  fg_bar_bounds.size.w = (int)(float)(((float)s_battery_level / 100.0F) * bg_bar_bounds.size.w);
  
  // Center foreground bar
  fg_bar_bounds.origin.x += (bg_bar_bounds.size.w - fg_bar_bounds.size.w) / 2;

  // Draw the background of battery bar
  if(s_battery_level <= 10) {
    #if defined(PBL_COLOR)
      graphics_context_set_fill_color(ctx, GColorRed);
    #else
      graphics_context_set_fill_color(ctx, GColorBlack);
    #endif
  } else if(s_battery_level <= 20) {
    #if defined(PBL_COLOR)
      graphics_context_set_fill_color(ctx, GColorOrange);
    #else
      graphics_context_set_fill_color(ctx, GColorBlack);
    #endif
  }

  // Draw the bar
  #if defined(PBL_COLOR)
    graphics_fill_rect(ctx, bg_bar_bounds, 0, GCornerNone);
  #endif
  graphics_context_set_fill_color(ctx, settings.TextColor);
  graphics_fill_rect(ctx, fg_bar_bounds, 0, GCornerNone);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void config_handler(DictionaryIterator *iter, void *context) {
  // Read color preferences
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  if(bg_color_t) {
    settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);
  }

  Tuple *txt_color_t = dict_find(iter, MESSAGE_KEY_TextColor);
  if(txt_color_t) {
    settings.TextColor = GColorFromHEX(txt_color_t->value->int32);
  }

  Tuple *trinity_color_t = dict_find(iter, MESSAGE_KEY_ImageColor);
  if(trinity_color_t) {
    settings.ImageColor = trinity_color_t->value->cstring[0];
  }
  // Use new setings
  window_set_background_color(s_main_window, settings.BackgroundColor);
  text_layer_set_text_color(s_date_layer, settings.TextColor);
  text_layer_set_text_color(s_time_layer, settings.TextColor);
  layer_mark_dirty(s_battery_layer);
  if(settings.ImageColor == 'w') {
    s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TRINITY_WHITE);
  } else {
    s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TRINITY_BLACK);
  }
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
}

// Initialize the default settings
static void default_settings() {
  settings.BackgroundColor = GColorDukeBlue;
  settings.TextColor = GColorWhite;
  settings.ImageColor = 'w';
}

// Read settings from persistent storage
static void load_settings() {
  // Load the default settings
  default_settings();
  // Read settings from persistent storage, if they exist
  //persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void main_window_load(Window *window) {
  // Get the bounds of the Window
  window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  // Set main Window color
  #if defined(PBL_COLOR)
    window_set_background_color(window, settings.BackgroundColor);
  #else
    window_set_background_color(window, GColorBlack);
  #endif
  
  // Create the object of trinity file
  if(settings.ImageColor == 'w') {
    s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TRINITY_WHITE);
  } else {
    s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TRINITY_BLACK);
  }
  
  // Create date Layer
  GRect date_layer_bounds = window_bounds;
  date_layer_bounds.size.h /= 8;
  s_date_layer = text_layer_create(date_layer_bounds);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, settings.TextColor);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Create time TextLayer
  GRect time_layer_bounds = window_bounds;
  time_layer_bounds.size.h /= 5;
  time_layer_bounds.origin.y = (window_bounds.size.h / 8);
  s_time_layer = text_layer_create(time_layer_bounds);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, settings.TextColor);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Create battery meter Layer
  #if defined(PBL_RECT)
    s_battery_layer = layer_create(GRect((window_bounds.size.w * .125), (window_bounds.size.h * .35), (window_bounds.size.w * .75), 2));
  #else
    s_battery_layer = layer_create(GRect((window_bounds.size.w * .125), (window_bounds.size.h * .33), (window_bounds.size.w * .75), 2));
  #endif
  layer_set_update_proc(s_battery_layer, battery_update_proc);
  
  // Add to Window
  layer_add_child(window_get_root_layer(window), s_battery_layer);
  
  // Create the image canvas Layer
  GRect image_layer_bounds = window_bounds;
  image_layer_bounds.size.h = 100;
  #if defined(PBL_RECT)
    image_layer_bounds.origin.y = window_bounds.size.h - 100;
  #else
    image_layer_bounds.origin.y = window_bounds.size.h - 110;
  #endif
  s_bitmap_layer = bitmap_layer_create(image_layer_bounds);
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
}

static void main_window_unload(Window *window) {
  // Destroy layers
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_time_layer);
  layer_destroy(s_battery_layer);
  bitmap_layer_destroy(s_bitmap_layer);
  gbitmap_destroy(s_bitmap);
}

static void init() {
  // Obtain configuration settings
  load_settings();
  
  // Listen for AppMessages
  app_message_register_inbox_received(config_handler);
  app_message_open(128, 128);
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers)
    {
      .load = main_window_load,
      .unload = main_window_unload
    }
  );

  s_date_buffer[0] = '\0';
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
  
  // Grab battery level to start
  battery_callback(battery_state_service_peek());
}

static void deinit() {
  // Unsubscribe from tick timer service
  tick_timer_service_unsubscribe();
  
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}