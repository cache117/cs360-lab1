#include <sys/types.h>

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#else

#include <winsock2.h>

#endif

#include <unistd.h>
#include <stdio.h>

#ifndef WIN32
#define SOCKET_ERROR        -1
#endif
#define BUFFER_SIZE         100
#define HOST_NAME_SIZE      255

int main(int argc, char *argv[])
{
    int option;
    int printHeaders = 0;
    int count = 0;
    char *path;
    int hSocket;                 /* handle to socket */
    struct hostent *pHostInfo;   /* holds info about a machine */
    struct sockaddr_in Address;  /* Internet socket address stuct */
    long nHostAddress;
    char pBuffer[BUFFER_SIZE];
    unsigned nReadAmount;
    char strHostName[HOST_NAME_SIZE];
    int nHostPort;


    if (argc < 4)
    {
        printf("\nUsage: download host-name host-port path [-c | -d]  \n");
        return 0;
    }
    else
    {
        while ((option = getopt(argc, argv, "c:d")) != -1)
        {
            switch (option)
            {
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
        printf("count = %d, printHeaders = %d\n", count, printHeaders);

        strcpy(strHostName, argv[optind++]);
        nHostPort = atoi(argv[optind++]);
        path = argv[optind];
    }

    printf("\nMaking a socket");
    /* make a socket */
#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
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
    char *message = (char *) malloc(MAXGET);
    sprintf(message, "GET %s HTTP/1.1\r\nHost:%d:%s\r\n\r\n", path, nHostPort, strHostName);
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
