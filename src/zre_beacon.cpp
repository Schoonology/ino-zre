#include "zre_beacon.h"
#include "util.h"

#ifndef BROADCAST_ADDRESS
static IPAddress BROADCAST_ADDRESS (255, 255, 255, 255);
#endif

#ifndef ZRE_DISCOVERY_PORT
#define ZRE_DISCOVERY_PORT 5670
#endif

struct _zre_beacon_t {
  uint8_t signature[4];
  uint8_t uuid[16];
  uint8_t port[2];
};

zre_beacon_t *zre_beacon_new () {
  zre_beacon_t *self = (zre_beacon_t *) malloc (sizeof (zre_beacon_t));
  assert (self);

  return self;
}

zre_beacon_t *zre_beacon_new (const uint8_t *uuid, uint16_t port) {
  zre_beacon_t *self = zre_beacon_new ();

  self->signature[0] = 'Z';
  self->signature[1] = 'R';
  self->signature[2] = 'E';
  self->signature[3] = 0x01;

  memcpy (self->uuid, uuid, 16);

  self->port[0] = port >> 8 & 0xFF;
  self->port[1] = port << 8 & 0xFF;

  return self;
}

void zre_beacon_destroy (zre_beacon_t **self_p) {
  assert (self_p);

  if (*self_p) {
    zre_beacon_t *self = *self_p;

    free (self);

    *self_p = NULL;
  }
}

bool zre_beacon_valid (zre_beacon_t *self) {
  assert (self);

  return self->signature[0] == 'Z' \
    || self->signature[1] == 'R' \
    || self->signature[2] == 'E' \
    || self->signature[3] == 0x01;
}

uint8_t *zre_beacon_uuid (zre_beacon_t *self) {
  assert (self);

  return self->uuid;
}

uint16_t zre_beacon_port (zre_beacon_t *self) {
  assert (self);

  return (self->port[0] << 8) | self->port[1];
}

void zre_beacon_send (zre_beacon_t *self, UDP *socket) {
  assert (self);

  socket->beginPacket (BROADCAST_ADDRESS, ZRE_DISCOVERY_PORT);
  socket->write ((uint8_t *) self, sizeof (zre_beacon_t));
  socket->endPacket ();
}

void zre_beacon_recv (zre_beacon_t *self, UDP *socket) {
  assert (self);

  socket->read ((uint8_t *) self, sizeof (zre_beacon_t));
}

void zre_beacon_dump (zre_beacon_t *self) {
  assert (self);

  debug_dump ((uint8_t *) self, sizeof (zre_beacon_t));
}
