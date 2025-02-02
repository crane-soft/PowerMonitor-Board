#pragma  once

template <typename AvrType_t>
class csAverage {
public:
	csAverage(AvrType_t *BuffPtr, int Size) {
		Buffer = BuffPtr;
		BuffSize = Size;
		BuffIdx = 0;
		AvrAccu = 0;
		ClearLevel();
	}

	void ClearLevel() {
		FillLevel = 0;
	}

	void AddValue (AvrType_t NewValue) {
		AvrType_t * BuffPtr;
		BuffPtr = &Buffer[BuffIdx];
		AvrAccu -= *BuffPtr;
		AvrAccu += NewValue;
		*BuffPtr = NewValue;

		if (FillLevel < BuffSize) {
			++FillLevel;
		}
		if (++BuffIdx >= BuffSize) {
			BuffIdx = 0;
		}
	}

	AvrType_t GetAvrValue() {
		return AvrAccu / FillLevel;
	}

	AvrType_t GetAvrValue(int Factor) {
		return (AvrAccu * Factor) / FillLevel;
	}

private:
	uint16_t FillLevel;
	int16_t BuffIdx;
	int16_t BuffSize;
	int32_t AvrAccu;

	AvrType_t *Buffer;
};


template <typename AvrType_t, int const BuffSize>
class csAverageBuff : public csAverage <AvrType_t> {
public:
	csAverageBuff() : csAverage <AvrType_t> (Buffer, BuffSize){}
private:
	AvrType_t Buffer[BuffSize];
};


