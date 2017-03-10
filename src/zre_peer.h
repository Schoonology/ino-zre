/**
 * A non-local ZRE peer.
 *
 * Currently, only HELLO and WHISPER commands are supported.
 */

#ifndef __ZRE_PEER_H_INCLUDED__
#define __ZRE_PEER_H_INCLUDED__

#include "ZMTP.h"
#include "zre_beacon.h"

// Opaque type.
typedef struct _zre_peer_t zre_peer_t;

// Create a new ZRE peer from the data in the supplied beacon.
// Any required internal state will be copied from the beacon.
zre_peer_t *zre_peer_new (zre_beacon_t *beacon);

// Destroy a ZRE peer.
void zre_peer_destroy (zre_peer_t **self_p);

// Return the UUID associated with a peer.
uint8_t *zre_peer_uuid (zre_peer_t *self);

// Set the UUID sent as the Identity from the local DEALER socket.
//
// See `zmtp_socket_uuid` for more information.
void zre_peer_socket_uuid (zre_peer_t *self, uint8_t *uuid);

bool zre_peer_ready (zre_peer_t *self);

// Send a set of ZMTP frames to a connected peer. If the underlying
// DEALER socket is not ready, these frames will be dropped.
void zre_peer_whisper (zre_peer_t *self, zmtp_frame_t **frame_list, uint8_t frame_count);

// Update the internal state. This should be called periodically
// to read received data and send required commands (e.g. HELLO).
void zre_peer_update (zre_peer_t *self);

// Dump internal state.
void zre_peer_dump (zre_peer_t *self);

#endif
