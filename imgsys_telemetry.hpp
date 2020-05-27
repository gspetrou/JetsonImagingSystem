#ifndef IMGSYS_TELEMETRY_GUST_GUARD_H
#define IMGSYS_TELEMETRY_GUST_GUARD_H

#include <cstdlib>
#include <ctime>
#include "thirdparty/gustUtil.h"
#include "imgsys.hpp"

namespace ImgSys {
	class GustTelemetry: public ImgSys::LiveTelemetry {
	public:
		ImgSys::Telemetry getCurrentTelemetry();
		GustTelemetry();
	};

	class RandomTelemetry: public ImgSys::LiveTelemetry {
	private:
		int upperBound;
		int lowerBound;
		double getRandomDouble() {
			double out = (double) ((rand() % (upperBound - lowerBound + 1)) + lowerBound);
			return out;
		}
	public:
		RandomTelemetry(int lowBound=0, int highBound=100000) {
			srand(time(NULL));
			this->upperBound = highBound;
			this->lowerBound = lowBound;
		}

		ImgSys::Telemetry getCurrentTelemetry() {
			ImgSys::Telemetry telem = {
				this->getRandomDouble(),
				this->getRandomDouble(),
				this->getRandomDouble(),
				{this->getRandomDouble(), this->getRandomDouble(), this->getRandomDouble()}
			};

			return telem;
		}
	};
}


ImgSys::GustTelemetry::GustTelemetry() {

}

ImgSys::Telemetry ImgSys::GustTelemetry::getCurrentTelemetry() {
	ImgSys::Telemetry telem = {
		0.0,
		1.0,
		2.0,
		{3.0, 4.0, 5.0}
	};

	return telem;
}

#endif // IMGSYS_TELEMETRY_GUST_GUARD_H