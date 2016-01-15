#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255
#define URL_NAME_SIZE       255

int main(int argc, char *argv[])
{
    int hSocket;                 /* handle to socket */
    struct hostent *pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    char url[URL_NAME_SIZE];

    if (argc < 6)
    {
        printf("\nUsage: download host-name host-port path -c or -d\n");
        return 0;
    }
    else
    {
        strcpy(strHostName, argv[1]);
        nHostPort = atoi(argv[2]);
    }

    printf("\nMaking a socket");
    /* make a socket */
    hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (hSocket == SOCKET_ERROR)
    {
        printf("\nCould not make a socket\n");
        return 0;
    }

    /* get IP address from name */
    pHostInfo = gethostbyname(strHostName);
    /* copy address into long */
    memcpy(&nHostAddress, pHostInfo->h_addr, pHostInfo->h_length);

    /* fill address struct */
    Address.sin_addr.s_addr = nHostAddress;
    Address.sin_port = htons(nHostPort);
    Address.sin_family = AF_INET;

    printf("\nConnecting to %s (%X) on port %d", strHostName, nHostAddress, nHostPort);

    /* connect to host */
    if (connect(hSocket, (struct sockaddr *) &Address, sizeof(Address)) == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
#define MAXGET 1000
    // Create HTTP Message
    char  *message = (char *) malloc(MAXGET);
    sprintf(message, "GET %s HTTP/1.1\r\nHost:%s:%s\r\n\r\n", argv[3], argv[1], argv[2]);
    // Send HTTP on the socket
    printf("Request: %s\n", message);
    write(hSocket, message, strlen(message));
    // Rease Response back from socket
    nReadAmount = read(hSocket, pBuffer, BUFFER_SIZE);
    printf("Response: %s\n", pBuffer);

    /* read from socket into buffer
    ** number returned by read() and write() is the number of bytes
    ** read or written, with -1 being that an error occured */
    printf("\nReceived \"%s\" from server\n", pBuffer);
    /* write what we received back to the server */

    printf("\nWriting \"%s\" to server", pBuffer);

    printf("\nClosing socket\n");
    /* close socket */
    if (close(hSocket) == SOCKET_ERROR)
    {
        printf("\nCould not close socket\n");
        return 0;
    }
}
