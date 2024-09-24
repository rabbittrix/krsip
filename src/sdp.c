#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdp.h"

// Função auxiliar para duplicar strings
static char* str_dup(const char *str) {
    if (!str) return NULL;
    char *dup = (char*) malloc(strlen(str) + 1);
    if (dup) {
        strcpy(dup, str);
    }
    return dup;
}

// Inicializa uma estrutura SDP com valores padrão
sdp_t* sdp_create(void) {
    sdp_t *sdp = (sdp_t*) malloc(sizeof(sdp_t));
    if (!sdp) return NULL;
    
    sdp->version = str_dup("0");  // Versão SDP padrão
    sdp->origin = NULL;
    sdp->session_name = str_dup("SDP Session");
    sdp->connection_info = NULL;
    sdp->time_active = str_dup("0 0");
    sdp->media = NULL;
    
    return sdp;
}

// Libera os recursos de uma estrutura SDP
void sdp_destroy(sdp_t *sdp) {
    if (!sdp) return;
    
    free(sdp->version);
    free(sdp->origin);
    free(sdp->session_name);
    free(sdp->connection_info);
    free(sdp->time_active);
    
    if (sdp->media) {
        free(sdp->media->media_type);
        free(sdp->media->protocol);
        free(sdp->media->codec);
        free(sdp->media);
    }

    free(sdp);
}

// Configura a versão da sessão SDP
void sdp_set_version(sdp_t *sdp, const char *version) {
    if (!sdp) return;
    free(sdp->version);
    sdp->version = str_dup(version);
}

// Configura o campo "origin" da sessão SDP
void sdp_set_origin(sdp_t *sdp, const char *username, const char *session_id, const char *net_type, const char *addr_type, const char *unicast_address) {
    if (!sdp) return;
    char origin[256];
    snprintf(origin, sizeof(origin), "%s %s %s %s %s", username, session_id, net_type, addr_type, unicast_address);
    free(sdp->origin);
    sdp->origin = str_dup(origin);
}

// Configura o nome da sessão
void sdp_set_session_name(sdp_t *sdp, const char *session_name) {
    if (!sdp) return;
    free(sdp->session_name);
    sdp->session_name = str_dup(session_name);
}

// Configura a informação de conexão (endereço IP)
void sdp_set_connection_info(sdp_t *sdp, const char *net_type, const char *addr_type, const char *connection_address) {
    if (!sdp) return;
    char connection_info[128];
    snprintf(connection_info, sizeof(connection_info), "%s %s %s", net_type, addr_type, connection_address);
    free(sdp->connection_info);
    sdp->connection_info = str_dup(connection_info);
}

// Configura o tempo de duração da sessão
void sdp_set_time_active(sdp_t *sdp, const char *start_time, const char *stop_time) {
    if (!sdp) return;
    char time_active[64];
    snprintf(time_active, sizeof(time_active), "%s %s", start_time, stop_time);
    free(sdp->time_active);
    sdp->time_active = str_dup(time_active);
}

// Configura os parâmetros de mídia (áudio/vídeo)
void sdp_set_media(sdp_t *sdp, const char *media_type, int port, const char *protocol, int payload_type, const char *codec, int sample_rate, bool is_secure) {
    if (!sdp) return;
    
    if (!sdp->media) {
        sdp->media = (sdp_media_t*) malloc(sizeof(sdp_media_t));
        if (!sdp->media) return;
    }
    
    sdp->media->media_type = str_dup(media_type);
    sdp->media->protocol = str_dup(protocol);
    sdp->media->port = port;
    sdp->media->payload_type = payload_type;
    sdp->media->codec = str_dup(codec);
    sdp->media->sample_rate = sample_rate;
    sdp->media->is_secure = is_secure;
}

// Serializa a estrutura SDP para uma string que pode ser enviada via SIP
char* sdp_serialize(const sdp_t *sdp) {
    if (!sdp) return NULL;

    char *serialized = (char*) malloc(1024); // Buffer para a string SDP
    if (!serialized) return NULL;

    snprintf(serialized, 1024,
        "v=%s\r\n"
        "o=%s\r\n"
        "s=%s\r\n"
        "c=%s\r\n"
        "t=%s\r\n"
        "m=%s %d %s %d\r\n"
        "a=rtpmap:%d %s/%d\r\n"
        "a=recvonly\r\n%s",
        sdp->version,
        sdp->origin,
        sdp->session_name,
        sdp->connection_info,
        sdp->time_active,
        sdp->media->media_type,
        sdp->media->port,
        sdp->media->protocol,
        sdp->media->payload_type,
        sdp->media->payload_type,
        sdp->media->codec,
        sdp->media->sample_rate,
        sdp->media->is_secure ? "a=crypto:1 AES_CM_128_HMAC_SHA1_80 inline:..." : "");

    return serialized;
}
