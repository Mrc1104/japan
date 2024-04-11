#include "Coda3EventDecoder.h"

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
	header.push_back(++fEvtNumber );
	header.push_back(0x0);

	// evttime is held by a 64 bit (bits 0-48 is the time) ... for now we set the upper 32 bits to 0
	header.push_back(localtime);
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
