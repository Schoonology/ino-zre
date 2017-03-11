/**
 * A ZRE beacon, suitable for broadcasting over UDP.
 */

#ifndef __ZRE_BEACON_H_INCLUDED__
#define __ZRE_BEACON_H_INCLUDED__

#include "arch.h"
#include "zmtp_uuid.h"

// Opaque type.
typedef struct _zre_beacon_t zre_beacon_t;

// Create a new, invalid ZRE beacon with a zeroed-out UUID and port.
zre_beacon_t *zre_beacon_new ();

// Create a new, valid ZRE beacon with the specified UUID and port. The
// UUID should be 16 bytes, which will be copied into the beacon's
// internal state.
zre_beacon_t *zre_beacon_new (zmtp_uuid_t *uuid, uint16_t port);

// Destroy a ZRE beacon.
void zre_beacon_destroy (zre_beacon_t **self_p);

// Return true if the signature of the beacon is correct, false otherwise.
bool zre_beacon_valid (zre_beacon_t *self);

// Return the 16-byte UUID in the beacon.
uint8_t *zre_beacon_uuid (zre_beacon_t *self);

// Return the 2-byte port in the beacon.
uint16_t zre_beacon_port (zre_beacon_t *self);

// Return the recvfrom address of a received beacon, 0.0.0.0 otherwise.
uint8_t *zre_beacon_addr (zre_beacon_t *self);

// Send a beacon over the provided UDP socket. It is the caller's
// responsibility to call UDP.beginPacket and UDP.endPacket.
void zre_beacon_send (zre_beacon_t *self, UDP *socket);

// Receive a beacon from the provided UDP socket. It is the caller's
// responsibility to call UDP.parsePacket.
void zre_beacon_recv (zre_beacon_t *self, UDP *socket);

// Dump internal state.
void zre_beacon_dump (zre_beacon_t *self);

#endif
