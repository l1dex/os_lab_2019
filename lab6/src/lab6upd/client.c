#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#define h_addr h_addr_list[0]
struct Server {
  char ip[255];
  int port;
};
 typedef struct arg_str{
  struct Server serverObj;
  uint64_t begin;
  uint64_t end;
} argsForThread;
pthread_t *threads;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
char** readServers(int fd);
int get_next_line(const int fd, char **line);
char** strsplit(const char* s, char d);
void* serversThread(void* argsForThread);
uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}
uint64_t mod = -1;
int main(int argc, char **argv) {
  uint64_t k = -1;
  //uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255
  int fd = 0;
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0: {
        ConvertStringToUI64(optarg, &k);
	printf("%d\n",k);
	// TODO: your code here
        break;
      }
      case 1:{
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
        break;
      }
      case 2:
        // TODO: your code here
	if ((fd = open(optarg, O_RDONLY)) < 0){
	  printf("error opening servers file. Enter correct path");
	  return 1;
	}
        memcpy(servers, optarg, strlen(optarg));
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }
  char *args;
  // for one server here, rewrite with servers from file
  unsigned int servers_num = 1;
  // TODO: delete this and parallel work between servers
  //to[0].port = 20001;
  //memcpy(to[0].ip, "127.0.0.1", sizeof("127.0.0.1"));
  //done
  char **ret = NULL;
  ret = readServers(fd);
  int i = 0;
  while(ret[i]){
    i++;
    //printf("%s\n",ret[i]);
  }
  servers_num = i;
  struct Server *to = malloc(sizeof(struct Server) * servers_num);
  char** split;
  uint64_t part = k/servers_num;
  for (int j = 0; j < servers_num; j++){ 
    split = strsplit(ret[j],':');
    //printf("split ip = %s\n", split[0]);
    memcpy(&to[j].ip, split[0], strlen(split[0]));
    //to[j].ip[9] = '\0';
    //strcpy(&to[j].ip, split[0]);
    //to[j].ip[9] = '\0';
    to[j].port = atoi(split[1]);
    printf("%s:",to[j].ip);
    printf("%d\n",to[j].port);
    printf("j = %d\n",j);
    //free(split[0]);
    //free(split[1]);
    //free(split);
  }
  close(fd);
  printf("************");
  //argsForThread->res =(int*)malloc(sizeof(int) * servers_num);
  threads = (pthread_t*)malloc(sizeof(pthread_t) * servers_num);
  // TODO: work continiously, rewrite to make parallel 
  uint64_t result = 0;
  uint64_t answer = 1;
  argsForThread* argsArray = (argsForThread*)malloc(sizeof(argsForThread) * servers_num);
  for (int i = 0; i < servers_num; i++) {
    //argsForThread* argsForThread = (argsForThread)malloc(sizeof(argsForThread));
    argsArray[i].serverObj = to[i];    
    argsArray[i].begin = i*part+1;
    argsArray[i].end = (i == (servers_num - 1) ) ? k : (i + 1) * part;
    if (pthread_create(&threads[i], NULL, (void *)serversThread, (void*)&argsArray[i])){
      printf("Error creating thread!\n");
      return 1;
    }
    // TODO: from one server
    // unite results
    //uint64_t answer = 0;
    // memcpy(&answer, response, sizeof(uint64_t));
  }
  for (int i = 0; i < servers_num; i++){
      if(pthread_join(threads[i],(void**)&result) != 0){
	perror("pthread_join");
	exit(1);
      }
  }
  answer = MultModulo(answer, result, mod);
  printf("answer: %llu\n", answer);
  //free(to);
  return 0;
}
void* serversThread(void* arguments){
  argsForThread *args = (argsForThread*)arguments;
  //struct Server* serverObj = args->serverObj;
  struct hostent *hostname = gethostbyname(args->serverObj.ip);
  if (hostname == NULL) {
    fprintf(stderr, "gethostbyname failed with %s\n", args->serverObj.ip);
    exit(1);
  }
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(args->serverObj.port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);
    printf("%s:%d\n",args->serverObj.ip,args->serverObj.port);
    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }

    // TODO: for one server
    // parallel between servers
    //uint64_t begin = 1;
    //uint64_t end = k;
    pthread_mutex_lock(&mut);
    char task[sizeof(uint64_t) * 3];
    memcpy(task, &args->begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &args->end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck, task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }
    pthread_mutex_unlock(&mut);
    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }
    uint64_t res;
    memcpy(&res, response, sizeof(uint64_t));
    close(sck);
    return (void*)(uint64_t*)res;
} 
char** readServers(int fd)
{
  int result = 0;
  char *line = 0;
  char **values;
  int i = 0;
  // printf("entry");
  values = (char**)malloc(sizeof(char*));
  while ((result = get_next_line(fd, &line)) == 1){
    // printf("%s\n",line);
    values[i] = strdup(line);
    // printf("%d - %s\n",i, values[i]);
    i++;
    values = (char**)realloc(values,sizeof(char*)*i+1);
    // free(line);    
  }
    values[i] = NULL;
    return values;
 
}