#include "stun_turn_ice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#endif
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h> // Add this line to define ssize_t
#ifdef _WIN32
typedef int ssize_t; // Define ssize_t for Windows
#endif

#define STUN_BINDING_REQUEST 0x0001
#define STUN_BINDING_RESPONSE 0x0101
#define STUN_BINDING_ERROR_RESPONSE 0x0111
#define STUN_MAGIC_COOKIE 0x2112A442
#define STUN_MESSAGE_HEADER_SIZE 20

#define TURN_ALLOCATE_REQUEST 0x0003
#define TURN_ALLOCATE_RESPONSE 0x0103
#define TURN_ALLOCATE_ERROR_RESPONSE 0x0113

typedef struct {
    uint16_t type;
    uint16_t length;
    uint32_t magic_cookie;
    uint8_t transaction_id[12];
} stun_header_t;

typedef struct {
    stun_header_t header;
    // Add other TURN-specific fields here if needed
} turn_message_t;

typedef struct {
    uint16_t type;
    uint16_t length;
    uint32_t magic_cookie;
    uint8_t transaction_id[12];
} stun_header_t;
bool stun_send_binding_request(const char *server_ip, uint16_t server_port) {
    WSADATA wsaData;
    int sockfd;
    struct sockaddr_in server_addr;
    stun_header_t request;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");

    WSACleanup();
    return false;
}

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sockfd);
        return false;
    }

    memset(&request, 0, sizeof(request));
    request.type = htons(STUN_BINDING_REQUEST);
    request.length = 0;
        ssize_t sent_bytes = sendto(sockfd, (char*)&request, sizeof(request), 0,
                                     (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (sent_bytes < 0) {
            perror("Send failed");
            closesocket(sockfd);
            WSACleanup();
            return false;
        }
    
        closesocket(sockfd);
        WSACleanup();
        return true;

    return false;
}

bool stun_process_response(const uint8_t *buffer, size_t buffer_size) {
    if (buffer_size < STUN_MESSAGE_HEADER_SIZE) {
        return false; 
    }

    stun_header_t *response = (stun_header_t *)buffer;
    uint16_t type = ntohs(response->type);
    
    if (type == STUN_BINDING_RESPONSE) {
        printf("Received STUN Binding Response\n");
        return true;
    } else if (type == STUN_BINDING_ERROR_RESPONSE) {
        printf("Received STUN Binding Error Response\n");
        return false;
    }

    return false; 
}

bool turn_send_allocate_request(const char *server_ip, uint16_t server_port) {
    int sockfd;
    struct sockaddr_in server_addr;
    turn_message_t request;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sockfd);
        return false;
    }

    memset(&request, 0, sizeof(request));
    request.header.type = htons(TURN_ALLOCATE_REQUEST);
    request.header.length = 0; 
    request.header.magic_cookie = htonl(STUN_MAGIC_COOKIE);
    memset(request.header.transaction_id, 0, sizeof(request.header.transaction_id));

    ssize_t sent_bytes = sendto(sockfd, (char*)&request, sizeof(request), 0,
                                 (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (sent_bytes < 0) {
        perror("Send failed");
        close(sockfd);
        return false;
    }

    close(sockfd);
    return true;
}

bool turn_process_response(const uint8_t *buffer, size_t buffer_size) {
    if (buffer_size < STUN_MESSAGE_HEADER_SIZE) {
        return false; 
    }

    turn_message_t *response = (turn_message_t *)buffer;
    uint16_t type = ntohs(response->header.type);
    
    if (type == TURN_ALLOCATE_RESPONSE) {
        printf("Received TURN Allocate Response\n");
        return true;
    } else if (type == TURN_ALLOCATE_ERROR_RESPONSE) {
        printf("Received TURN Allocate Error Response\n");
        return false;
    }

    return false; 
}

bool ice_add_candidate(const char *candidate, int priority) {
    printf("Adding ICE candidate: %s with priority %d\n", candidate, priority);
    return true; 
}

bool ice_connect() {
    printf("Establishing ICE connection\n");
    return true; 
}
