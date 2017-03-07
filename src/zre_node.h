#ifndef __ZRE_NODE_H_INCLUDED__
#define __ZRE_NODE_H_INCLUDED__

#include "ZMTP.h"

typedef struct _zre_node_t zre_node_t;

// Constructor, creates a new ZRE node. Note that until you start the
// node it is silent and invisible to other nodes on the network.
// The node name is provided to other nodes during discovery. If you
// specify NULL, Zyre generates a randomized node name from the UUID.
zre_node_t *zre_node_new (const char *name);

// Destructor, destroys a Zyre node. When you destroy a node, any
// messages it is sending or receiving will be discarded.
void zre_node_destroy (zre_node_t **self_p);

// Start node, after setting header values. When you start a node it
// begins discovery and connection. Returns 0 if OK, -1 if it wasn't
// possible to start the node.
void zre_node_start (zre_node_t *self);

// Stop node; this signals to other peers that this node will go away.
// This is polite; however you can also just destroy the node without
// stopping it.
void zre_node_stop (zre_node_t *self);

// Update the node's internal state. This should be called from the
// main project's `loop` function.
void zre_node_update (zre_node_t *self);

// Receive next message from network; the message may be a control
// message (ENTER, EXIT, JOIN, LEAVE) or data (WHISPER, SHOUT).
// Returns zmtp_msg_t object, or NULL if interrupted
// Caller owns return value and must destroy it when done.
zmtp_msg_t *zre_node_recv (zre_node_t *self);

// Send message to single peer, specified as a UUID string
// Destroys message after sending
int zyre_node_whisper (zre_node_t *self, const char *peer, zmtp_msg_t **msg_p);

#endif
