/**********************************************************\
* File: VEventDecoder.h                                    *
*                                                          *
* Author:                                                  *
* Time-stamp:                                              *
* Description: Contains the base functions needed to       *
*              Encode and Decode CODA data and distribute  *
*              to the subsystems
\**********************************************************/

#include <vector>

#include "RTypes.h"

class VEventDecoder{
public:
	VEventDecoder()  { };
	~VEventDecoder() { };

protected:
	// Encoding Functions
	virtual std::vector<UInt_t> EncodePHYSEventHeader() = 0;
	virtual void EncodePrestartEventHeader(int* buffer, int buffer_size, int runnumber, int runtype = 0) = 0;
  virtual void EncodeGoEventHeader(int* buffer, buffer_size) = 0;
  virtual void EncodePauseEventHeader(int* buffer, int buffer_size) = 0;
  virtual void EncodeEndEventHeader(int* buffer, int buffer_size) = 0;


};
