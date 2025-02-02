#pragma once
#include <Arduino.h>
class csTimer {
public:
	csTimer() {
		Timeout = 0;
        TimeStart = 0;
	}

	void Set(int32_t Time) {
        TimeStart = millis();
		Timeout = TimeStart + Time;
	}

	int Get() {
		return millis()- TimeStart;
	}

	int32_t Elapsed() {
		return (millis() > Timeout);
	}

private:
    uint32_t TimeStart;
	uint32_t Timeout;
};
