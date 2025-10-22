#include "dbg.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
typedef int (*lib_function)(const char *data, int maxlen);

int main(int argc, char *argv[]) {
  int rc = 0;

  void *lib = dlopen("build/libex29.so", RTLD_LAZY);
  check(lib != NULL, "Failed to open the library: %s", dlerror());

  lib_function print_a_message = dlsym(lib, "print_a_message");
  check(print_a_message != NULL, "Did not find print_a_message");

  lib_function uppercase = dlsym(lib, "uppercase");
  check(uppercase != NULL, "Did not find uppercase");

  lib_function lowercase = dlsym(lib, "lowercase");
  check(lowercase != NULL, "Did not find lowercase");

  lib_function fail_on_purpose = dlsym(lib, "fail_on_purpose");
  check(fail_on_purpose != NULL, "Did not find fail_on_purpose");

  rc = print_a_message("hello", 10);
  check(rc == 0, "Function print_a_message return %d", rc);

  rc = uppercase("hello", 10);
  check(rc == 0, "Function uppercase return %d", rc);

  rc = lowercase("HELLO", 10);
  check(rc == 0, "Function lowercase return %d", rc);

  rc = fail_on_purpose("HELLO", 10);
  check(rc == 0, "Function fail_on_purpose return %d", rc);

  rc = dlclose(lib);
  check(rc == 0, "Failed to close lib");

  return 0;
error:
  return 1;
}
