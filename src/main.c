#include "pebble.h"
#include "main.h"
#include "string_manipulation.h"
#include "morse.h"
#include "level.h"
#include <stdbool.h>

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 1
#define NUM_ANSWER_SECTIONS 1
#define NUM_ANSWER_ITEMS 3
#define MAX_LEVEL 10
int level = 1;
int score = 0;

static Window *s_main_window;
static TextLayer *s_question_layer;
static TextLayer *s_result_layer;
static MenuLayer *choose_answer_menu_layer;
static MenuLayer *difficulty_menu_layer;

/* choose difficulty callbacks */
static uint16_t difficulty_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t difficulty_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    case 1:
      return NUM_SECOND_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t difficulty_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void difficulty_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Start Game");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Prepare Yourself");
      break;
  }
}

static void difficulty_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {  
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, "Beginner", "Noobs", NULL);
          break;
        case 1:
          
          menu_cell_basic_draw(ctx, cell_layer, "Intermediate", "Still a noob", NULL);
          break;
        case 2: 
          menu_cell_basic_draw(ctx, cell_layer, "Hard", "Expert", NULL);
          break;
      }
      break;
    case 1:
      switch (cell_index->row) {
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, "Reference", "Learn Morse Code", NULL);
          break;
      }
  }
}

/* handler to go to question page */
static void difficulty_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  menu_layer_destroy(menu_layer);
  switch_to_question_page(s_main_window);
}


/* choose answer menu callbacks */
static uint16_t answer_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_ANSWER_SECTIONS;
}

static uint16_t answer_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_ANSWER_ITEMS;
    default:
      return 0;
  }
}

static int16_t answer_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void answer_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which level we're working with
  char level_text[80];
  strcpy(level_text, "Level ");
  
  char snum[20];
  int_to_string(snum, level);
  strcat(level_text, snum);
  
  menu_cell_basic_header_draw(ctx, cell_layer, level_text);
}

static void answer_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item we'll draw
  
  Level current_level = levels[level-1];
  switch (cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, current_level.answers[0], NULL, NULL);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, current_level.answers[1], NULL, NULL);
      break;
    case 2: 
      menu_cell_basic_draw(ctx, cell_layer, current_level.answers[2], NULL, NULL);
      break;
  }
}


/* handler for going to result page */
static void answer_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  menu_layer_destroy(menu_layer);
  
  // Use the row to specify which item will receive the select action
  switch (cell_index->row) {
    case 0:
      switch_to_result_page(s_main_window, 0);
      break;
    case 1:
      switch_to_result_page(s_main_window, 1);
      break;
    case 2:
      switch_to_result_page(s_main_window, 2);
      break;
  }
}


/* handler for going to choose answer page */
static void start_answering_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_destroy(s_question_layer);
  switch_to_choose_answer_page(s_main_window);
}

static void click_config_provider_question_page(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, start_answering_click_handler);
}


/* handler for returning to question page after result page */
static void return_to_question_page_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_destroy(s_result_layer);
  
  if (level == MAX_LEVEL) {
    switch_to_difficulty_page(s_main_window);
    level = 1;
    score = 0;
  } else {
    level++;
    switch_to_question_page(s_main_window); 
  }
  
}

static void click_config_provider_result_page(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, return_to_question_page_handler);
}


/* helper function to check answer */
static bool isAnswerCorrect(int answerIndex) {
  Level current_level = levels[level-1];
  if (answerIndex == current_level.index_correct_answer) {
    score += 10;
    return true;
  } else {
    return false;
  }
}

static char* getCorrectAnswer(int answerIndex) {
  Level current_level = levels[level-1];
  return current_level.answers[current_level.index_correct_answer];
}

/* encapsulates switching */
static void switch_to_question_page(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  static char question_string[80];
  strcpy(question_string, "Level ");
  
  char snum[20];
  int_to_string(snum, level);
  strcat(question_string, snum);
  strcat(question_string, ": \nListen and identify the Morse Code...\n\nPress Select to answer!");
  
  s_question_layer = text_layer_create(GRect(5, 0, bounds.size.w - 5, bounds.size.h));
  text_layer_set_font(s_question_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_question_layer, question_string);
  text_layer_set_overflow_mode(s_question_layer, GTextOverflowModeWordWrap);
  
  layer_add_child(window_layer, text_layer_get_layer(s_question_layer));
  window_set_click_config_provider(s_main_window, click_config_provider_question_page);
  
  Level current_level = levels[level-1];
  char* str = current_level.answers[current_level.index_correct_answer];
  call_vib(str);
}

static void switch_to_choose_answer_page(Window *window) {
  // Now we prepare to initialize the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  // Create the menu layer
  choose_answer_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(choose_answer_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = answer_menu_get_num_sections_callback,
    .get_num_rows = answer_menu_get_num_rows_callback,
    .get_header_height = answer_menu_get_header_height_callback,
    .draw_header = answer_menu_draw_header_callback,
    .draw_row = answer_menu_draw_row_callback,
    .select_click = answer_menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(choose_answer_menu_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(choose_answer_menu_layer));
}

static void switch_to_result_page(Window *window, int indexOfGivenAnswer) {
  static char result_string[80];
  strcpy(result_string, "Level ");
  
  char snum[20];
  int_to_string(snum, level);
  strcat(result_string, snum);
  
  if (isAnswerCorrect(indexOfGivenAnswer)) {
    strcat(result_string, ": Correct!\n");
  } else {
    strcat(result_string, ": Wrong!\n");
  }
  
  strcat(result_string, "\nThe answer was: \n");
  strcat(result_string, getCorrectAnswer(indexOfGivenAnswer));
  strcat(result_string, "\n\nScore: ");
  
  if (score == 0) {
    strcat(result_string, "You suck!");
  } else {
    char sscore[20];
    int_to_string(sscore, score);
    strcat(result_string, sscore);
  }
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  s_result_layer = text_layer_create(GRect(5, 0, bounds.size.w - 5, bounds.size.h));
  text_layer_set_font(s_result_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_result_layer, result_string);
  text_layer_set_overflow_mode(s_result_layer, GTextOverflowModeWordWrap);
  
  layer_add_child(window_layer, text_layer_get_layer(s_result_layer));
  window_set_click_config_provider(s_main_window, click_config_provider_result_page);
}

static void switch_to_difficulty_page(Window *window) {
  // Create the menu layer for choosing difficulty
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  // Create the menu layer
  difficulty_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(difficulty_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = difficulty_menu_get_num_sections_callback,
    .get_num_rows = difficulty_menu_get_num_rows_callback,
    .get_header_height = difficulty_menu_get_header_height_callback,
    .draw_header = difficulty_menu_draw_header_callback,
    .draw_row = difficulty_menu_draw_row_callback,
    .select_click = difficulty_menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(difficulty_menu_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(difficulty_menu_layer));
}

/* load initial page */
static void main_window_load(Window *window) {
  randomize_correct_answer_index();
  switch_to_difficulty_page(window);
}

/* boiler plate to start UI */
static void main_window_unload(Window *window) {
  // Destroy the menu layer
//   menu_layer_destroy(difficulty_menu_layer);
//   text_layer_destroy(s_question_layer);
//   menu_layer_destroy(choose_answer_menu_layer);
//   text_layer_destroy(s_result_layer);
}
static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}
static void deinit() {
  window_destroy(s_main_window);
}
int main(void) {
  init();
  app_event_loop();
  deinit();
}
