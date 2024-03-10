#include <unistd.h>
#include <stdio.h>  // EOF

void read_str(char *s) {
  int i = 0;
  do {
    char c;
    read(STDIN_FILENO, &c, sizeof(char));
    s[i++] = c;
  } while (s[i - 1] != 0 && s[i - 1] != '\n' && s[i - 1] != EOF);
  s[i] = 0;
}

void write_str(const char *s) {
  int i = 0;
  do {
    write(STDOUT_FILENO, &s[i++], sizeof(char));
  } while (s[i - 1] != 0 && s[i - 1] != '\n');
}

