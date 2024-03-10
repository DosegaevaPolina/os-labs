#include "io.h"
#include <string.h>  // strlen

void str_unify_spaces(char *str) {
  char tmp[strlen(str)+1];
  tmp[0] = str[0];
  for(int i=1,j=1; str[i]; i++){
    if (str[i-1] == ' ' && str[i] == ' ')
      continue;
    tmp[j++] = str[i];
  }
  strcpy(str, tmp);
}


int main() {
  char str[100];

  read_str(str);
  str_unify_spaces(str);
  write_str(str);
  return 0;
}
