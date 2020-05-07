#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

//libs from parralel 

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

int SaintNumber = -1;
int** SaintMassive;

struct SumArgs {
  int *array;
  int begin;
  int end;
};

void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  for (int i = 0; i < array_size; i++) {
    array[i] = rand()%10;
  }
}

int Sum(const struct SumArgs *args) {
  printf("Args struct: begin %d, end %d, ",args->begin,args->end);
  int sum = 0;
  printf("taken massive for calc summ:"); 
  for(int i = args->begin;i < args->end;i++){
      printf("%d ",args->array[i - args->begin]);
    sum+=args->array[i - args->begin];
  } 
  printf("\nretuned sum from theards %d\n",sum);
  return sum;
}

void *ThreadSum(void *args) {
  int GodValue = ++SaintNumber;

  if(GodValue == 0){
      printf("\n Zalupa detected - %d \n",SaintMassive[GodValue][0]);
  }

  struct SumArgs *sum_args = (struct SumArgs *)args + GodValue;
  sum_args->array = SaintMassive[GodValue];
  printf("\nArray in ThreadSum");
  for(int i = sum_args->begin;i < sum_args->end;i++){
   printf( " %d ",sum_args->array[i - sum_args->begin]);
  }
  printf("\n");
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  

  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */
   while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"threads", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
                printf("seed is a positive number\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
          case 2:
            threads_num = atoi(optarg);
            if (threads_num <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }
  if (seed == -1 || array_size == -1 || threads_num == -1) {
    printf("--threads_num = num --seed = num --array_size = num",
           argv[0]);
    return 1;
  }

 if(threads_num >= array_size){
     printf("\nThreads num < array_size\n");
     return 1;
 }
    printf("Show of arg from console: seed = %d, array_size = %d, threads_num = %d\n",seed,array_size,threads_num);

  /*
   * TODO:
   * your code here
   * Generate array here
   */

  int *array = malloc(sizeof(int) * array_size);

  GenerateArray(array,array_size,seed);

    //show generated massive 
    for(int i = 0; i < array_size;i++){
        printf(" %d of array = %d\n",i,array[i]);
    }

  pthread_t threads[threads_num];
  struct SumArgs args[threads_num];//haahah tvari 
  int part = (array_size / threads_num)  + 1;
  SaintMassive = malloc(threads_num * sizeof(int));
  for(int i = 0;i<threads_num;i++){
    args[i].begin = i*part;
    args[i].end = ((i+1)*part <= array_size) ? (i+1)*part : array_size;
    printf("begin of %d is %d Array:",i,args[i].begin);
    args[i].array = (int*) malloc((args[i].end - args[i].begin)*sizeof(int));
    for(int j = args[i].begin; j < args[i].end; j++){
        args[i].array[j - args[i].begin] = array[i*part + j - args[i].begin];
        printf(" %d ",args[i].array[j - args[i].begin]);
    } 
    printf("\n");
    if(i == 0){
        SaintMassive[i] = malloc(part * sizeof(int));
        for(int j = 0; j < part; j++){
            SaintMassive[i][j] = args[i].array[j];
        }
    }else{
        SaintMassive[i] = args[i].array;
    }
  }
 printf("Zalupa with Saint massive ");
  for(int i = 0 ;i < 4;i++){
    printf(" %d ",SaintMassive[0][i]);
  }
printf("\n");
  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  free(array);
  printf("Total: %d\n", total_sum);
  return 0;
}
