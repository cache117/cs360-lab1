#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iosfwd>


#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255

int main(int argc, char *argv[])
{
    int option;
    int hSocket;                 /* handle to socket */
    struct hostent *pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;
    int count = 0;
    int printHeaders = 0;
    char *path;

    if (argc < 4)
    {
        printf("\nUsage: download host-name host-port path [-c or -d]\n");
        return 0;
    }
    else
    {
        while ((option = getopt (argc, argv, "c:d")) != -1) {
            switch (option) {
                case 'c':
                    count = atoi(optarg);
                    break;
                case 'd':
                    printHeaders = 1;
                    break;
                default:
                    abort();
            }
        }
        printf ("count = %d, printHeaders = %d\n", count, printHeaders);

        strcpy(strHostName, argv[optind++]);
        nHostPort = atoi(argv[optind++]);
        path = argv[optind];
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


    /* connect to host */
    if (connect(hSocket, (struct sockaddr *) &Address, sizeof(Address)) == SOCKET_ERROR)
    {
        printf("\nCould not connect to host\n");
        return 0;
    }
#define MAXGET 1000
    // Create HTTP Message
    char  *message = (char *) malloc(MAXGET);
    sprintf(message, "GET %s HTTP/1.1\r\nHost:%s:%d\r\n\r\n", path, strHostName, nHostPort);
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