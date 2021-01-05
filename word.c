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
#include "word.h"


Word*rootWord = NULL, *curWord = NULL;
Word* makeNewNodeWord(Word wordInput) {
  Word* newWord = (Word*) malloc (sizeof(Word));
  strcpy(newWord->word, wordInput.word);
  newWord->length = wordInput.length;
  newWord->next = NULL;

  return newWord;
}

void insertNodeWord(Word wordInput) {
  Word* new = makeNewNodeWord(wordInput);
  if (rootWord == NULL) {
    rootWord = new;
    curWord = rootWord;
  } else {
    new->next = curWord->next;
    curWord->next = new;
    curWord= new;
  }
}

Word* findNodeWord(char word[]) {
  Word* node = rootWord;
  if (node == NULL)
    return NULL;
  else {
    do {
      if (strcmp(node->word, word) == 0) {
        return node;
      }
      node = node->next;
    } while (node != NULL);
  }

  return NULL;
}
Word* findOneWord(char word[]){
    Word* node = rootWord;
  if (node == NULL){
    return NULL;
  }
  else {
    
    do {

      if (strncmp(node->word, word,strlen(word)) == 0) {
        
        return node;
      }
      node = node->next;
    } while (node != NULL);
  }

  return NULL;
}
void printNodeWord() {
  Word *node = rootWord;
  do {
    printf("%s%d \n", node->word, node->length);
    node = node->next;
  } while(node != NULL);
}

void freeListWord() {
  Word* node = rootWord ;
  if (rootWord == NULL) {
    return;
  }

  while (node != NULL){
    rootWord = rootWord->next;
    free(node);
    node = rootWord;
  }
}
