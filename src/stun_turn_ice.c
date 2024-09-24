#include "stun_turn_ice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> 
#ifdef __unix__
#include <unistd.h>
#endif
#include <sys/socket.h> 

bool stun_send_binding_request(const char *server_ip, uint16_t server_port) {
    int sockfd;
    struct sockaddr_in server_addr;
    stun_header_t request;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
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
    memset(request.transaction_id, 0, sizeof(request.transaction_id)); 

    ssize_t sent_bytes = sendto(sockfd, &request, sizeof(request), 0,
                                 (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (sent_bytes < 0) {
        perror("Send failed");
        close(sockfd);
        return false;
    }

    close(sockfd);
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
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    memset(&request, 0, sizeof(request));
    request.header.type = htons(TURN_ALLOCATE_REQUEST);
    request.header.length = 0; 
    request.header.magic_cookie = htonl(STUN_MAGIC_COOKIE);
    memset(request.header.transaction_id, 0, sizeof(request.header.transaction_id)); // Zera o ID da transação

    ssize_t sent_bytes = sendto(sockfd, &request, sizeof(request), 0,
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
    uint16_t type = ntohs(response.header.type);
    
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
