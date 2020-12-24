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
#include "user.h"


User *root = NULL, *cur = NULL;
User* makeNewNode(User userInput) {
  User* newUser = (User*) malloc (sizeof(User));
  strcpy(newUser->username, userInput.username);
  strcpy(newUser->password, userInput.password);
  newUser->state = userInput.state;
  newUser->next = NULL;

  return newUser;
}

void insertNode(User userInput) {
  User* new = makeNewNode(userInput);
  if (root == NULL) {
    root = new;
    cur = root;
  } else {
    new->next = cur->next;
    cur->next = new;
    cur = new;
  }
}

User* findNode(char username[]) {
  User* node = root;
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

void printNode() {
  User *node = root;
  do {
    printf("%s\t%s\t%d \n", node->username, node->password, node->state);
    node = node->next;
  } while(node != NULL);
}

void freeList() {
  User* node = root ;
  if (root == NULL) {
    return;
  }

  while (node != NULL){
    root = root->next;
    free(node);
    node = root;
  }
}
