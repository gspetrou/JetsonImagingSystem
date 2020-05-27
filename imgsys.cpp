#include <iostream>
#include <string>
#include <cstdlib>
#include "thirdparty/httplib.h"
#include "thirdparty/readerwriterqueue.h"
#include "thirdparty/atomicops.h"
#include "thirdparty/cxxopts.hpp"
#include "imgsys.hpp"
#include "imgsys_cameras_gstreamer.hpp"
#include "imgsys_telemetry.hpp"

const int CAP_WIDTH = 4104;		// Width of image capture
const int CAP_HEIGHT = 3046;	// Height of image capture
const int CAP_FPS = 1;			// FPS, must be less in the range [1:30]
const int JPEG_QUALITY = 100;	// Jpeg quality percentage in range [1:100]
const std::string CAM_SAVEPATH = "/Users/georgepetrou/Desktop/imaging/img/";
const std::string ADDRESS = "localhost:8000/";

// Garbage design until I found a way to make the telemetry type into a template.
#define TELEM_SOURCE ImgSys::RandomTelemetry

void imageSend(moodycamel::BlockingReaderWriterQueue<ImgSys::FileSendData> &readyImageQueue, cxxopts::ParseResult &argsResult) {
	ImgSys::ParsedURL parsedURL = ImgSys::parseURL(argsResult["destination"].as<std::string>());

	httplib::Client httpClient(parsedURL.address, parsedURL.port);
	ImgSys::FileSendData imageData;

	while (true) {
		// wait_dequeue will block until theres something to dequeue.
		readyImageQueue.wait_dequeue(imageData);
		
		// Read jpeg file data into jpegData
		std::string jpegData;
		httplib::detail::read_file(imageData.imagePath, jpegData);

		std::string fileName = ImgSys::fullFilePathToFileName(imageData.imagePath);

		// Build multipart POST request
		httplib::MultipartFormDataItems items = {
			{ "lon", std::to_string(imageData.telemetry.longitude), "", "" },
			{ "lat", std::to_string(imageData.telemetry.latitude), "", "" },
			{ "alt", std::to_string(imageData.telemetry.altitudeMSL), "", "" },
			{ "uav_pitch", std::to_string(imageData.telemetry.angle.pitch), "", "" },
			{ "uav_yaw", std::to_string(imageData.telemetry.angle.yaw), "", "" },
			{ "uav_roll", std::to_string(imageData.telemetry.angle.roll), "", "" },
			{ "cam_pitch", "0", "", "" },
			{ "cam_yaw", "0", "", "" },
			{ "cam_roll", "0", "", "" },
			{ "image", jpegData, fileName + ".jpg", "image/jpg" }
		};

		auto res = httpClient.Post(parsedURL.endpoint.c_str(), items);
		std::cout << "Sent: " << std::to_string(imageData.telemetry.longitude) << "\t\t" << imageData.imagePath << std::endl;
	}
}

void imageCaptured(std::string filename, void *readyImageQueuePtr, void *liveTelemetryPtr) {
	moodycamel::BlockingReaderWriterQueue<ImgSys::FileSendData> *readyImageQueue = (moodycamel::BlockingReaderWriterQueue<ImgSys::FileSendData>*) readyImageQueuePtr;
	TELEM_SOURCE *liveTelemetry = (TELEM_SOURCE*) liveTelemetryPtr;

	std::cout << "Captured: " << filename << std::endl;

	ImgSys::FileSendData imageData;
	imageData.imagePath = filename;
	imageData.telemetry = liveTelemetry->getCurrentTelemetry();

	readyImageQueue->enqueue(imageData);
}

cxxopts::ParseResult handleCommandLineArgs(int argc, char *argv[]) {
	cxxopts::Options options("Imaging System", "Captures some images, saves them to disk, "
		"tracks telemetry at the time of capture, and sends them to a destination.");
	options.add_options()
		("w,width", "Capture width", cxxopts::value<int>()->default_value(std::to_string(CAP_WIDTH)))
		("h,height", "Capture height", cxxopts::value<int>()->default_value(std::to_string(CAP_HEIGHT)))
		("f,fps", "Capture framerate (between 1 and 30)", cxxopts::value<int>()->default_value(std::to_string(CAP_FPS)))
		("q,quality", "JPEG capture quality (between 1 and 100)", cxxopts::value<int>()->default_value(std::to_string(JPEG_QUALITY)))
		("l,location", "File save location (absolute path)", cxxopts::value<std::string>()->default_value(CAM_SAVEPATH))
		("d,destination", "File send destination address", cxxopts::value<std::string>()->default_value(ADDRESS))
		("s,sensorid", "Camera sensor ids to use for capture", cxxopts::value<std::vector<int>>()->default_value(""))
		("t,testmode", "Enable testmode (no jetson)", cxxopts::value<bool>()->default_value("false"))
	;
	cxxopts::ParseResult argsResult = options.parse(argc, argv);

	if (argsResult["sensorid"].as<std::vector<int>>().size() == 0) {
		std::cout << "ERROR: You must specify at least one camera sensor-id like so:\n\t-s 0\n\t--sensorid=0,1,2\n";
		std::exit(EXIT_FAILURE);
	}

	return argsResult;
}

int main(int argc, char *argv[]) {
	// Command line arguements
	cxxopts::ParseResult argsResult = handleCommandLineArgs(argc, argv);

	moodycamel::BlockingReaderWriterQueue<ImgSys::FileSendData> readyImageQueue(100);
	TELEM_SOURCE liveTelemetry;

	std::thread imageSendThread(imageSend, std::ref(readyImageQueue), std::ref(argsResult));

	ImgSys::GStreamerCameras camPipeline;
	camPipeline.setTestMode(argsResult["testmode"].as<bool>());
	camPipeline.setJpegQuality(argsResult["quality"].as<int>());
	camPipeline.setCaptureCallback(imageCaptured, (void*) &readyImageQueue, (void*) &liveTelemetry);

	for (int camSensorID : argsResult["sensorid"].as<std::vector<int>>()) {
		std::string location = argsResult["location"].as<std::string>() + "cam" + std::to_string(camSensorID) + "_%06d.jpg";
		camPipeline.addCamera(
			camSensorID,
			argsResult["width"].as<int>(),
			argsResult["height"].as<int>(),
			argsResult["fps"].as<int>(),
			location
		);
	}
	
	camPipeline.build();
	camPipeline.beginCapture();

	// Note: No code down here will be reached since GStreamer is in control of the main thread!

	return EXIT_SUCCESS;
}