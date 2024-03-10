#include "semaphore.h"
#include "sys/wait.h"
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

int create_process() {
  pid_t pid = fork();
  if (-1 == pid) {
    perror("Error while fork");
    exit(-2);
  }
  return pid;
}

void read_input(char *buffer, size_t size) {
  ssize_t n;
  n = read(STDIN_FILENO, buffer, size - 1);
  if (n == -1) {
    perror("Read error");
    exit(-1);
  }
  for (int i = n; i < size; i++) {
    buffer[i] = 0;
  }
}

int main() {
  const char *mmapped_file_name;
  mmapped_file_name = "mmaped_file";

  const char *semaphores_names[3];
  semaphores_names[0] = "/semaphoreOne";   // семафор для child1
  semaphores_names[1] = "/semaphoreTwo";   // семафор для child2
  semaphores_names[2] = "/semaphoreThree"; // семафор для parent

  // shm_unlink(mmapped_file_name);
  // sem_unlink(semaphores_names[0]);
  // sem_unlink(semaphores_names[1]);
  // sem_unlink(semaphores_names[2]);

  int mmapped_file_descriptor;
  char *mmapped_file_pointer;

  mmapped_file_descriptor = shm_open(mmapped_file_name, O_RDWR | O_CREAT, 0777);
  ftruncate(mmapped_file_descriptor, STRING_SIZE);
  mmapped_file_pointer = mmap(NULL, STRING_SIZE, PROT_READ | PROT_WRITE,
                              MAP_SHARED, mmapped_file_descriptor, 0);

  sem_t *semaphores[3];
  semaphores[0] = sem_open(semaphores_names[0], O_CREAT, 0777, 0);
  semaphores[1] = sem_open(semaphores_names[1], O_CREAT, 0777, 0);
  semaphores[2] = sem_open(semaphores_names[2], O_CREAT, 0777, 0);

  pid_t pid = create_process();
  if (pid == 0) // child1
  {
    execl("child1", "", mmapped_file_name, semaphores_names[0],
          semaphores_names[1], NULL);
    perror("exec");
    exit(-3);
  } else {
    pid_t pid = create_process();
    if (pid == 0) // child2
    {
      execl("child2", "", mmapped_file_name, semaphores_names[1],
            semaphores_names[2], NULL);
      perror("exec");
      exit(-3);
    } else // parent
    {
      char str[STRING_SIZE];
      read_input(str, STRING_SIZE); // читаем входные данные в str
      for (int i = 0; i < strlen(str); ++i) {
        mmapped_file_pointer[i] = str[i];
      }

      sem_post(semaphores[0]);
      sem_wait(semaphores[2]);
      wait(NULL);

      munmap(mmapped_file_pointer, 0);
      shm_unlink(mmapped_file_name);
      for (int i = 0; i < 3; i++) {
        sem_unlink(semaphores_names[i]);
      }

      write(STDOUT_FILENO, mmapped_file_pointer, STRING_SIZE);
    }
  }
  return 0;
}


