#include <stdio.h>
int main(int argc, char *argv[]) {
  int bugs = 100;
  double bug_rate = 1.2;

  printf("You have %d bugs at the imaginary rate of %f.\n", bugs, bug_rate);

  // long的最大值是1<<63 - 1
  long universe_of_defects = 8L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L;
  // unsigned long的最大值是1<<64 - 1
  // unsigned long universe_of_defects =
  // 16L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L;
  printf("The entire universe has %ld bugs.\n", universe_of_defects);

  double expected_bugs = bugs * bug_rate;
  printf("You are expected to have %f bugs.\n", expected_bugs);

  double part_of_universe = expected_bugs / universe_of_defects;
  printf("That is only a %e portion of the universe.\n", part_of_universe);

  // this makes no sense, just a demo of something weird
  char nul_byte = 'a';
  // nul_byte被cast成了int
  // int care_percentage = bugs * nul_byte;
  // 如果顺序颠倒呢？也没问题
  int care_percentage = nul_byte * bugs;
  printf("Which means you should care %d%%.\n", care_percentage);

  return 0;
}
