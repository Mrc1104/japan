#include "Coda2EventDecoder.h"

#include <vector>
#include <ctime>

// Encoding Functions

std::vector<UInt_t> Coda2EventDecoder::EncodePHYSEventHeader()
{
	vector<UInt_t> header;
	header.push_back((0x0001 << 16) | (0x10 << 8) | 0xCC);
	// event type | event data type | event ID (0xCC for CODA event)
	header.push_back(4);	// size of header field
	header.push_back((0xC000 << 16) | (0x01 << 8) | 0x00);
	// bank type | bank data type (0x01 for uint32) | bank ID (0x00 for header event)
	header.push_back(++fEvtNumber); // event number (initialized to 0,
	// so increment before use to agree with CODA number)
	header.push_back(1);	// event class
	header.push_back(0);	// status summary
	return header;
}


void Coda2EventDecoder::EncodePrestartEventHeader(int* buffer, int buffer_size, int runnumber, int runtype = 0)
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

void Coda2EventDecoder::EncodeGoEventHeader(int* buffer, int buffer_size)
{
	int localtime  = (int)time(0);
	int eventcount = 0;
	// TODO: We need access to the ControlEvent enum
	buffer[0] = 4; // Go event length
	buffer[1] = buffer[1] = ((kGO_EVENT << 16) | (0x01 << 8) | 0xCC);
	buffer[2] = localtime;
	buffer[3] = 0; // unused
	buffer[4] = eventcount;
}

void Coda2EventDecoder::EncodePauseEventHeader(int* buffer, int buffer_size)
{
	int localtime  = (int)time(0);
	int eventcount = 0;
	// TODO: We need access to the ControlEvent enum
	buffer[0] = 4; // Pause event length
	buffer[1] = ((kPAUSE_EVENT << 16) | (0x01 << 8) | 0xCC);
	buffer[2] =	localtime;
	buffer[3] = 0; // unused
	buffer[4] = eventcount;
}

void Coda2EventDecoder::EncodeEndEventHeader(int* buffer, int buffer_size)
{
	int localtime  = (int)time(0);
	int eventcount = 0;
	// TODO: We need access to the ControlEvent enum
	buffer[0] = 4; // End event length
	buffer[1] = buffer[1] = ((kEND_EVENT << 16) | (0x01 << 8) | 0xCC);
	buffer[2] = localtime;
	buffer[3] = 0; // unused
	buffer[4] = eventcount; 
}
