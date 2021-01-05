#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define BUFF_SIZE 1028
#define LENGTH 2048

// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[32];
char choice[32];
void str_overwrite_stdout() {
  printf("%s", "> ");
  fflush(stdout);
}

void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

void send_msg_handler() {
  char message[LENGTH] = {};
  char buffer[LENGTH + 32] = {};

  while(1) {
    str_overwrite_stdout();
    fgets(message, LENGTH, stdin);
    str_trim_lf(message, LENGTH);

    if (strcmp(message, "exit") == 0) {
      break;
    } else {
      sprintf(buffer, "%s\n", message);
      send(sockfd, buffer, strlen(buffer), 0);
    }

    bzero(message, LENGTH);
    bzero(buffer, LENGTH + 32);
  }
  catch_ctrl_c_and_exit(2);
}

void recv_msg_handler() {
  char message[LENGTH] = {};
  while (1) {
    int receive = recv(sockfd, message, LENGTH, 0);
    if (receive > 0) {
      printf("%s", message);
      str_overwrite_stdout();
    } else if (receive == 0) {
      break;
    } else {
      // -1
    }
    memset(message, 0, sizeof(message));
  }
}
char* EncodeMessage (char mess[], int code) {
  char *EncodedMess;
  EncodedMess = (char *)malloc(BUFF_SIZE);

  if (code == 0) {
    *(EncodedMess) = '0';
    *(EncodedMess + 1) = '0';
  } 
  else if (code == 1) {
    *(EncodedMess) = '0';
    *(EncodedMess + 1) = '1';
  }
  else if (code == 2) {
    *(EncodedMess) = '0';
    *(EncodedMess + 1) = '2';
  }
  else if (code == 3) {
    *(EncodedMess) = '0';
    *(EncodedMess + 1) = '3';
  }
  else if (code == 4) {
    *(EncodedMess) = '0';
    *(EncodedMess + 1) = '4';
  }
  else if (code == 41) {
    *(EncodedMess) = '4';
    *(EncodedMess + 1) = '1';
  }
  else if (code == 42) {
    *(EncodedMess) = '4';
    *(EncodedMess + 1) = '2';
  }
  else if (code == 43) {
    *(EncodedMess) = '4';
    *(EncodedMess + 1) = '3';
  }
  else if(code == 5){
    *(EncodedMess) = '0';
    *(EncodedMess +1) = '5'; 
  }
  else if(code ==51){
     *(EncodedMess) = '5';
     *(EncodedMess+1) = '1';
  }
  *(EncodedMess + 2) = ' ';
  for (int i=0; i<strlen(mess); ++i) {
    *(EncodedMess + 3 + i) = mess[i];
  }
  *(EncodedMess + 3 + strlen(mess) + 1) = '\0';

  return EncodedMess;
}

char* DecodeMessage (char mess[]) {
  char *code;
  code = (char *)malloc(BUFF_SIZE);
  
  *(code) = mess[0];
  *(code + 1) = mess[1];
  *(code + 2) = '\0';

  return code;
}
int main(int argc, char **argv){
  if(argc != 2){
    printf("Usage: %s <port>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *ip = "127.0.0.1";
  int port = atoi(argv[1]);
  int nBit ;
  int state =0;
   char sendline[BUFF_SIZE+1] ,sendMess[BUFF_SIZE+1],recvline[BUFF_SIZE+1];
    char *text = calloc(1, 1);
  signal(SIGINT, catch_ctrl_c_and_exit);
   printf( "---Welcome---\n1. Login\n2. Exit\nYour choice: ");
  fgets(choice, 32, stdin);
  str_trim_lf(choice, strlen(choice));


  // if (strlen(name) > 32 || strlen(name) < 2){
  //   printf("Name must be less than 30 and more than 2 characters.\n");
  //   return EXIT_FAILURE;
  // }

  struct sockaddr_in server_addr;

  /* Socket settings */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip);
  server_addr.sin_port = htons(port);


  // Connect to Server
  int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (err == -1) {
    printf("ERROR: connect\n");
    return EXIT_FAILURE;
  }
  send(sockfd, choice, 32, 0);


  while (1) {
     
      while ((nBit = recv(sockfd, recvline, sizeof(recvline), 0)) != 0) {
      
        recvline[nBit] = '\0';
        printf("%s", recvline);
        if (strcmp(recvline, "01 Goodbye bro..") == 0) {
          printf("Disconnect to server...\n");
          // close(sockfd);
        }
       
         if(strcmp(recvline,"1")==0){
             // strcpy(sendMess,EncodeMessage(sendline,51));
                pthread_t send_msg_thread;
                if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
                  printf("ERROR: pthread\n");
                  return EXIT_FAILURE;
                }

                pthread_t recv_msg_thread;
                if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
                  printf("ERROR: pthread\n");
                  return EXIT_FAILURE;
                }
                  pthread_join(send_msg_thread,NULL);
                  pthread_join(recv_msg_thread,NULL);
                  while (1){
                        if(flag){
                          printf("\nBye\n");
                          break;
                        }
                  }

          }
          if(strcmp(recvline,"2")==0){
               pthread_t send_msg_thread;
                if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
                  printf("ERROR: pthread\n");
                  return EXIT_FAILURE;
                }

                pthread_t recv_msg_thread;
                if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
                  printf("ERROR: pthread\n");
                  return EXIT_FAILURE;
                }
                  pthread_join(send_msg_thread,NULL);
                  pthread_join(recv_msg_thread,NULL);
                  while (1){
                        if(flag){
                          printf("\nBye\n");
                          break;
                        }
                  }

          }
          
        if (fgets(sendline, BUFF_SIZE, stdin) != NULL) {
          text = realloc(text, strlen(text) + 1 + strlen(sendline));
          if (!text) {
            perror("No string");
            exit(5);
          }
          strcat(text, sendline);
          //printf("%s\n",sendline );
          // Reply to LoginAndExitMenu
          if (strcmp(DecodeMessage(recvline), "00") == 0) {
            strcpy(sendMess, EncodeMessage(sendline, 0));
          
          }
          // Reply username
          else if (strcmp(DecodeMessage(recvline), "01") == 0) {
            strcpy(sendMess, EncodeMessage(sendline, 1));
            
          }
          // Reply password
          else if (strcmp(DecodeMessage(recvline), "02") == 0) {
            strcpy(sendMess, EncodeMessage(sendline, 2));
           
          }
          // Reply menu-chat
          else if (strcmp(DecodeMessage(recvline), "03") == 0) {
            printf("%s\n",recvline );
            strcpy(sendMess, EncodeMessage(sendline, 3));
            
          }
          // Reply chat 1-1
          else if (strcmp(DecodeMessage(recvline), "04") == 0) {
            strcpy(sendMess, EncodeMessage(sendline, 4));
           
          }
          // // Reply chat 1-1 to receive and send message
          // else if (strcmp(DecodeMessage(recvline), "41") == 0) {
          //   strcpy(sendMess, EncodeMessage(sendline, 41));
          // }
          // // Reply send waiting message
          // else if (strcmp(DecodeMessage(recvline), "42") == 0) {
          //   strcpy(sendMess, EncodeMessage(sendline, 42));
          // }
          else if(strcmp(DecodeMessage(recvline),"05")==0){
              strcpy(sendMess,EncodeMessage(sendline,5));
          }
           send(sockfd, sendMess, strlen(sendMess), 0);
           bzero(sendMess,BUFF_SIZE+1);
        
        }

      
      
       bzero(recvline,BUFF_SIZE+1);
        }
    }
    close(sockfd);
  return EXIT_SUCCESS;
}
