#include "Coda3EventDecoder.h"
#include "THaCodaFile.h"

#include <vector>
#include <ctime>


// Encoding Functions

std::vector<UInt_t> Coda3EventDecoder::EncodePHYSEventHeader()
{
 	int localtime = (int) time(0);
	// TODO:
	// Could we make this more dynamic by reversing the TBOBJ::Fill mechanism?
	header.push_back(0xFF501001);
	header.push_back(0x0000000b); // word count for Trigger Bank
	header.push_back(0xFF212001); // 0x001 = # of ROCs (is this an issue if we have multiple rocs?)
	header.push_back(0x010a0004); 

	// evtnum is held by a 64 bit ... for now we set the upper 32 bits to 0
	// I don't think I have this order correct....
	header.push_back(++fEvtNumber ); // Isn't this the upper 32 ?
	header.push_back(0x0);

	// evttime is held by a 64 bit (bits 0-48 is the time) ... for now we set the upper 32 bits to 0
	// I don't think I have this order correct...
	header.push_back(localtime); // Isn't this the upper 32 ?
	header.push_back(0x0);

	header.push_back(0x1850001);
	header.push_back(0xc0da); // TS# Trigger
	header.push_back(0x2010002);
	header.push_back(0xc0da01);  	
	header.push_back(0xc0da02);	
	return header;
}


void Coda3EventDecoder::EncodePrestartEventHeader(int* buffer, int buffer_size, int runnumber, int runtype = 0)
{
	int localtime  = (int)time(0);
	int eventcount = 0;
	buffer[0] = 4; // Prestart event length
	// TODO: We need access to the ControlEvent enum
	buffer[1] = ((kPRESTART_EVENT << 16) | (0x01 << 8) | 0xCC);
	buffer[2] = localtime;
	buffer[3] = runnumber;
	buffer[4] = runtype;
}

void Coda3EventDecoder::EncodeGoEventHeader(int* buffer, int buffer_size)
{
	int localtime  = (int)time(0);
	int eventcount = 0;
	// TODO: We need access to the ControlEvent enum
	buffer[0] = 4; // Go event length
	buffer[1] = ((0xffd1 << 16) | (0x01 << 8) );
	buffer[2] = localtime;
	buffer[3] = 0; // unused
	buffer[4] = eventcount;
}

void Coda3EventDecoder::EncodePauseEventHeader(int* buffer, int buffer_size)
{
	int localtime  = (int)time(0);
	int eventcount = 0;
	// TODO: We need access to the ControlEvent enum
	buffer[0] = 4; // Pause event length
  buffer[1] = ((0xffd2 << 16) | (0x01 << 8) );
	buffer[2] =	localtime;
	buffer[3] = 0; // unused
	buffer[4] = eventcount;
}

void Coda3EventDecoder::EncodeEndEventHeader(int* buffer, int buffer_size)
{
	int localtime  = (int)time(0);
	int eventcount = 0;
	// TODO: We need access to the ControlEvent enum
	buffer[0] = 4; // End event length
  buffer[1] = ((0xffd3 << 16) | (0x01 << 8) );
	buffer[2] = localtime;
	buffer[3] = 0; // unused
	buffer[4] = eventcount; 
}


void Coda3EventDecoder::DecodeEventIDBank(UInt_t *buffer)
{

  fPhysicsEventFlag = kFALSE;
  Int_t ret = HED_OK;

  // Main engine for decoding, called by public LoadEvent() methods
  assert(evbuffer);

  fEvtLength = evbuffer[0]+1;  // in longwords (4 bytes)
  fEvtType = 0;
	fEvtTag = 0;
  fBankDataType = 0;
	// Trigger Bank vars
  evt_time = 0;
	trigger_bits = 0;
	block_size = 0;

  // Determine event type
  interpretCoda3(evbuffer);
	

	// What to do with bad events?	
  if( fEvtType <= MAX_PHYS_EVTYPE && ( (ret = trigBankDecode(evbuffer)) == HED_OK ) ) {
  	fPhysicsEventFlag = kTRUE;
		// Originally from HallA::CodaDecoder::physics_decode which called
		// and HallA::CodaDecoder::FindRocsCoda3
		// Both of which had extra CrateMap logic (not needed for JAPAN)
		// Below are the 4 lines needed from those two functions
    fEvtNumber = tbank.evtNum;
    UInt_t pos = 2 + tbank.len;
	  fWordsSoFar = (pos); 
	  fBankDataType = (evbuffer[pos+1] & 0xff00) >> 8;
  	//  Initialize the fragment size to the event size, in case the 
  	//  event is not subbanked.
  	fFragLength = fEvtLength-fWordsSoFar;
		// TODO:
		// What to do with fEvtClass and fStatSum ?
		return ret;
  }

	// TrigBankDecode failed, return error code
	if( ret != HED_OK)
	{
		QwWarning << "trigBankDecode returned with status = " << ret << QwLog::endl;
		return ret;
	}

	//  Run this event through the Control event processing.
	//  If it is not a control event, nothing will happen.
	fBankDataType = (evbuffer[1] & 0xff00) >> 8; // not sure if this works
  fEvtNumber = 0;
  fWordsSoFar = (2);
  //  Initialize the fragment size to the event size, in case the 
 	//  event is not subbanked.
 	fFragLength = fEvtLength-fWordsSoFar;
	// TODO:
	// What to do with fEvtClass and fStatSum ?
	ProcessControlEvent(fEvtType, &evbuffer[fWordsSoFar]);
  return ret;

}
