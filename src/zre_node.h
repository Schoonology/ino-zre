/**
 * The local ZRE node.
 */


#ifndef __ZRE_NODE_H_INCLUDED__
#define __ZRE_NODE_H_INCLUDED__

#include "ZMTP.h"

// Opaque type.
typedef struct _zre_node_t zre_node_t;

// Create a new ZRE node with the specified name and a random UUID.
zre_node_t *zre_node_new (const char *name);

// Destroy a ZRE node.
void zre_node_destroy (zre_node_t **self_p);

// Start a ZRE node. This binds the internal UDP state, but _update
// calls are still required to send beacons.
void zre_node_start (zre_node_t *self);

// Stop a ZRE node.
void zre_node_stop (zre_node_t *self);

// Update the internal state. This should be called periodically
// to read received data and send beacons. If _update is not called
// often enough (usually more frequently that every 5 seconds),
// connected nodes will consider the node slow or evasive.
void zre_node_update (zre_node_t *self);

// Send a set of frames to all connected peers (NOT a normal operation
// for ZRE).
//
// See `zre_peer_whisper` for more information.
void zre_node_broadcast (zre_node_t *self, zmtp_frame_t **frame_list, uint8_t frame_count);

#endif
