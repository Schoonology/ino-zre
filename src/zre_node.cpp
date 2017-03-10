#include "arch.h"
#include "zre_node.h"
#include "zre_beacon.h"
#include "zre_peer.h"
#include "util.h"

#define ZRE_DISCOVERY_PORT 5670
#define ZRE_SERVICE_PORT 5671

struct _zre_node_t {
  uint8_t *uuid;
  zre_beacon_t *beacon;
  zre_peer_t **peer_list;
  uint8_t peer_count;
  char *name;
  UDP *socket;
};

// Constructor
zre_node_t *zre_node_new (const char *name) {
  zre_node_t *self = (zre_node_t *) malloc (sizeof (zre_node_t));
  assert (self);

  self->uuid = uuid_new ();
  self->beacon = zre_beacon_new (self->uuid, ZRE_SERVICE_PORT);
  self->socket = new UDP();
  self->peer_count = 0;

  Serial.println("Beacon:");
  zre_beacon_dump (self->beacon);

  if (name == NULL) {
    self->name = (char *) malloc (7);
    memcpy (self->name, self->uuid, 6);
  } else {
    size_t name_len = strlen (name);
    self->name = (char *) malloc (name_len);
    memcpy (self->name, name, name_len);
  }

  return self;
}

void zre_node_destroy (zre_node_t **self_p) {
  assert (self_p);

  if (*self_p) {
    zre_node_t *self = *self_p;

    free (self->uuid);
    zre_beacon_destroy (&self->beacon);
    free (self->name);
    delete self->socket;

    if (self->peer_list) {
      for (uint8_t i = 0; i < self->peer_count; ++i) {
        zre_peer_t *peer = self->peer_list[i];
        zre_peer_destroy (&peer);
      }

      free (self->peer_list);
    }

    free (self);
    *self_p = NULL;
  }
}

void zre_node_start (zre_node_t *self) {
  assert (self);

  self->socket->begin (ZRE_DISCOVERY_PORT);
}

void zre_node_stop (zre_node_t *self) {
  assert (self);

  zre_beacon_destroy (&self->beacon);
  self->beacon = zre_beacon_new (self->uuid, 0);

  self->socket->beginPacket (IPAddress (255, 255, 255, 255), ZRE_DISCOVERY_PORT);
  zre_beacon_send (self->beacon, self->socket);
  self->socket->endPacket ();

  self->socket->stop ();
}

zre_peer_t *zre_node_require_peer (zre_node_t *self, zre_beacon_t *beacon) {
  assert (self);
  assert (beacon);

  for (uint8_t i = 0; i < self->peer_count; ++i) {
    zre_peer_t *peer = self->peer_list[i];

    if (uuid_equal (zre_peer_uuid (peer), zre_beacon_uuid (beacon))) {
      // TODO(schoon) - Handle port going to zero (disconnects).
      return peer;
    }
  }

  uint8_t peer_count = self->peer_count + 1;
  zre_peer_t **peer_list = (zre_peer_t **) malloc (sizeof (zre_peer_t *) * peer_count);
  memcpy (peer_list, self->peer_list, self->peer_count);
  if (self->peer_list) {
    free (self->peer_list);
  }
  self->peer_list = peer_list;
  self->peer_count = peer_count;

  zre_peer_t **peer_p = self->peer_list + (self->peer_count - 1);
  *peer_p = zre_peer_new (beacon);

  zre_peer_socket_uuid (*peer_p, self->uuid);

  Serial.println ("New peer:");
  Serial.println ("---");
  zre_peer_dump (*peer_p);
  Serial.println ("---");

  return *peer_p;
}

void zre_node_update (zre_node_t *self) {
  assert (self);

  self->socket->beginPacket (IPAddress (255, 255, 255, 255), ZRE_DISCOVERY_PORT);
  zre_beacon_send (self->beacon, self->socket);
  self->socket->endPacket ();

  if (self->socket->parsePacket () > 0) {
    zre_beacon_t *beacon = zre_beacon_new ();
    zre_beacon_recv (beacon, self->socket);

    if (zre_beacon_valid (beacon)) {
      zre_node_require_peer (self, beacon);
    }

    zre_beacon_destroy (&beacon);
  }

  for (uint8_t i = 0; i < self->peer_count; ++i) {
    zre_peer_t *peer = self->peer_list[i];
    zre_peer_update (peer);
  }
}

void zre_node_broadcast (zre_node_t *self, zmtp_frame_t **frame_list, uint8_t frame_count) {
  assert (self);
  assert (frame_list);

  for (uint8_t i = 0; i < self->peer_count; ++i) {
    zre_peer_t *peer = self->peer_list[i];
    zre_peer_whisper (peer, frame_list, frame_count);
  }
}
