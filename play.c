/**
 * This file contains bindings to gstreamer.
 * Compile with gcc play.c -o play -Wall `pkg-config --cflags --libs gstreamer-0.10`
 * Use (for testing): ./play file:///path/to/file.mp3
 */

#include <gst/gst.h>
#include <stdbool.h>
#include <pthread.h>
#include "play.h"

static GMainLoop *loop;
static GstElement *pipeline;
static GstBus *bus;

/* Seeks the current track to position "time" in ms. */
void seek (long time)
{
	//Implement me!
}

/* Opens an URI (file:///home/kalle/music/song.mp3) and schedules it */
void set_uri (const char *uri)
{
	if (uri) {
		/* Set the next URI that the playbin will play */
		g_object_set(G_OBJECT(pipeline), "uri", uri, NULL);
		
		/* Bus for message handling */
		bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
		gst_bus_add_watch (bus, bus_call, loop);
		gst_object_unref (bus);
			     
		gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
	}
}

void play ()
{
	gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
}

void pause ()
{
	gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
}

void stop ()
{
	gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
}

/* Initializes gstreamer */
void init ()
{
	gst_init (NULL, NULL); /* Initialize gstreamer */

	/* Fetches main event loop, as not running */
	loop = g_main_loop_new (NULL, FALSE); 
	
	/* Creates a playbin pipeline with the name 'player' */
	pipeline = gst_element_factory_make ("playbin2", "player");

}

/* TODO: Make use of this fine bus */
static gboolean bus_call(GstBus *bus, GstMessage *msg, void *user_data)
{
	switch (GST_MESSAGE_TYPE (msg)){
		case GST_MESSAGE_EOS: {
			g_message("End of stream");
			g_main_loop_quit(loop);
			break;
		}
		case GST_MESSAGE_ERROR: {
			GError *err;
			gst_message_parse_error(msg, &err, NULL);
			g_error("%s", err->message);
			g_error_free(err);
			g_main_loop_quit(loop);
		} 
		default:
			break;
	}
	
	return TRUE;
}

/* For testing. Reads an URI from argv, and plays it for 10 seconds. */
int main (int argc, char *argv[])
{
	init();
	
	set_uri (argv[1]);
	play();
	sleep(10);
	return 0;
}


