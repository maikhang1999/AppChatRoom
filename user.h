#define MAX 25

// User define
typedef struct User {
  char username[MAX];
  char password[MAX];
  int state;
  struct User *next;
} User;


User* makeNewNode(User userInput);
void insertNode(User userInput);
User* findNode(char username[]);
void printNode();
void freeList(); 
