#include <string>
#include <iostream>
#include "imgsys.hpp"

std::string ImgSys::fullFilePathToFileName(std::string fullPath) {
	const std::size_t lastSlashIndex = fullPath.find_last_of("\\/");
	if (std::string::npos != lastSlashIndex){
		fullPath.erase(0, lastSlashIndex + 1);
	}
	const std::size_t periodIndex = fullPath.rfind('.');
	if (std::string::npos != periodIndex) {
		fullPath.erase(periodIndex);
	}
	return fullPath;
}

ImgSys::ParsedURL ImgSys::parseURL(std::string fullURL) {
	ImgSys::ParsedURL output = {"localhost", "/", 8000};

	std::size_t protocolPos;
	std::string protocol = "";
	if ((protocolPos = fullURL.find("http://")) != std::string::npos) {
		fullURL = fullURL.substr(protocolPos + 7);
		protocol = "http://";
	} else if ((protocolPos = fullURL.find("https://")) != std::string::npos) {
		fullURL = fullURL.substr(protocolPos + 8);
		protocol = "https://";
	}

	std::size_t colonPos = fullURL.find_first_of(":");
	if (colonPos != std::string::npos) {
		output.address = protocol + fullURL.substr(0, colonPos);
		fullURL = fullURL.substr(colonPos+1);

		std::size_t slashPos = fullURL.find_first_of("/");
		if (slashPos != std::string::npos) {
			output.port = std::stoi(fullURL.substr(0, slashPos));
			output.endpoint = fullURL.substr(slashPos);
		} else {
			output.port = std::stoi(fullURL);
		}
	} else {
		std::size_t slashPos = fullURL.find_first_of("/");
		if (slashPos != std::string::npos) {
			output.address = protocol + fullURL.substr(0, slashPos);
			output.endpoint = fullURL.substr(slashPos);
		} else {
			output.address = protocol + fullURL;
		}
	}

	return output;
}