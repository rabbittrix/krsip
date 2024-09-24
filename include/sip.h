#ifndef SIP_H
#define SIP_H

#include <stdint.h>
#include <stdbool.h>

// SIP Status Codes
#define SIP_STATUS_OK 200
#define SIP_STATUS_RINGING 180
#define SIP_STATUS_CALL_FORWARDED 181
#define SIP_STATUS_UNAUTHORIZED 401
#define SIP_STATUS_NOT_FOUND 404
#define SIP_STATUS_BUSY 486
#define SIP_STATUS_REQUEST_TIMEOUT 408
#define SIP_STATUS_SERVER_UNAVAILABLE 503

// SIP Methods
typedef enum {
    SIP_METHOD_INVITE,
    SIP_METHOD_BYE,
    SIP_METHOD_CANCEL,
    SIP_METHOD_REGISTER,
    SIP_METHOD_OPTIONS,
    SIP_METHOD_ACK,
    SIP_METHOD_INFO,
    SIP_METHOD_MESSAGE
} sip_method_t;

// Estrutura para configuração SIP
typedef struct {
    const char *username;      // Nome de usuário SIP
    const char *domain;        // Domínio SIP (ex: sip.domain.com)
    const char *password;      // Senha SIP
    const char *outbound_proxy;// Proxy de saída
    uint16_t port;             // Porta SIP (padrão 5060)
    bool use_tls;              // Usar TLS para transporte SIP?
    bool use_ipv6;             // Usar IPv6?
} sip_config_t;

// Estrutura para uma sessão SIP
typedef struct {
    const char *local_uri;     // URI local SIP (ex: sip:user@domain.com)
    const char *remote_uri;    // URI remota SIP (ex: sip:remote@domain.com)
    const char *call_id;       // ID da chamada
    uint16_t local_port;       // Porta local SIP usada na sessão
    uint16_t remote_port;      // Porta remota SIP usada na sessão
    bool is_secure;            // Se a sessão usa TLS
    bool is_held;              // Chamada está em espera (hold)?
    bool is_call_active;       // Chamada ativa?
} sip_session_t;

// Funções SIP

/**
 * @brief Inicializa o SIP stack (inicialização de bibliotecas, transporte, etc.)
 * 
 * @param config Configurações para SIP (username, domínio, senha, etc.)
 * @return 0 em sucesso, -1 em erro
 */
int sip_init(const sip_config_t *config);

/**
 * @brief Realiza registro SIP no servidor especificado nas configurações
 * 
 * @param config Configurações de registro SIP (domínio, proxy, etc.)
 * @return 0 em sucesso, -1 em erro
 */
int sip_register(const sip_config_t *config);

/**
 * @brief Envia uma requisição SIP INVITE para iniciar uma chamada
 * 
 * @param session Sessão SIP (local e remote URI, etc.)
 * @return 0 em sucesso, -1 em erro
 */
int sip_invite(sip_session_t *session);

/**
 * @brief Envia uma requisição SIP BYE para encerrar a chamada ativa
 * 
 * @param session Sessão SIP a ser encerrada
 * @return 0 em sucesso, -1 em erro
 */
int sip_bye(sip_session_t *session);

/**
 * @brief Coloca uma chamada em espera enviando um SIP re-INVITE com SDP de hold
 * 
 * @param session Sessão SIP que será colocada em espera
 * @return 0 em sucesso, -1 em erro
 */
int sip_hold(sip_session_t *session);

/**
 * @brief Retoma uma chamada em espera enviando um SIP re-INVITE com SDP de unhold
 * 
 * @param session Sessão SIP a ser retomada
 * @return 0 em sucesso, -1 em erro
 */
int sip_resume(sip_session_t *session);

/**
 * @brief Responde a uma mensagem SIP de registro ou INVITE
 * 
 * @param session Sessão SIP associada à mensagem
 * @param status_code Código de status SIP (ex: 200, 180, 486, etc.)
 * @return 0 em sucesso, -1 em erro
 */
int sip_respond(sip_session_t *session, int status_code);

/**
 * @brief Realiza limpeza e finalização da stack SIP, liberando recursos
 */
void sip_cleanup(void);

#endif // SIP_H
