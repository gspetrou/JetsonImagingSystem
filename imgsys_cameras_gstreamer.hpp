#ifndef IMGSYS_CAMERAS_GSTREAMER_GUARD_H
#define IMGSYS_CAMERAS_GSTREAMER_GUARD_H

#include <gst/gst.h>
#include "imgsys.hpp"

namespace ImgSys {
	// class GStreamerCameras: public ImgSys::Cameras
	//
	// Desc:		Implements the ImgSys::Cameras class with a GStreamer backend.
	class GStreamerCameras: public ImgSys::Cameras {
	private:
		GstElement *pipeline;	// The pipeline object all GStreamer elements are attached to.
		GMainLoop *loop;		// The pipeline loop.
		guint bus_watch_id;		// Used for receiving messages from the pipeline bus.

		// Called when a message is received.
		static gboolean msgReceivedCallback(__attribute__((unused)) GstBus*, GstMessage*, gpointer);

		// Called when the pipeline stalls and should end.
		static gboolean pipelineTimeOut(__attribute__((unused)) gpointer);
	public:
		void build();			// Sets up the media framework
		void beginCapture();	// Begins image capture.
	};
}

gboolean ImgSys::GStreamerCameras::msgReceivedCallback(__attribute__((unused)) GstBus *bus, GstMessage *message, __attribute__((unused)) gpointer data) {
	ImgSys::GStreamerCameras *cam = (ImgSys::GStreamerCameras*) data;

	switch (GST_MESSAGE_TYPE(message)) {
		// Message from multifilesink
		case GST_MESSAGE_ELEMENT: {
			gchar *gfilename;
			const GstStructure *msgStructure = gst_message_get_structure(message);
			gst_structure_get(msgStructure, "filename", G_TYPE_STRING, &gfilename, NULL);
			cam->captureCallback((std::string) gfilename, cam->capCallbackData_Queue, cam->capCallbackData_Telem);
			break;
		}
		// Some time of error :(
		case GST_MESSAGE_ERROR: {
			GError *err;
			gchar *debug;

			gst_message_parse_error(message, &err, &debug);
			g_print("Error: %s\n", err->message);
			g_error_free(err);
			g_free(debug);

			g_main_loop_quit(cam->loop);
			break;
		}
		// End of stream
		case GST_MESSAGE_EOS:
			printf("EOS\n");
			g_main_loop_quit(cam->loop);
			break;
		default:
			//cout << "Unhandled multifilesink message!" << GST_MESSAGE_TYPE(message) << endl;
			break;
	}

	return TRUE;	// If we return FALSE here no more callbacks will run.
}

gboolean ImgSys::GStreamerCameras::pipelineTimeOut(__attribute__((unused)) gpointer ptr) {
	return TRUE;
}

