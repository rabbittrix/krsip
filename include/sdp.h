#ifndef SDP_H
#define SDP_H

#include <stdbool.h>

// Estrutura para armazenar os detalhes de mídia (audio/video)
typedef struct {
    char *media_type;       // Tipo de mídia (audio, video)
    char *protocol;         // Protocolo de transporte (RTP/AVP, RTP/SAVP)
    int port;               // Porta de mídia
    int payload_type;       // Tipo de payload (ex: 0 para PCMU, 8 para PCMA)
    char *codec;            // Codec usado (ex: PCMU, PCMA, H264)
    int sample_rate;        // Taxa de amostragem (ex: 8000 para áudio)
    bool is_secure;         // Se a mídia usa SRTP
} sdp_media_t;

// Estrutura SDP completa (informações de sessão, mídia, etc.)
typedef struct {
    char *version;          // Versão SDP (normalmente "0")
    char *origin;           // Identificação da origem (username, session ID, etc.)
    char *session_name;     // Nome da sessão
    char *connection_info;  // Informação de conexão (endereço IP)
    char *time_active;      // Tempo em que a sessão é válida
    sdp_media_t *media;     // Detalhes de mídia (audio/video)
} sdp_t;

// Inicializa uma estrutura SDP com valores padrão
sdp_t* sdp_create(void);

// Libera os recursos de uma estrutura SDP
void sdp_destroy(sdp_t *sdp);

// Configura a versão da sessão SDP
void sdp_set_version(sdp_t *sdp, const char *version);

// Configura o campo "origin" da sessão SDP
void sdp_set_origin(sdp_t *sdp, const char *username, const char *session_id, const char *net_type, const char *addr_type, const char *unicast_address);

// Configura o nome da sessão
void sdp_set_session_name(sdp_t *sdp, const char *session_name);

// Configura a informação de conexão (endereço IP)
void sdp_set_connection_info(sdp_t *sdp, const char *net_type, const char *addr_type, const char *connection_address);

// Configura o tempo de duração da sessão
void sdp_set_time_active(sdp_t *sdp, const char *start_time, const char *stop_time);

// Configura os parâmetros de mídia (áudio/vídeo)
void sdp_set_media(sdp_t *sdp, const char *media_type, int port, const char *protocol, int payload_type, const char *codec, int sample_rate, bool is_secure);

// Serializa a estrutura SDP para uma string que pode ser enviada via SIP
char* sdp_serialize(const sdp_t *sdp);

#endif // SDP_H
