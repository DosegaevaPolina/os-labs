#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

struct ThreadArgs { // используется для передачи аргументов в потоки
  int *array;
  int start;
  int end;
};

bool boolSortedArray(int array[],
                     int sizeArray) // проверяет, отсортирован ли массив `array`
                                    // размером `sizeArray`
{
  for (int indexInArray = 0; indexInArray < sizeArray - 1; indexInArray++) {
    if (array[indexInArray] > array[indexInArray + 1]) {
      return false;
    }
  }
  return true;
}

void sortEvenAndOdd(
    int array[], int sizeArray) // разделяет массив на четные и нечетные
                                // элементы и затем снова объединяет их в массив
{
  int even[sizeArray / 2];
  int odd[sizeArray / 2];
  int indexEven = 0;
  int indexOdd = 0;
  for (int indexInArray = 0; indexInArray < sizeArray; indexInArray++) {
    if (indexInArray % 2 == 0) {
      even[indexEven++] = array[indexInArray];
    } else {
      odd[indexOdd++] = array[indexInArray];
    }
  }
  indexOdd = 0;
  for (int indexInArray = 0; indexInArray < sizeArray; indexInArray++) {
    if (indexInArray < sizeArray / 2) {
      array[indexInArray] = even[indexInArray];
    } else {
      array[indexInArray] = odd[indexOdd++];
    }
  }
}

void sortedArray(
    int array[],
    int sizeArray) // сортирует массив, пока он не окажется отсортированным
{
  while (!boolSortedArray(array, sizeArray)) {
    for (int indexInArray = 0; indexInArray < sizeArray - 1; indexInArray++) {
      if (array[indexInArray] > array[indexInArray + 1]) {
        int temp = array[indexInArray + 1];
        array[indexInArray + 1] = array[indexInArray];
        array[indexInArray] = temp;
      }
    }
  }
}

void arrayDivision(int array[],
                   int sizeArray) // разделяет исходный массив на части
{
  bool boolExtraNumber = false;
  if (sizeArray % 2 != 0) {
    sizeArray--;
    boolExtraNumber = true;
  }

  if (sizeArray != 2) {

    sortEvenAndOdd(array, sizeArray);
    int firstHalf[sizeArray / 2];
    int secondHalf[sizeArray / 2];
    int indexSecondHalf = 0;
    int indexFirstHalf = 0;

    for (int indexInArray = 0; indexInArray < sizeArray; indexInArray++) {
      if (indexInArray < sizeArray / 2) {
        firstHalf[indexInArray] = array[indexInArray];
      } else {
        secondHalf[indexSecondHalf++] = array[indexInArray];
      }
    }

    arrayDivision(firstHalf, sizeArray / 2);
    arrayDivision(secondHalf, sizeArray / 2);

    indexSecondHalf = 0;
    for (int indexInArray = 0; indexInArray < sizeArray; indexInArray++) {
      if (indexInArray % 2 == 0) {
        array[indexInArray] = firstHalf[indexFirstHalf++];
      } else {
        array[indexInArray] = secondHalf[indexSecondHalf++];
      }
    }
    if (boolExtraNumber) {
      sizeArray++;
      sortedArray(array, sizeArray);
    } else {
      sortedArray(array, sizeArray);
    }
  } else {
    if (array[0] > array[1]) {
      int temp = array[1];
      array[1] = array[0];
      array[0] = temp;
    }
  }
}

void *calculations(
    void *arg) // является точкой входа для потока, принимает указатель arg в
               // качестве аргумента, возвращает указатель типа void
{
  struct ThreadArgs *args = (struct ThreadArgs *)
      arg; // принимает указатель arg на структуру ThreadArgs
  int arraySize = args->end - args->start +
                  1; // вычисляется размер массива, обрабатываемого в потоке
  arrayDivision(args->array + args->start,
                arraySize); // разделяет часть массива для сортировки.
  sortedArray(args->array + args->start,
              arraySize); // выполняется сортировка части массива
  return NULL;
}

int main(int argc, char *argv[]) {
  int numberOfThreads = 0; // количество потоков

  if (argc < 2) {
    cout << "Введите количество потоков" << endl;
    return 1;
  }

  numberOfThreads = atoi(argv[1]);

  cout << "Введите длину массива" << endl;
  int arraySize;
  cin >> arraySize;
  int *array = new int[arraySize];
  cout << "Введите элементы массива" << endl;
  for (int i = 0; i < arraySize; ++i) {
    cin >> array[i];
  }

  /*
  int arraySize = 100;
  int* array = new int[arraySize];


  for (int i = 0; i < arraySize; i++) {
      array[i] = arraySize - i;
  }
  */

  cout << "Количество потоков : " << numberOfThreads << endl;

  // точка отсчёта запуска программы
  struct timeval start, end;
  gettimeofday(&start, NULL);

  pthread_t
      threads[numberOfThreads]; // создает массив структур pthread_t, который
                                // будет содержать идентификаторы всех потоков
  struct ThreadArgs
      args[numberOfThreads]; // создает массив структур `ThreadArgs`, который
                             // будет содержать аргументы для каждого потока.

  for (int i = 0; i < numberOfThreads; i++) // создает несколько потоков
  {
    args[i].array = array; // позволяет каждому потоку иметь доступ к исходному
                           // массиву для сортировки своей части
    args[i].start =
        i * (arraySize /
             numberOfThreads); // устанавливает начальный индекс части массива,
                               // которую будет сортировать `i`-ый поток
    args[i].end = (i + 1) * (arraySize / numberOfThreads) -
                  1; // устанавливает конечный индекс этой части массива для
                     // сортировки текущим потоком

    // создается новый поток. Параметры функции `pthread_create` включают
    // идентификатор потока, аргументы,
    //  функцию, которую нужно выполнить в потоке, и аргументы для этой функции,
    //  аргументы передаются через указатель на `args[i]`.
    pthread_create(&threads[i], NULL, calculations, (void *)&args[i]);
  }

  for (int i = 0; i < numberOfThreads;
       i++) // ожидание завершения выполнения всех созданных потоков
  {
    pthread_join(threads[i], NULL);
  }

  sortedArray(array, arraySize); // окончательная сортировка после того, как
                                 // потоки завершили свою работу

  // точка завершения запуска программы
  gettimeofday(&end, NULL);

  double execution_time = (end.tv_sec - start.tv_sec) * 1000.0;
  execution_time += (end.tv_usec - start.tv_usec) / 1000.0;

  cout << "Время исполнения: " << execution_time << endl;

  cout << "Отсортированный массив: ";
  for (int i = 0; i < arraySize; i++) {
    cout << array[i] << " ";
  }
  cout << endl;

  delete[] array; // освобождает память, выделенную для массива

  return 0;
}
