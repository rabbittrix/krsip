#ifndef STUN_TURN_ICE_H
#define STUN_TURN_ICE_H

#include <stdint.h>
#include <stdbool.h>

// Definições de tipos para STUN
#define STUN_MESSAGE_HEADER_SIZE 20
#define STUN_MAGIC_COOKIE 0x2112A442

typedef enum {
    STUN_BINDING_REQUEST = 0x0001,
    STUN_BINDING_RESPONSE = 0x0101,
    STUN_BINDING_ERROR_RESPONSE = 0x0111,
    // Adicione outros tipos de mensagens STUN conforme necessário
} stun_message_type_t;

// Estrutura do cabeçalho STUN
typedef struct {
    uint16_t type;                // Tipo da mensagem
    uint16_t length;              // Tamanho do payload
    uint32_t magic_cookie;        // Cookie mágico
    uint8_t transaction_id[12];   // ID da transação
} stun_header_t;

// Estruturas TURN
typedef enum {
    TURN_ALLOCATE_REQUEST = 0x0003,
    TURN_ALLOCATE_RESPONSE = 0x0103,
    TURN_ALLOCATE_ERROR_RESPONSE = 0x0113,
    // Adicione outros tipos de mensagens TURN conforme necessário
} turn_message_type_t;

// Estrutura TURN
typedef struct {
    stun_header_t header;         // Cabeçalho STUN (reutilizando a estrutura)
    // Campos específicos de TURN podem ser adicionados aqui
} turn_message_t;

// Estruturas ICE
typedef struct {
    char candidate[256];          // Candidato ICE
    int priority;                 // Prioridade do candidato
    char foundation[32];          // Fundação do candidato
} ice_candidate_t;

// Funções STUN

/**
 * @brief Envia uma solicitação de binding STUN.
 * 
 * @param server_ip Endereço IP do servidor STUN.
 * @param server_port Porta do servidor STUN.
 * @return bool Retorna true se a solicitação for enviada com sucesso.
 */
bool stun_send_binding_request(const char *server_ip, uint16_t server_port);

/**
 * @brief Processa a resposta do servidor STUN.
 * 
 * @param buffer Buffer que contém a resposta STUN.
 * @param buffer_size Tamanho do buffer.
 * @return bool Retorna true se a resposta for processada com sucesso.
 */
bool stun_process_response(const uint8_t *buffer, size_t buffer_size);

// Funções TURN

/**
 * @brief Envia uma solicitação de alocação TURN.
 * 
 * @param server_ip Endereço IP do servidor TURN.
 * @param server_port Porta do servidor TURN.
 * @return bool Retorna true se a solicitação for enviada com sucesso.
 */
bool turn_send_allocate_request(const char *server_ip, uint16_t server_port);

/**
 * @brief Processa a resposta do servidor TURN.
 * 
 * @param buffer Buffer que contém a resposta TURN.
 * @param buffer_size Tamanho do buffer.
 * @return bool Retorna true se a resposta for processada com sucesso.
 */
bool turn_process_response(const uint8_t *buffer, size_t buffer_size);

// Funções ICE

/**
 * @brief Adiciona um candidato ICE.
 * 
 * @param candidate O candidato a ser adicionado.
 * @param priority A prioridade do candidato.
 * @return bool Retorna true se o candidato for adicionado com sucesso.
 */
bool ice_add_candidate(const char *candidate, int priority);

/**
 * @brief Realiza o processo de conexão ICE.
 * 
 * @return bool Retorna true se a conexão for estabelecida com sucesso.
 */
bool ice_connect();

#endif 
