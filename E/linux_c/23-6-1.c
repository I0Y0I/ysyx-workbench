void test1() {
  const char **p;
  // 应该是这条有问题
  (**p)++;
  (*p)++;
  p++;
}
void test2() {
  char *const *p;
  (**p)++;
  // 应该是这条有问题
  (*p)++;
  p++;
}
void test3() {
  char **const p;
  (**p)++;
  (*p)++;
  // 应该是这条有问题
  p++;
}