void ImgSys::GStreamerCameras::build() {
	if (this->hasBeenBuilt()) {
		// TODO: Real error handling
		std::cout << "ERROR: Tried to build an already built pipeline.\n";
		return;
	}

	// Init GStreamer if it hasn't already been.
	if (!gst_is_initialized())
		gst_init(NULL, NULL);

	// Create the pipeline element.
	this->pipeline = gst_pipeline_new("img_sys_pipeline");
	if (!this->pipeline) {
		g_printerr("Could not instantiate pipeline element.\n");
		gst_element_set_state(pipeline, GST_STATE_NULL);
		gst_object_unref(pipeline);
		return;
	}

	// Generate a pipeline for each camera.
	// Theyre all containned in this->pipeline but independent of each other.
	for (ImgSys::Cameras::CameraDetail camDetail : this->cameras) {
		const std::string fpsStr = std::to_string(camDetail.fps);
		const std::string fpsRange = fpsStr + " " + fpsStr;

		GstElement *camerasrc;
		GstElement *jpegenc;
		GstCaps *camerasrcCaps; 
		if (this->getTestMode()) {
			camerasrc = gst_element_factory_make("videotestsrc", NULL);	// Make camera src element (videotestsrc in test mode).
			g_object_set(camerasrc, "pattern", 17, NULL);	
			jpegenc = gst_element_factory_make("jpegenc", NULL);		// Make jpeg encode element (jpegenc in test mode).
			std::string caps = "video/x-raw,"
				"format=(string)I420,"
				"width=(int)" + std::to_string(camDetail.width) + ","
				"height=(int)" + std::to_string(camDetail.height) + ","
				"framerate=(fraction)" + fpsStr + "/1";

			camerasrcCaps = gst_caps_from_string(caps.c_str());
		} else {
			// Make camera src element, nvcamerasrc in non-testmode.
			camerasrc = gst_element_factory_make("nvcamerasrc", NULL);
			// Also set sensorid and fps.
			g_object_set(camerasrc, "sensor-id", camDetail.sensorID, "fpsRange", fpsRange.c_str(), NULL);
			// Make jpeg encode element, nvjpeg in non-testmode since its faster.
			jpegenc = gst_element_factory_make("nvjpegenc", NULL);
			std::string caps = "video/x-raw(memory:NVMM),"
				"format=(string)I420,"
				"width=(int)" + std::to_string(camDetail.width) + ","
				"height=(int)" + std::to_string(camDetail.height) + ","
				"framerate=(fraction)" + fpsStr + "/1";
			camerasrcCaps = gst_caps_from_string(caps.c_str());
		}
		 
		// Identity element, used to sync pipeline to FPS speed.
		GstElement *identity = gst_element_factory_make("identity", NULL);
		g_object_set(identity, "sync", TRUE, NULL);

		// Set jpeg encode quality.
		g_object_set(jpegenc, "quality", this->getJpegQuality(), NULL);

		// Set file save location and post-messages over bus.
		GstElement *multifilesink = gst_element_factory_make("multifilesink", NULL);
		g_object_set(multifilesink, "location", camDetail.filepath.c_str(), "post-messages", TRUE, NULL);

		// Error message if we couldn't create the elements.
		if (!camerasrc || !identity || !jpegenc || !multifilesink) {
			g_printerr("Could not instantiate all pipeline elements.\n");
		}

		// Add elements to pipeline
		gst_bin_add_many(GST_BIN(this->pipeline), camerasrc, identity, jpegenc, multifilesink, NULL);

		// Link elements together
		if (!gst_element_link_filtered(camerasrc, identity, camerasrcCaps) || !gst_element_link(identity, jpegenc) || !gst_element_link(jpegenc, multifilesink)) {
			g_printerr("Elements could not be linked.\n");
			gst_caps_unref(camerasrcCaps);
			gst_object_unref(this->pipeline);
			return;
		}
		gst_caps_unref(camerasrcCaps);
	}

	// Create callback for when a post-messages mesage is sent.
	this->loop = g_main_loop_new(NULL, FALSE);
	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(this->pipeline));
	this->bus_watch_id = gst_bus_add_watch(bus, ImgSys::GStreamerCameras::msgReceivedCallback, this);
	gst_object_unref(bus);
	this->wasBuilt = true;
}

void ImgSys::GStreamerCameras::beginCapture() {
	if (!this->hasBeenBuilt()) {
		// TODO: Real error handling
		std::cout << "ERROR: Tried to begin a pipeline before it was built.\n";
		return;
	}

	if (this->hasBeganCapture()) {
		// TODO: Real error handling
		std::cout << "ERROR: Tried to begin an already started pipeline.\n";
		return;
	}

	g_timeout_add_seconds(35, ImgSys::GStreamerCameras::pipelineTimeOut, this->pipeline);
	if (gst_element_set_state(this->pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
		g_printerr("Unable to set the pipeline to the playing state.\n");
		gst_object_unref(this->pipeline);
		return;
	}

	this->beganCapture = true;

	g_main_loop_run(this->loop);

	// Cleanup
	gst_element_set_state(this->pipeline, GST_STATE_NULL);
	gst_object_unref(this->pipeline);
	g_source_remove(this->bus_watch_id);
	g_main_loop_unref(this->loop);
}

#endif // IMGSYS_CAMERAS_GSTREAMER_GUARD_H