#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include  "client_server.h"
#include "user.h" 
#define MAX 25


clients *head = NULL, *curClient = NULL;
clients *MakeNode(int socket,char clientName[]){
    clients *node = (struct clients *)malloc(sizeof(struct clients));
    strcpy(node->clientName,clientName);
    node->sock = socket;
    node->next = NULL;
    return node;
}
void AddClientPort(int new_sock){
  clients *temp = head;
  puts("Connection oke..");
  int a = 1;
  if(head == NULL) {
    temp = (struct clients *)malloc(sizeof(struct clients));
    // strcpy(temp->clientName, clientName);
    strcpy(temp->clientName, "");
    temp->sock = new_sock;
    temp->next = NULL;
    head = temp;
    curClient = head;
  }
  else {
    clients *new = (struct clients *)malloc(sizeof(struct clients));
    strcpy(new->clientName, "");
    new->sock = new_sock;
    new->next = NULL;
    curClient->next = new;
    curClient = new;
  }
}
clients *insertClient(int socket,char  clientName[]){
    clients *node = MakeNode(socket,clientName);
    if(head == NULL){
        head = node;
        head->next = NULL;
    }
    node->next = head;
    head = node;
    return head;
}
void printClient() {
  clients *node = head;
  do {
    printf("%s\t%d\n", node->clientName, node->sock);
    node = node->next;
  } while(node != NULL);
}
int AddClientName(int socket, char clientName[]) {
  clients *temp = head;
  while (temp != NULL) {
    if (temp->sock == socket && strcmp(temp->clientName, "") == 0) {
      strcpy(temp->clientName, clientName);
      return 1;
    }

    temp = temp->next;
  }

  return 0;
}

clients* findOnlineUser(char username[]) {
  User* node = head;
  if (node == NULL)
    return NULL;
  else {
    do {
      if (strcmp(node->username, username) == 0) {
        return node;
      }
      node = node->next;
    } while (node != NULL);
  }

  return NULL;
}
clients *findClient(int socket){
  clients *node = head;
  if(node == NULL)
      return NULL;
  else{
      do{
          if(node->sock == socket)
             return node;
          node = node->next;
      }while(node != NULL);
    
  }
  return NULL;
}

void SendToAll(char msg[], char sender[]) {   
  clients *temp = head;
  int clients_socket;
  int byte;

  clients_socket  = socket(AF_INET, SOCK_STREAM, 0);

  if(clients_socket == -1)
    perror("Error On Socket(SendToAll)");

  while (temp != NULL) {
    if (strcmp(temp->clientName, sender) != 0) {
      byte = send(temp->sock, msg, strlen(msg), 0);
      printf("%s\n", msg);
      if (byte == -1)
        perror("Error on Send(SendToAll");
      else if (byte == 0)
        printf("Connection've been closed");
    }
    temp = temp->next;
  }
}


