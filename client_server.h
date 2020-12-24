#define MAX 25
// Client define with name and socket
typedef struct clients {
  char clientName[MAX];   
  int sock;
  struct clients *next;
} clients;

void AddClientPort(int new_sock);
int AddClientName(int socket, char clientName[]);
clients* findOnlineUser(char username[]);
void SendToAll(char msg[], char sender[]); 
clients *insertClient(int socket,char  clientName[]);
void printClient() ;
clients *MakeNode(int socket,char clientName[]);
clients *findClient(int socket);