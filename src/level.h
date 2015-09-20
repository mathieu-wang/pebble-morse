#pragma once

typedef struct level {
  char* answers[3];
  int index_correct_answer;
} Level;

void randomize_correct_answer_index();