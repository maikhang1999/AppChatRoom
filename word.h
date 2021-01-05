#define MAX 25	

// Word define
typedef struct  Word{
  char word[MAX];
  int length;
  struct Word *next;
} Word;


Word* makeNewNodeWord(Word wordInput);
void insertNodeWord(Word wordInput);
Word* findNodeWord(char word[]);
Word* findOneWord(char word[]);
void printNodeWord();
void freeListWord(); 

