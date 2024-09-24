#ifndef KRSIP_H
#define KRSIP_H

#ifdef __cplusplus
extern "C" {
#endif

// Includes as dependencies of system, how WebRTC and OpenSSL
#include <stdint.h>
#include <stddef.h>

// Constants
#define KRSIP_VERSION "1.0.0"
#define KRSIP_MAX_BUFFER_SIZE 1024
#define KRSIP_MAX_KEY_SIZE 2048
#define KRSIP_MAX_IV_SIZE 16
#define KRSIP_MAX_SALT_SIZE 16
#define KRSIP_MAX_ITERATIONS 10000
#define KRSIP_MAX_KEY_LENGTH 32
#define KRSIP_MAX_IV_LENGTH 16
#define KRSIP_MAX_SALT_LENGTH 16
#define KRSIP_MAX_ITERATIONS_LENGTH 5
#define KRSIP_MAX_KEY_ITERATIONS 10000
#define KRSIP_MAX_IV_ITERATIONS 10000
#define KRSIP_MAX_SALT_ITERATIONS 10000
#define KRSIP_MAX_ITERATIONS_ITERATIONS 5
#define KRSIP_MAX_KEY_ITERATIONS_LENGTH 5
#define KRSIP_MAX_IV_ITERATIONS_LENGTH 5
#define KRSIP_MAX_SALT_ITERATIONS_LENGTH 5
#define KRSIP_MAX_ITERATIONS_ITERATIONS_LENGTH 5
#define KRSIP_MAX_KEY_ITERATIONS_ITERATIONS 10000
#define KRSIP_MAX_IV_ITERATIONS_ITERATIONS 10000

// Types
typedef enum {
    KRSIP_OK = 0,
    KRSIP_ERROR = -1,
    KRSIP_AUTH_FAILED = -2,
    KRSIP_NETWORK_ERROR = -3,
    KRSIP_MEDIA_ERROR = -4
} krsip_status_t;

typedef enum {
    KRSIP_TRANSPORT_UDP,
    KRSIP_TRANSPORT_TCP,
    KRSIP_TRANSPORT_TLS,
    KRSIP_TRANSPORT_DTLS,
    KRSIP_TRANSPORT_WS,
    KRSIP_TRANSPORT_WSS,
} krsip_transport_t;

typedef enum {
    KRSIP_MEDIA_AUDIO,
    KRSIP_MEDIA_VIDEO,
    KRSIP_MEDIA_DATA,
    KRSIP_MEDIA_MESSAGE
} krsip_media_t;

// Structures of configuration SIP
typedef struct {
    const char *username;
    const char *password;
    const char *domain;
    krsip_transport_t transport;
    const char *stun_server;
    const char *turn_server;
    const char *oauth_token;
} krsip_config_t;

// Initialization of the library KRSIP
int krsip_init(void);
void krsip_cleanup(void);

// Functions SIP
int krsip_register(const krsip_config_t *config);
int krsip_invite(const char *target_uri);
int krsip_byte(void);
int krsip_hold(void);
int krsip_resume(void);

// Functions of media (WebRTC)
int krsip_start_audio_stream(const char *remote_ip, uint16_t remote_port);
int krsip_stop_audio_stream(void);
int krsip_start_video_stream(const char *remote_ip, uint16_t remote_port);
int krsip_stop_video_stream(void);
int krsip_start_data_stream(const char *remote_ip, uint16_t remote_port);
int krsip_stop_data_stream(void);

// Functions of NAT Traversal (STUN/TURN/ICE)
int krsip_ice_init(void);
int krsip_stun_turn_init(const char *stun_server, const char *turn_server);

// Functions of security (OAuth 2.0)
int krsip_authenticate_oauth(const char *oauth_token);

// Functions of encryption (SRTP/SRTCP)
int krsip_encrypt_srtp(const char *key, const char *iv, const char *salt, const char *iterations);

// Function of test and debug
int krsip_set_debug_level(int level);
int krsip_print_status(void);

#ifdef __cplusplus
}
#endif
#endif // KRSIP_H