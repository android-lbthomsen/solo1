#ifndef _CTAPHID_H_H
#define _CTAPHID_H_H

#include "usbhid.h"

#define TYPE_MASK               0x80    // Frame type mask
#define TYPE_INIT               0x80    // Initial frame identifier
#define TYPE_CONT               0x00    // Continuation frame identifier

#define CTAPHID_PING         (TYPE_INIT | 0x01) // Echo data through local processor only
#define CTAPHID_MSG          (TYPE_INIT | 0x03) // Send U2F message frame
#define CTAPHID_LOCK         (TYPE_INIT | 0x04) // Send lock channel command
#define CTAPHID_INIT         (TYPE_INIT | 0x06) // Channel initialization
#define CTAPHID_WINK         (TYPE_INIT | 0x08) // Send device identification wink
#define CTAPHID_ERROR        (TYPE_INIT | 0x3f) // Error response

#define CTAPHID_INIT_PAYLOAD_SIZE  (HID_MESSAGE_SIZE-7)
#define CTAPHID_CONT_PAYLOAD_SIZE  (HID_MESSAGE_SIZE-5)

#define CTAPHID_BROADCAST_CID       0xffffffff

#define CTAPHID_BUFFER_SIZE         4096

#define CAPABILITY_WINK             0x01
#define CAPABILITY_LOCK             0x02
#define CAPABILITY_CBOR             0x04
#define CAPABILITY_NMSG             0x08

typedef struct
{
    uint32_t cid;
    union{
        struct{
            uint8_t cmd;
            uint8_t bcnth;
            uint8_t bcntl;
            uint8_t payload[CTAPHID_INIT_PAYLOAD_SIZE];
        } init;
        struct{
            uint8_t seq;
            uint8_t payload[CTAPHID_CONT_PAYLOAD_SIZE];
        } cont;
    } pkt;
} CTAPHID_PACKET;


typedef struct
{
    uint32_t broadcast;
    uint8_t cmd;
    uint8_t bcnth;
    uint8_t bcntl;
    uint8_t nonce[8];
    uint32_t cid;
    uint8_t protocol_version;
    uint8_t version_major;
    uint8_t version_minor;
    uint8_t build_version;
    uint8_t capabilities;
} __attribute__((packed)) CTAPHID_INIT_RESPONSE;


// API specific, not protocol specific //
typedef enum
{
    NO_RESPONSE = 0,
    CTAPHID_RESPONSE,
    U2F_RESPONSE,
    CBOR_RESPONSE,
} CTAPHID_CODE;

typedef struct
{
    int status;
    uint8_t * data;
    int length;
} CTAPHID_STATUS;
////////


void ctaphid_init();

void ctaphid_handle_packet(uint8_t * pkt_raw, CTAPHID_STATUS * stat);

void ctaphid_dump_status(CTAPHID_STATUS * stat);

#define ctaphid_packet_len(pkt)     ((uint16_t)((pkt)->pkt.init.bcnth << 8) | ((pkt)->pkt.init.bcntl))

#endif
