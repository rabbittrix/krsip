#include "rtp.h"
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

void rtp_init_packet(rtp_packet_t *packet, uint8_t payload_type, uint32_t ssrc) {
    memset(packet, 0, sizeof(rtp_packet_t)); 
    packet->header.version = RTP_VERSION;   
    packet->header.payload_type = payload_type; 
    packet->header.ssrc = ssrc;             
}

void rtp_set_sequence_number(rtp_packet_t *packet, uint16_t sequence_number) {
    packet->header.sequence_number = sequence_number;
}

void rtp_set_timestamp(rtp_packet_t *packet, uint32_t timestamp) {
    packet->header.timestamp = timestamp;
}

void rtp_set_payload(rtp_packet_t *packet, const uint8_t *payload, size_t size) {
    packet->payload = (uint8_t*) malloc(size); 
    if (packet->payload) {
        memcpy(packet->payload, payload, size); 
        packet->payload_size = size;            
    }
}

size_t rtp_serialize(const rtp_packet_t *packet, uint8_t *buffer, size_t buffer_size) {
    
    if (buffer_size < RTP_HEADER_SIZE + packet->payload_size) {
        return 0; 
    }

    buffer[0] = (packet->header.version << 6) | (packet->header.padding << 5) |
                (packet->header.extension << 4) | packet->header.csrc_count;
    buffer[1] = (packet->header.marker << 7) | packet->header.payload_type;
    *((uint16_t*)&buffer[2]) = htons(packet->header.sequence_number);
    *((uint32_t*)&buffer[4]) = htonl(packet->header.timestamp);
    *((uint32_t*)&buffer[8]) = htonl(packet->header.ssrc);

    for (unsigned int i = 0; i < packet->header.csrc_count; i++) {
        *((uint32_t*)&buffer[RTP_HEADER_SIZE + i * 4]) = htonl(packet->header.csrc[i]);
    }

    memcpy(&buffer[RTP_HEADER_SIZE + packet->header.csrc_count * 4], packet->payload, packet->payload_size);

    return RTP_HEADER_SIZE + packet->header.csrc_count * 4 + packet->payload_size;
}

bool rtp_deserialize(const uint8_t *buffer, size_t buffer_size, rtp_packet_t *packet) {
    if (buffer_size < RTP_HEADER_SIZE) {
        return false; 
    }

    packet->header.version = (buffer[0] >> 6) & 0x03;
    packet->header.padding = (buffer[0] >> 5) & 0x01;
    packet->header.extension = (buffer[0] >> 4) & 0x01;
    packet->header.csrc_count = buffer[0] & 0x0F;
    packet->header.marker = (buffer[1] >> 7) & 0x01;
    packet->header.payload_type = buffer[1] & 0x7F;
    packet->header.sequence_number = ntohs(*((uint16_t*)&buffer[2]));
    packet->header.timestamp = ntohl(*((uint32_t*)&buffer[4]));
    packet->header.ssrc = ntohl(*((uint32_t*)&buffer[8]));

    if (buffer_size < RTP_HEADER_SIZE + packet->header.csrc_count * 4) {
        return false;
    }

    for (unsigned int i = 0; i < packet->header.csrc_count; i++) {
        packet->header.csrc[i] = ntohl(*((uint32_t*)&buffer[RTP_HEADER_SIZE + i * 4]));
    }

    size_t payload_offset = RTP_HEADER_SIZE + packet->header.csrc_count * 4;
    size_t payload_size = buffer_size - payload_offset;

    packet->payload = (uint8_t*) malloc(payload_size);
    if (!packet->payload) {
        return false;
    }
    memcpy(packet->payload, &buffer[payload_offset], payload_size);
    packet->payload_size = payload_size;

    return true;
}

void rtp_free_payload(rtp_packet_t *packet) {
    if (packet->payload) {
        free(packet->payload);
        packet->payload = NULL;
        packet->payload_size = 0;
    }
}
