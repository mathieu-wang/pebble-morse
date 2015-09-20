#include <stdlib.h>
#include <pebble.h>
#include "level.h"

Level levels[10] = {{{"t", "e", "a"}, 0},
                  {{"r", "a", "n"}, 0},
                  {{"sos", "bob", "lol"}, 0},
                  {{"the", "tea", "tie"}, 0},
                  {{"word", "work", "worm"}, 0},
                  {{"redbull", "red ball", "rednull"}, 0},
                  {{"hack mit", "hack tim", "hash mit"}, 0},
                  {{"we suck at c", "we suck at p", "we suck at b"}, 0},
                  {{"Daenerys", "Deanerys", "Daeneris"}, 0},
                  {{"Kanye 2020", "Kanye 2024", "Kaney 2020"}, 0}};

void randomize_correct_answer_index() {
  srand(time(NULL));
  int i;
  for (i = 0; i < (int)(sizeof(levels)/sizeof(levels[0])); i++) {
    int randomnumber = rand() % 3;
    printf("%d\n", randomnumber);
    levels[i].index_correct_answer = randomnumber;
  }
}