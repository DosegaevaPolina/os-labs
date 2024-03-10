#include "semaphore.h"
#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // pid_t
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> // fork,getpid

#define STRING_SIZE 100

void str_to_lower(char *s) {
  for (int i = 0; s[i]; i++) {
    s[i] = tolower(s[i]);
  }
}

int main(int argc, char *argv[]) {
  // write(STDOUT_FILENO, "1 дочерний процесс запущен!\n", 51);
  char mmapped_file_name[STRING_SIZE];
  strcpy(mmapped_file_name, argv[1]);

  // write(STDOUT_FILENO, "Название mmap файла: ", 35);
  // write(STDOUT_FILENO, mmapped_file_name, 100);

  char semaphore_child1_name[STRING_SIZE];
  char semaphore_child2_name[STRING_SIZE];
  strcpy(semaphore_child1_name, argv[2]); // семафор для child1
  strcpy(semaphore_child2_name, argv[3]); // семафор для child2

  int mmapped_file_descriptor = shm_open(mmapped_file_name, O_RDWR, 0777);
  ftruncate(mmapped_file_descriptor, STRING_SIZE);
  char *mmapped_file_pointer = mmap(NULL, STRING_SIZE, PROT_READ | PROT_WRITE,
                                    MAP_SHARED, mmapped_file_descriptor, 0);

  sem_t *semaphore_child1 = sem_open(semaphore_child1_name, 0);
  sem_t *semaphore_child2 = sem_open(semaphore_child2_name, 0);
  char string[STRING_SIZE];

  // write(STDOUT_FILENO, "Я около семафора в 1 процессе!\n", 55);
  sem_wait(semaphore_child1);

  str_to_lower(mmapped_file_pointer);

  munmap(mmapped_file_pointer, 0);
  sem_post(semaphore_child2);
  sem_close(semaphore_child1);
  sem_close(semaphore_child2);
}

