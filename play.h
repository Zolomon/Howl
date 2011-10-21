#ifndef play_h
#define play_h

#include "gst/gst.h"

/* Commands for changing the stream state 
 * Might need these later for polling state. */
typedef enum {
	PLAY,
	PAUSE,
	STOP
} state_t;

static gboolean bus_call(GstBus *bus, GstMessage *msg, void *user_data);
void set_state(state_t s);
void seek(long time);
void set_uri(const char *uri);
void play(void);
void pause(void);
void stop(void);


#endif
