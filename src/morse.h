#pragma once
#include <stdio.h>
#include <string.h>
#include <math.h>

void vibrate(const uint32_t segments[]);
void dot();
void dash();
void letter_gap();
void word_gap();
char* getMorseCode(char c);
void call_vib(char *word);

extern char* MORSE_CODES[256];
extern uint32_t unit_length;
