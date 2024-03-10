#include <ctype.h>
#include <unistd.h>
#include "io.h"

void str_to_lower(char *s) {
  for (int i = 0; s[i]; i++) {
    s[i] = tolower(s[i]);
  }
}

int main() {
  char str[100];

  read_str(str);
  str_to_lower(str);
  write_str(str);
  return 0;
}
