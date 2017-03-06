#ifndef __ZRE_BEACON_H_INCLUDED__
#define __ZRE_BEACON_H_INCLUDED__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#elif defined(SPARK)
  #include "application.h"
#else
  #error Only ARDUINO or SPARK supported.
#endif

typedef struct _zre_beacon_t zre_beacon_t;

zre_beacon_t *zre_beacon_new ();

zre_beacon_t *zre_beacon_new (const uint8_t *uuid, uint16_t port);

void zre_beacon_destroy (zre_beacon_t **self_p);

bool zre_beacon_valid (zre_beacon_t *self);

uint8_t *zre_beacon_uuid (zre_beacon_t *self);

uint16_t zre_beacon_port (zre_beacon_t *self);

uint8_t *zre_beacon_addr (zre_beacon_t *self);

void zre_beacon_send (zre_beacon_t *self, UDP *socket);

void zre_beacon_recv (zre_beacon_t *self, UDP *socket);

void zre_beacon_dump (zre_beacon_t *self);

#endif
