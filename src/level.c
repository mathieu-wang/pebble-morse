#include <stdlib.h>
#include <pebble.h>
#include "level.h"

Level levels[10] = {{{"t", "e", "a"}, 0},
                  {{"r", "a", "n"}, 0},
                  {{"sos", "bob", "lol"}, 0},
                  {{"the", "tea", "tie"}, 0},
                  {{"word", "work", "worm"}, 0},
                  {{"hack mit", "hack tim", "hash mit"}, 0},
                  {{"we suck at c", "we suck at p", "we suck at b"}, 0},
                  {{"Kanye 2020", "Kanye 2024", "Kanye 3020"}, 0},
                  {{"how can we code without redbull?", "how can we code without a red ball?", "how can we node without redbull?"}, 0},
                  {{"Daenerys of House Targaryen, the First of Her Name, Queen of Meereen, Queen of the Andals, the Rhoynar and the First Men, Lady Regnant of the Seven Kingdoms, Protector of the Realm, Khaleesi of the Great Grass Sea, Breaker of Chains, and Mother of Dragons",
                   "Daenerys of House Targaryen, the Fourth of Her Name, Queen of Meereen, Queen of the Andals, the Rhoynar and the First Men, Lady Regnant of the Seven Kingdoms, Protector of the Realm, Khaleesi of the Great Grass Sea, Breaker of Chains, and Mother of Dragons",
                   "Daenerys of House Targaryen, the First of Her Name, Queen of Meereen, Queen of the Andals, the Rhoynar and the First Men, Lady Regnant of the Seven Kingdoms, Protector of the Realm, Khaleesi of the Great Grass Sea, Breaker of Chains, and Mother of Drogons"}, 0}};

void randomize_correct_answer_index() {
  srand(time(NULL));
  int i;
  for (i = 0; i < (int)(sizeof(levels)/sizeof(levels[0])); i++) {
    int randomnumber = rand() % 3;
    printf("%d\n", randomnumber);
    levels[i].index_correct_answer = randomnumber;
  }
}