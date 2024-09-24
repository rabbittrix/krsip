#import <Foundation/Foundation.h>
#import <CFNetwork/CFNetwork.h>
#import "stun_turn_ice.h"

// Função para enviar uma solicitação de binding STUN no iOS
bool stun_send_binding_request(NSString *serverIP, uint16_t serverPort) {
    CFSocketRef socket = CFSocketCreate(kCFAllocatorDefault, PF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, NULL, NULL);
    if (!socket) {
        NSLog(@"Socket creation failed");
        return false;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, [serverIP UTF8String], &serverAddr.sin_addr);

    stun_header_t request;
    memset(&request, 0, sizeof(request));
    request.type = htons(STUN_BINDING_REQUEST);
    request.length = 0;
    request.magic_cookie = htonl(STUN_MAGIC_COOKIE);
    memset(request.transaction_id, 0, sizeof(request.transaction_id));

    NSData *data = [NSData dataWithBytes:&request length:sizeof(request)];
    ssize_t sentBytes = CFSocketSendData(socket, (CFDataRef)data, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
    
    if (sentBytes < 0) {
        NSLog(@"Send failed");
        CFSocketInvalidate(socket);
        CFRelease(socket);
        return false;
    }

    CFSocketInvalidate(socket);
    CFRelease(socket);
    return true;
}

// Função para processar a resposta do servidor STUN
bool stun_process_response(const uint8_t *buffer, size_t buffer_size) {
    if (buffer_size < STUN_MESSAGE_HEADER_SIZE) {
        return false; // Pacote muito pequeno
    }

    stun_header_t *response = (stun_header_t *)buffer;
    uint16_t type = ntohs(response->type);
    
    if (type == STUN_BINDING_RESPONSE) {
        NSLog(@"Received STUN Binding Response");
        return true;
    } else if (type == STUN_BINDING_ERROR_RESPONSE) {
        NSLog(@"Received STUN Binding Error Response");
        return false;
    }

    return false; // Tipo de mensagem não suportado
}

// Funções TURN e ICE podem ser implementadas de forma semelhante


