#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "io.h"

void create_two_process(int id[]) {
  id[0] = fork();
  if (id[0] > 0) {
    id[1] = fork();
  }
  if (id[0] == -1 || id[1] == -1) {
    write(STDERR_FILENO, "fork error\n", 12);
    exit(-1);
  }
  return;
}

int main(int argc, char *argv[]) {
  int child_pid[2];
  int parent_pid = getpid();

  int parent_child1[2]; // parent -> child1
  const char *err = "pipe wasn't created\n";
  if (pipe(parent_child1) == -1) {
    write(STDERR_FILENO, err, strlen(err));
  }
  int child1_child2[2]; // child1 -> child2
  if (pipe(child1_child2) == -1) {
    write(STDERR_FILENO, err, strlen(err));
  }
  int child2_parent[2]; // child2 -> parent
  if (pipe(child2_parent) == -1) {
    write(STDERR_FILENO, err, strlen(err));
  }

  char s[100];
  read_str(s);
  create_two_process(child_pid); // create child1 & child2

  if (child_pid[0] > 0 && child_pid[1] > 0) { // Parent process
    close(parent_child1[0]);
    write(parent_child1[1], s, strlen(s)); // passes the string to the Child1 process
    close(parent_child1[1]);

    waitpid(child_pid[0], NULL, WUNTRACED);
    waitpid(child_pid[1], NULL, WUNTRACED);

    char res[100] = "";
    close(child2_parent[1]);
    read(child2_parent[0], res, strlen(s)); // gets the result from the Child2 process

    write(STDOUT_FILENO, res, strlen(res));
    close(child2_parent[0]);
  } else if (child_pid[0] == 0) { // Child1 process
    wait(NULL);
    close(parent_child1[1]);
    dup2(parent_child1[0], STDIN_FILENO);

    close(child1_child2[0]);
    dup2(child1_child2[1], STDOUT_FILENO);

    execvp("./child1", NULL);
  } else { // Child2 process
    wait(NULL);
    close(child1_child2[1]);
    dup2(child1_child2[0], STDIN_FILENO);

    close(child2_parent[0]);
    dup2(child2_parent[1], STDOUT_FILENO);

    execvp("./child2", NULL);
  }

  return 0;
}
