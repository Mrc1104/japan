#include "VEventDecoder.h"
#include "RTypes.h"

#include <vector>

class Coda2EventDecoder : public VEventDecoder
{
public:
		Coda2EventDecoder() { }
		~Coda2EventDecoder() { }
protected:
	virtual std::vector<UInt_t> EncodePHYSEventHeader();
	virtual void EncodePrestartEventHeader(int* buffer, int buffer_size, int runnumber, int runtype = 0);
  virtual void EncodeGoEventHeader(int* buffer, buffer_size);
  virtual void EncodePauseEventHeader(int* buffer, int buffer_size);
  virtual void EncodeEndEventHeader(int* buffer, int buffer_size);

};
