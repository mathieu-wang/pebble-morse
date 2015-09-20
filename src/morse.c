#include <pebble.h>

const uint32_t const unit_length = 200; //ms

void vibrate(const uint32_t const segments[]) {
  VibePattern pat = {
    .durations = segments,
    .num_segments = 2,
  };
  int time_to_vibrate = segments[0] + segments[1];
  vibes_enqueue_custom_pattern(pat);
  /*
    need to sleep while the vibrate method is being called because if the next call to this functions
    comes in while it is vibrating, it will simply return (vibes_enqueue_custom_pattern is not a real queue:
    http://forums.getpebble.com/discussion/20983/consecutive-vibes-enqueue-custom-patterns)
  */
  psleep(time_to_vibrate); 
}

void dot() {
  // Vibe pattern: ON for unit_length ms, OFF for unit_length ms
  const uint32_t const segments[] = {unit_length, unit_length};
  vibrate(segments);
}

void dash() {
  // Vibe pattern: ON for 3 unit_length ms, OFF for unit_length ms
//   const uint32_t const segments[] = { 3* unit_length, unit_length, unit_length};
  const uint32_t const segments[] = {3* unit_length, unit_length};
  vibrate(segments);
}

void letter_gap() {
  // Vibe pattern: OFF for 2 unit_length ms
  // letter gaps always follow element gaps (included in dot and dash), so total of 1+2=3 units long   
  const uint32_t const segments[] = {0, 2 * unit_length};
  vibrate(segments);
}

void word_gap() {
  // Vibe pattern: OFF for 4 unit_length ms
  //word gaps always follow letter gaps, so total of 1+2+4=7 units long
  const uint32_t const segments[] = {0, 4 * unit_length};
  vibrate(segments);
}
