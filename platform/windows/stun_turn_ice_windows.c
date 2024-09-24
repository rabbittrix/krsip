#include "stun_turn_ice.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define STUN_MAGIC_COOKIE 0x2112A442
#define STUN_MESSAGE_HEADER_SIZE sizeof(stun_header_t)

// Define TURN message types
#define TURN_ALLOCATE_REQUEST 0x0003
#define TURN_ALLOCATE_RESPONSE 0x0103
#define TURN_ALLOCATE_ERROR_RESPONSE 0x0113

// Define the turn_message_t structure
typedef struct {
    stun_header_t header;
    // Add other TURN-specific fields here if needed
} turn_message_t;

// Define STUN message types
#define STUN_BINDING_REQUEST 0x0001
#define STUN_BINDING_RESPONSE 0x0101
#define STUN_BINDING_ERROR_RESPONSE 0x0111

// Define the stun_header_t structure
typedef struct {
    uint16_t type;
    uint16_t length;
    uint32_t magic_cookie;
    uint8_t transaction_id[12];
} stun_header_t;

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

bool initialize_winsock() {
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return false;
    }
    return true;
}

bool stun_send_binding_request(const char *server_ip, uint16_t server_port) {
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    stun_header_t request;

    if (!initialize_winsock()) {
        return false;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    memset(&request, 0, sizeof(request));
    request.type = htons(STUN_BINDING_REQUEST);
    request.length = 0; 
    request.magic_cookie = htonl(STUN_MAGIC_COOKIE);
    memset(request.transaction_id, 0, sizeof(request.transaction_id)); // Zera o ID da transação

    int sent_bytes = sendto(sockfd, (char*)&request, sizeof(request), 0,
                                 (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (sent_bytes == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return false;
    }

    closesocket(sockfd);
    WSACleanup();
    return true;
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
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    turn_message_t request;

    if (!initialize_winsock()) {
        return false;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    memset(&request, 0, sizeof(request));
    request.header.type = htons(TURN_ALLOCATE_REQUEST);
    request.header.length = 0; 
    request.header.magic_cookie = htonl(STUN_MAGIC_COOKIE);
    memset(request.header.transaction_id, 0, sizeof(request.header.transaction_id));

    int sent_bytes = sendto(sockfd, (char*)&request, sizeof(request), 0,
                                 (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (sent_bytes == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return false;
    }

    closesocket(sockfd);
    WSACleanup();
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
