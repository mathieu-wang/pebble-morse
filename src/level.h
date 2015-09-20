#pragma once

typedef struct level {
  char* answers[3];
  int index_correct_answer;
} Level;

extern Level levels[10];

void randomize_correct_answer_index();