#pragma once
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DASH 1
#define DOT 2

void vibrate(const uint32_t const segments[]);
void dot();
void dash();
void letter_gap();
void word_gap();
char* getMorseCode(char c);



void chars_to_vibes(char* chars);



