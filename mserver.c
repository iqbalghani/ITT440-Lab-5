#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h> //gia to exit()
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main()
{
int welcomeSocket, newSocket;
char buffer[1024];
char name[32];

struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;

welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(7891);
serverAddr.sin_addr.s_addr = inet_addr("192.168.56.104");
memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

bind(welcomeSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

if (listen(welcomeSocket, 5) == 0)
    printf("Listening\n");
else
    printf("Error\n");

addr_size = sizeof serverStorage;

while (1) {
    newSocket = accept(welcomeSocket, (struct sockaddr*)&serverStorage, &addr_size);
    if (newSocket < 0) {
        perror("Socket");
        continue;
    }
    int pid = fork();
    if (pid < 0) {
        perror("Could not fork");
        exit(1);
    }
    if (pid == 0) {
        // This is the child process
        close(welcomeSocket);

        recv(newSocket, name, sizeof(name), 0);
        int i = 0;

        while (1) {

            recv(newSocket, buffer, sizeof(buffer), 0);
            if (strcmp(buffer, "exit\n") == 0) {
                printf("%s logout\n", name);
                break;
            }
            else if ((strcmp(buffer, "\n") < 0 || strcmp(buffer, "\n") > 0))
                printf("%s:%s", name, buffer);
            else if ((strcmp(buffer, "\n") == 0) && i == 0)
                printf("%s joined\n", name);
        }

        close(newSocket);
        exit(0);
    }
    else {
        // This is the parent

        close(newSocket);
    }
}

close(welcomeSocket);

return 0;
}
