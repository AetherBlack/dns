#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include "getopt.h"
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable:4996) 

#define DEFAULT_NAME_SERVER "8.8.8.8"
#define DNS_PORT 53

int main(int argc, char* argv[])
{
    int opt;
    int Hopt = 0;
    /* DOMAIN TO SEARCH */
    char* DOMAIN = NULL;
    char* DNS_SERVER = NULL;
    /* DEFAULT NS */
    DNS_SERVER = DEFAULT_NAME_SERVER;

    /* GET ARGUMENT */
    while ((opt = getopt(argc, argv, ":s:help:H:")) != -1)
    {
        switch (opt)
        {
            /* EXAMPLE */
        case 'h':
            printf("%s [-s NAME_SERVER] -H DNS_HOST\n\t-s NAME_SERVER -> default value: %s\n", argv[0], DEFAULT_NAME_SERVER);
            return 0;
            /* NEW NAME SERVER */
        case 's':
            printf("> server\nServer: %s\n", optarg);
            DNS_SERVER = optarg;
            break;
            /* HOST TO RESOLVE */
        case 'H':
            DOMAIN = optarg;
            Hopt = 1;
            break;
            /* UNKNOWN OPTION */
        case '?':
            printf("Unknow option: %c\n", optopt);
            return 1;
        }
    }

    /* CHECK IF HOST IS SET */
    if (!Hopt)
    {
        printf("-H option need to be set !\n");
        return 1;
    }
    printf("> %s\nAddress: ", DOMAIN);

    /* VAR FOR SOCKET */
    WSADATA WSAData;
    SOCKET sock;
    SOCKADDR_IN sin;

    /* BUFFER FOR DATA (PACKET SIZE MAX) */
    char buffer[65536];

    if (WSAStartup(MAKEWORD(2, 0), &WSAData))
    {
        printf("Connection Error !");
        return 1;
    }

    /* UDP CONNECTION */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* NAME SERVER TO QUERY */
    sin.sin_addr.s_addr = inet_addr(DNS_SERVER);
    sin.sin_family = AF_INET;
    /* DNS PORT */
    sin.sin_port = htons(DNS_PORT);

    /* CONNECT TO THE HOST */
    connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    /* DNS HEADERS */
    int DNS_HEADERS_LENGTH = 13;
    char DNS_HEADERS[13] = {
                                '\x00', '\x02', //Identifier
                                '\x01',         //Request, query, no troncate, recursif
                                '\x00',         //No recursif, always 0, no error
                                '\x00', '\x01', //Number of question
                                '\x00', '\x00', //Number of answer
                                '\x00', '\x00', //Authority
                                '\x00', '\x00', //Additionnal data
                                '\x06'          //Futur length of the domain
    };

    /* DNS FOOTER */
    int DNS_FOOTER_LENGTH = 5;
    char DNS_FOOTER[5] = {
                            '\x00',         //End of string
                            '\x00', '\x01', //type A DNS query
                            '\x00', '\x01'  //class IN IP Addr
    };

    /* HOST NAME TO QUERY */
    int DNS_DOMAIN_LENGTH = strnlen(DOMAIN, 255);

    /* TOTAL LENGTH */
    int QUERY_LENGTH = DNS_HEADERS_LENGTH + DNS_DOMAIN_LENGTH + DNS_FOOTER_LENGTH;


    int index = DNS_DOMAIN_LENGTH;
    int dns_not_root_domain = -1;

    /* REPLACE DOT BY length of subdomain */
    while (index != 0)
    {
        /* IF DOT REPLACE WITH length of subdomain */
        if (DOMAIN[index] == 0x2e)//0x2e = 46 = .
        {
            DOMAIN[index] = dns_not_root_domain;
            dns_not_root_domain = 0;
        }
        else
            dns_not_root_domain++;

        index--;
    }

    /* CHANGE THE LENGTH OF THE DOMAIN (DNS DATA) */
    dns_not_root_domain++;
    DNS_HEADERS[12] = dns_not_root_domain;


    char query[512];// MAX LENGTH PACKET cf: RFC

    /* ADD HEADER IN query */
    memcpy(query, DNS_HEADERS, DNS_HEADERS_LENGTH);
    /* ADD DOMAIN */
    memcpy(query + DNS_HEADERS_LENGTH, DOMAIN, DNS_DOMAIN_LENGTH);
    /* ADD FOOTER IN query */
    memcpy(query + (DNS_HEADERS_LENGTH + DNS_DOMAIN_LENGTH), DNS_FOOTER, DNS_FOOTER_LENGTH);

    /* SEND THE PACKET */
    send(sock, query, QUERY_LENGTH, 0);

    /* START RESPONSE TIME */
    clock_t start, stop;
    start = clock();

    /* RECV THE DATA INTO buffer */
    recv(sock, buffer, sizeof(buffer), 0);

    /* END RESPONSE */
    stop = clock();

    /* SEEK THE INDEX TO THE IP ADDR */
    int SEEK_IPADDR = (34 + DNS_DOMAIN_LENGTH) - 4;
    /* VARIABLE TO CONVERT UNSIGNED TO SIGNED */
    int val;
    index = SEEK_IPADDR;

    /* GET IP ADDR */
    while (index != (SEEK_IPADDR + 4))
    {
        /* GET INT */
        val = buffer[index];
        /* GENERATE IP ADDR */
        if (index == (SEEK_IPADDR + 3))
            /* CHECK IF UNSIGNED */
            if (val < 0)
                printf("%d\n", 256 + val);
            else
                printf("%d\n", val);
        else
            /* CHECK IF UNSIGNED */
            if (val < 0)
                printf("%d.", 256 + val);
            else
                printf("%d.", val);

        index++;
    }

    /* PRINT RESPONSE TIME */
    printf("Response Time: %d ms\n", (stop - start));

    /* CLOSE SOCKET */
    closesocket(sock);
    WSACleanup();
    return 0;
}
