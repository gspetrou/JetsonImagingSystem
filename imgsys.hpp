#ifndef IMGSYS_GUARD_H
#define IMGSYS_GUARD_H

/************
	Includes
*************/
#include <vector>
#include <string>
#include <array>
#include <mutex>

/********************
	ImgSys Namespace
*********************/
namespace ImgSys {
	struct ParsedURL {
		std::string address;
		std::string endpoint;
		int port;
	};

	ImgSys::ParsedURL parseURL(std::string);
	
	// Convert full image path to just image name ("/media/nvidia/sd/img_5.jpg" -> "img_5")
	std::string fullFilePathToFileName(std::string);

	// typedef ImageCaptureCallback
	//
	// Desc:		A typedef for: void myFunc(std::string)
	//				Callback ran when an image is taken.
	// Callback Arg One:		std::string, full file path of image just created.
	typedef void (*ImageCaptureCallback)(std::string, void*, void*);

	struct Angle {
		double pitch;
		double yaw;
		double roll;
	};

	struct Telemetry {
		double latitude;
		double longitude;
		double altitudeMSL;
		Angle angle;
	};

	struct FileSendData {
		ImgSys::Telemetry telemetry;
		std::string imagePath;
	};

	class LiveTelemetry {
	public:
		virtual ImgSys::Telemetry getCurrentTelemetry() = 0;
	};

	// class Cameras
	//
	// Desc:		Class which abstracts the backend framework being used for image capture.
	class Cameras {
	public:
		// struct CameraDetail
		//
		// Desc:		Struct containning fields that represent a camera.
		// Field 1:		int, width of camera capture.
		// Field 2:		int, height of camera capture.
		// Field 3:		int, fps of camera capture (in range [1:30]).
		// Field 4:		int, sensor-id to identify this camrea.
		// Field 5:		std::string, where to save images taken from the camera.
		struct CameraDetail {
			int sensorID;
			int width;
			int height;
			int fps;
			std::string filepath;
		};

		// Cameras()
		//
		// Desc:		Default constructor.
		Cameras();

		// I really dont need to document these:
		int getJpegQuality();
		bool hasBeganCapture();
		bool hasBeenBuilt();
		void setJpegQuality(int);
		bool getTestMode();
		void setTestMode(bool);
		std::vector<ImgSys::Cameras::CameraDetail> getCameraList();
		void setCaptureCallback(ImgSys::ImageCaptureCallback, void*, void*);

		// void addCamera(int, int, int, int, std::string)
		//
		// Desc:		Registers a new camera with the pipeline.
		// Arg One:		int, sensor-id representing that camera.
		// Arg Two:		int, width of camera capture.
		// Arg Three:	int, height of camera capture.
		// Arg Four:	int, fps of camera capture.
		// Arg Five:	std::string, where images from that camera are saved.
		void addCamera(int, int, int, int, std::string);

		// virtual void build()
		//
		// Desc:		Does all necessary config with the backend framework to build the camera pipeline.
		//				Once this is called you should change settings (width, fps, etc.).
		// Note:		When overriding make sure to set this.wasBuilt
		virtual void build() = 0;

		// virtual void beginCapture()
		//
		// Desc:		Start capturing images.
		// Note:		When overriding make sure to set this.beganCapture
		virtual void beginCapture() = 0;

	protected:
		int jpegQuality;	// Quality of jpeg capture (in range of [1:100])
		bool beganCapture;	// Has image capture began.
		bool wasBuilt;		// Has our pipeline been built, cant make changes after.
		bool testMode;		// Should we operate independently of the Jetson using a test image source.
							// GStreamer:				nvcamerasrc -> nvjpegenc -> identity -> multifilesink
							// GStreamer (testmode):	videotestsrc -> jpegenc -> identity -> multifilesink
		ImgSys::ImageCaptureCallback captureCallback;
		std::vector<ImgSys::Cameras::CameraDetail> cameras;	// Vector of camera devices to capture from.
		
		void *capCallbackData_Queue;
		void *capCallbackData_Telem;
	};
}
#endif // IMGSYS_GUARD_H