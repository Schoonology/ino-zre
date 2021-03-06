#include "ZMTP.h"
#include "zre_peer.h"
#include "util.h"

struct _zre_peer_t {
  zmtp_socket_t *socket;
  uint8_t uuid[16];
  uint16_t port;
  uint8_t sequence;
};

zre_peer_t *zre_peer_new (zre_beacon_t *beacon) {
  zre_peer_t *self = (zre_peer_t *) malloc (sizeof (zre_peer_t));
  assert (self);

  self->port = zre_beacon_port (beacon);
  memcpy (self->uuid, zre_beacon_uuid (beacon), 16);
  self->sequence = 1;

  self->socket = zmtp_socket_new (DEALER);
  // TODO(schoon) - Handle failed connections.
  zmtp_socket_connect (self->socket, zre_beacon_addr (beacon), self->port);

  return self;
}

void zre_peer_destroy (zre_peer_t **self_p) {
  assert (self_p);

  if (*self_p) {
    zre_peer_t *self = *self_p;

    zmtp_socket_destroy (&self->socket);

    free (self);
    *self_p = NULL;
  }
}

uint8_t *zre_peer_uuid (zre_peer_t *self) {
  assert (self);

  return self->uuid;
}

void zre_peer_socket_uuid (zre_peer_t *self, zmtp_uuid_t *uuid) {
  assert (self);
  assert (uuid);

  return zmtp_socket_uuid (self->socket, uuid);
}

void zre_peer_send (zre_peer_t *self, zmtp_frame_t *frame) {
  assert (self);
  assert (frame);

  zmtp_socket_send (self->socket, frame);
  zmtp_frame_dump (frame);
}

bool zre_peer_ready (zre_peer_t *self) {
  assert (self);

  return zmtp_socket_ready (self->socket);
}

void zre_peer_whisper (zre_peer_t *self, zmtp_msg_t **message_p) {
  assert (self);
  assert (message_p);

  // TODO(schoon) - Queue.
  if (!zmtp_socket_ready (self->socket)) {
    return;
  }

  uint8_t whisper[6] = {
    0xAA, 0xA1,
    2, 2, 0, self->sequence
  };

  zmtp_frame_t *wrapper = zmtp_frame_new (whisper, 6, ZMTP_FRAME_MORE);
  zre_peer_send (self, wrapper);
  zmtp_frame_destroy (&wrapper);

  zmtp_msg_send (message_p, self->socket);

  ++self->sequence;
}

void zre_peer_whisper (zre_peer_t *self, zmtp_frame_t **frame_list, uint8_t frame_count) {
  assert (self);
  assert (frame_list);

  // TODO(schoon) - Queue.
  if (!zmtp_socket_ready (self->socket)) {
    return;
  }

  uint8_t whisper[6] = {
    0xAA, 0xA1,
    2, 2, 0, self->sequence
  };

  zmtp_frame_t *wrapper = zmtp_frame_new (whisper, 6, ZMTP_FRAME_MORE);

  zre_peer_send (self, wrapper);

  for (uint8_t i = 0; i < frame_count; ++i) {
    zre_peer_send (self, frame_list[i]);
  }

  zmtp_frame_destroy (&wrapper);

  ++self->sequence;
}

void zre_peer_update (zre_peer_t *self) {
  assert (self);

  zmtp_socket_update (self->socket);

  if (self->sequence == 1 && zmtp_socket_ready (self->socket)) {
    uint8_t hello[47] = {
      0xAA, 0xA1, // signature
      1, 2, 0, self->sequence, // id version sequence
      // 0,          // endpoint
      25, 't', 'c', 'p', ':', '/', '/', '1', '9', '2', '.', '1', '6', '8', '.', '2', '9', '.', '1', '5', '5', ':', '5', '6', '7', '1',
      0, 0, 0, 0, // groups
      0,          // status
      5, 'S', 'p', 'a', 'r', 'k',
      0, 0, 0, 0, // headers
    };

    zmtp_frame_t *frame = zmtp_frame_new (hello, 47);

    zre_peer_send (self, frame);

    zmtp_frame_destroy (&frame);

    ++self->sequence;
  }
}

void zre_peer_dump (zre_peer_t *self) {
  assert (self);

  zre_debug_dump (self->uuid, 16);
  Serial.println (self->port);
  zmtp_socket_dump (self->socket);
}
