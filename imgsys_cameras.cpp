/************
	Includes
*************/
// Input/Output
#include <iostream>
// Strings
#include <string>
// Vectors
#include <vector>
// Helper file
#include "imgsys.hpp"

/**********************************
	ImgSys::Cameras Implementation
***********************************/
// Constructors
ImgSys::Cameras::Cameras() {
	this->jpegQuality = 80;
	this->wasBuilt = false;
	this->beganCapture = false;
	this->testMode = false;
}

// Getters/Setters
int ImgSys::Cameras::getJpegQuality() { return this->jpegQuality; }
void ImgSys::Cameras::setJpegQuality(int quality) {
	this->jpegQuality = quality;
}

bool ImgSys::Cameras::hasBeganCapture() { return this->beganCapture; }
bool ImgSys::Cameras::hasBeenBuilt() { return this->wasBuilt; }

bool ImgSys::Cameras::getTestMode() { return this->testMode; }
void ImgSys::Cameras::setTestMode(bool t) { this->testMode = t; }

void ImgSys::Cameras::setCaptureCallback(ImgSys::ImageCaptureCallback callback, void *imageQueue, void *liveTelem) {
	this->captureCallback = callback;
	this->capCallbackData_Queue = imageQueue;
	this->capCallbackData_Telem = liveTelem;
}

std::vector<ImgSys::Cameras::CameraDetail> ImgSys::Cameras::getCameraList() { return this->cameras; }
void ImgSys::Cameras::addCamera(int sensorID, int width, int height, int fps, std::string fileSavePath) {
	this->cameras.push_back({sensorID, width, height, fps, fileSavePath});
}