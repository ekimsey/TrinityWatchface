#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_date_layer, *s_time_layer;
static char s_date_buffer[10];
static GBitmap *s_white_trinity_bitmap;
static BitmapLayer *s_white_trinity_layer;

static void update_time()
{
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
  }
  else
  {
    // Use 12 hour format
    strftime(time_buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  strftime(s_date_buffer, sizeof(s_date_buffer), "%b %e", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, time_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
  
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  update_time();
}

static void main_window_load(Window *window)
{
  
  // Create date Layer
  s_date_layer = text_layer_create(GRect(0, 0, 144, 18));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 18, 144, 42));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);

  // Align date TextLayer and time TextLayer to center and set font
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  s_white_trinity_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TRINITY_WHITE);
  s_white_trinity_layer = bitmap_layer_create(GRect(0, 62, 144, 106));
  bitmap_layer_set_bitmap(s_white_trinity_layer, s_white_trinity_bitmap);
  bitmap_layer_set_alignment(s_white_trinity_layer, GAlignCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_white_trinity_layer));

}

static void main_window_unload(Window *window)
{
  // Destroy layers
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_white_trinity_layer);
  gbitmap_destroy(s_white_trinity_bitmap);
  
}

static void init()
{
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  // Set main Window color to black
  window_set_background_color(s_main_window, GColorBlack);

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
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();

}

static void deinit()
{
  // Unsubscribe from tick timer service
  tick_timer_service_unsubscribe();
  
  // Destroy Window
  window_destroy(s_main_window);
  
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}