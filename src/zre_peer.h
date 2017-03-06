#ifndef __ZRE_PEER_H_INCLUDED__
#define __ZRE_PEER_H_INCLUDED__

#include "zre_beacon.h"

typedef struct _zre_peer_t zre_peer_t;

zre_peer_t *zre_peer_new (zre_beacon_t *beacon);

void zre_peer_destroy (zre_peer_t **self_p);

uint8_t *zre_peer_uuid (zre_peer_t *self);

void zre_peer_update (zre_peer_t *self);

void zre_peer_dump (zre_peer_t *self);

#endif
