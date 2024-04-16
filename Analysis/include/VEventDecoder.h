#ifndef VEVENTDECODER_H
#define VEVENTDECODER_H

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

public:
	// Encoding Functions
	virtual std::vector<UInt_t> EncodePHYSEventHeader()                = 0;
	virtual void EncodePrestartEventHeader(int* buffer, int buffer_size, int runnumber, int runtype = 0) = 0;
  virtual void EncodeGoEventHeader(int* buffer, buffer_size)         = 0;
  virtual void EncodePauseEventHeader(int* buffer, int buffer_size)  = 0;
  virtual void EncodeEndEventHeader(int* buffer, int buffer_size)    = 0;

public:
	// Decoding Functions
  virtual void DecodeEventIDBank(UInt_t *buffer) = 0;
  Bool_t DecodeSubbankHeader(UInt_t *buffer) = 0;

protected:
	// Information we need to extact from the decoding
	UInt_t fWordsSoFar;
	UInt_t fEvtLength;	
	UInt_t fEvtType;
	UInt_t fEvtTag;
	UInt_t fBankDataType;
	Bool_t fPhysicsEventFlag;
  UInt_t fEvtNumber;   ///< CODA event number; only defined for physics events
  UInt_t fEvtClass;
  UInt_t fStatSum;
  UInt_t fFragLength;
  BankID_t fSubbankTag;
  UInt_t fSubbankType;
  UInt_t fSubbankNum;
  ROCID_t fROC;
	// Information needed for proper decoding
	fAllowLowSubbankIDs

protected:
 	// MQwCodaControlEvent Keywords (duplicate from MQwCodaControlEvent... might want to move to its own header file and #include it)
	enum EventTypes{
    kSYNC_EVENT        =  16,
    kPRESTART_EVENT    =  17,
    kGO_EVENT          =  18,
    kPAUSE_EVENT       =  19,
    kEND_EVENT         =  20
  };

};

#endif
