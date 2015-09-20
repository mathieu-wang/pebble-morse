#include <pebble.h>
#define CURRENT_STAGE_KEY 00
#define CURRENT_LEVEL_KEY 01
#define CURRENT_STAGE_KEY_DEFAULT 0
#define CURRENT_LEVEL_KEY_DEFAULT 0
#define NUM_OF_DIGITS_STAGE 2
#define NUM_OF_DIGITS_LEVEL 2

int read_stage_from_storage() {
   int current_stage = persist_exists(CURRENT_STAGE_KEY) 
   ? persist_read_int (CURRENT_STAGE_KEY)
   : CURRENT_STAGE_KEY_DEFAULT;
   
   return current_stage;
}

int read_level_from_storage() {
   int current_level = persist_exists(CURRENT_LEVEL_KEY_DEFAULT) 
      ? persist_read_int (CURRENT_LEVEL_KEY_DEFAULT)
      : CURRENT_LEVEL_KEY_DEFAULT;
   return current_level;
}

void write_stage_to_storage(int stage) {    
   persist_write_int(CURRENT_STAGE_KEY, stage);
}

void write_level_to_storage(int level) {
   persist_write_int(CURRENT_LEVEL_KEY, level);   
}
