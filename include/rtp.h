#ifndef RTP_H
#define RTP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Versão do protocolo RTP
#define RTP_VERSION 2

// Tamanho do cabeçalho RTP básico em bytes (sem extensões)
#define RTP_HEADER_SIZE 12

// Definição de tipos para RTP
typedef struct {
    unsigned int version:2;      // Versão do protocolo (deve ser 2)
    unsigned int padding:1;      // Se há padding nos pacotes
    unsigned int extension:1;    // Se há cabeçalho de extensão
    unsigned int csrc_count:4;   // Número de identificadores CSRC (max 15)
    unsigned int marker:1;       // Bit marcador
    unsigned int payload_type:7; // Tipo de payload (determina o codec usado)
    uint16_t sequence_number;    // Número de sequência do pacote
    uint32_t timestamp;          // Timestamp do pacote
    uint32_t ssrc;               // Identificador do sincronizador de fluxo (SSRC)
    uint32_t csrc[15];           // Identificadores CSRC (máximo 15)
} rtp_header_t;

// Estrutura RTP para armazenar pacotes de mídia e metadados
typedef struct {
    rtp_header_t header;         // Cabeçalho RTP
    uint8_t *payload;            // Payload (dados de mídia)
    size_t payload_size;         // Tamanho do payload
} rtp_packet_t;

// Funções RTP

/**
 * @brief Inicializa um pacote RTP com valores padrões.
 * 
 * @param packet Ponteiro para a estrutura do pacote RTP.
 * @param payload_type Tipo do payload (determina o codec, ex: 0 para PCMU, 96 para H264).
 * @param ssrc Identificador do sincronizador de fluxo (SSRC).
 */
void rtp_init_packet(rtp_packet_t *packet, uint8_t payload_type, uint32_t ssrc);

/**
 * @brief Define o número de sequência do pacote RTP.
 * 
 * @param packet Ponteiro para o pacote RTP.
 * @param sequence_number Número de sequência a ser definido.
 */
void rtp_set_sequence_number(rtp_packet_t *packet, uint16_t sequence_number);

/**
 * @brief Define o timestamp do pacote RTP.
 * 
 * @param packet Ponteiro para o pacote RTP.
 * @param timestamp Timestamp a ser definido.
 */
void rtp_set_timestamp(rtp_packet_t *packet, uint32_t timestamp);

/**
 * @brief Define o payload do pacote RTP.
 * 
 * @param packet Ponteiro para o pacote RTP.
 * @param payload Buffer de dados de mídia.
 * @param size Tamanho dos dados de mídia.
 */
void rtp_set_payload(rtp_packet_t *packet, const uint8_t *payload, size_t size);

/**
 * @brief Serializa o pacote RTP para enviar pela rede.
 * 
 * @param packet Ponteiro para o pacote RTP.
 * @param buffer Buffer de saída para armazenar os dados serializados.
 * @param buffer_size Tamanho do buffer de saída.
 * @return size_t O número de bytes que foram serializados no buffer.
 */
size_t rtp_serialize(const rtp_packet_t *packet, uint8_t *buffer, size_t buffer_size);

/**
 * @brief Desserializa os dados de um buffer para um pacote RTP.
 * 
 * @param buffer Buffer contendo os dados serializados.
 * @param buffer_size Tamanho dos dados no buffer.
 * @param packet Ponteiro para o pacote RTP onde os dados serão armazenados.
 * @return bool Retorna true se a desserialização for bem-sucedida.
 */
bool rtp_deserialize(const uint8_t *buffer, size_t buffer_size, rtp_packet_t *packet);

/**
 * @brief Libera o payload de um pacote RTP.
 * 
 * @param packet Ponteiro para o pacote RTP.
 */
void rtp_free_payload(rtp_packet_t *packet);

#endif // RTP_H
