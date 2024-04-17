#ifndef CODA2EVENTDECODER_H
#define CODA2EVEMTDECODER_H

#include "VEventDecoder.h"
#include "Rtypes.h"

#include <vector>

class Coda2EventDecoder : public VEventDecoder
{
public:
		Coda2EventDecoder() { }
		~Coda2EventDecoder() { }
public:
// Encoding Functions
	virtual std::vector<UInt_t> EncodePHYSEventHeader();
	virtual void EncodePrestartEventHeader(int* buffer, int buffer_size, int runnumber, int runtype = 0);
  virtual void EncodeGoEventHeader(int* buffer, int buffer_size);
  virtual void EncodePauseEventHeader(int* buffer, int buffer_size);
  virtual void EncodeEndEventHeader(int* buffer, int buffer_size);
public:
// Decoding Functions
  virtual Int_t DecodeEventIDBank(UInt_t *buffer);
public:
// Boolean Functions
	virtual Bool_t IsPhysicsEvent();
private:
	UInt_t fIDBankNum;

};
#endif
