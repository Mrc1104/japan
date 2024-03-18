#include "QwCoda3Decoder.h"
#include <stdexcept>


//_____________________________________________________________________________
void QwCoda3Decoder::debug_print( const UInt_t event_type, const UInt_t* evbuffer ) const
{
  // checks of ET-inserted data
  Int_t print_it=0;

  switch( event_type ) {

  case EPICS_EVTYPE:
    QwMessage << "EPICS data "<<QwLog::endl;
    print_it=1;
    break;
	// Do we need this event?
  case PRESCALE_EVTYPE:
    QwMessage << "Prescale data "<<QwLog::endl;
    print_it=1;
    break;
	// Do we need this event?
  case DAQCONFIG_FILE1:
    QwMessage << "DAQ config file 1 "<<QwLog::endl;
    print_it=1;
    break;
	// Do we need this event?
  case DAQCONFIG_FILE2:
    QwMessage << "DAQ config file 2 "<<QwLog::endl;
    print_it=1;
    break;
	// Do we need this event?
  case SCALER_EVTYPE:
    QwMessage << "LHRS scaler event "<<QwLog::endl;
    print_it=1;
    break;
	// Do we need this event?
  case SBSSCALER_EVTYPE:
    QwMessage << "SBS scaler event "<<QwLog::endl;
    print_it=1;
    break;
	// Do we need this event?
  case HV_DATA_EVTYPE:
    QwMessage << "High voltage data event "<<QwLog::endl;
    print_it=1;
    break;
  default:
    // something else ?
    QwMessage << "\n--- Special event type: " << event_type << "\n" << QwLog::endl;
  }
  if(print_it) {
    char *cbuf = (char *)evbuffer; // These are character data
    size_t elen = sizeof(int)*(evbuffer[0]+1);
    QwMessage << "Dump of event buffer .  Len = "<<elen<<QwLog::endl;
    // This dump will look exactly like the text file that was inserted.
    for (size_t ii=0; ii<elen; ii++) QwMessage << cbuf[ii];
  }
}



//_____________________________________________________________________________
uint32_t QwCoda3Decoder::TBOBJ::Fill( const uint32_t* evbuffer,
                                   uint32_t blkSize, uint32_t tsroc )
{
  if( blkSize == 0 )
    throw std::invalid_argument("CODA block size must be > 0");
  start = evbuffer;
  blksize = blkSize;
  len = evbuffer[0] + 1;
  tag = (evbuffer[1] & 0xffff0000) >> 16;
  nrocs = evbuffer[1] & 0xff;

  const uint32_t* p = evbuffer + 2;
  // Segment 1:
  //  uint64_t event_number
  //  uint64_t run_info                if withRunInfo
  //  uint64_t time_stamp[blkSize]     if withTimeStamp
  {
    uint32_t slen = *p & 0xffff;
    if( slen != 2*(1 + (withRunInfo() ? 1 : 0) + (withTimeStamp() ? blkSize : 0)))
      throw coda_format_error("Invalid length for Trigger Bank seg 1");
    const auto* q = (const uint64_t*) (p + 1);
    evtNum  = *q++;
    runInfo = withRunInfo()   ? *q++ : 0;
    evTS    = withTimeStamp() ? q    : nullptr;
    p += slen + 1;
  }
  if( p-evbuffer >= len )
    throw coda_format_error("Past end of bank after Trigger Bank seg 1");

  // Segment 2:
  //  uint16_t event_type[blkSize]
  //  padded to next 32-bit boundary
  {
    uint32_t slen = *p & 0xffff;
    if( slen != (blkSize-1)/2 + 1 )
      throw coda_format_error("Invalid length for Trigger Bank seg 2");
    evType = (const uint16_t*) (p + 1);
    p += slen + 1;
  }

  // nroc ROC segments containing timestamps and optional
  // data like trigger latch bits:
  // struct {
  //   uint64_t roc_time_stamp;     // Lower 48 bits only seem to be the time.
  //   uint32_t roc_trigger_bits;   // Optional. Typically only in TSROC.
  // } roc_segment[blkSize];
  TSROC = nullptr;
  tsrocLen = 0;
  for( uint32_t i = 0; i < nrocs; ++i ) {
    if( p-evbuffer >= len )
      throw coda_format_error("Past end of bank while scanning trigger bank segments");
    uint32_t slen = *p & 0xffff;
    uint32_t rocnum = (*p & 0xff000000) >> 24;
		QwMessage << "mrc ... " << std::hex << *p 
							<< "\nrocnum " << rocnum 
							<< "\ntsroc " << tsroc << std::dec << QwLog::endl;
		// tsroc is the crate # of the TS
		// This is filled with the THaCrateMap class which we are not using
		// can we just remove the if block below?
    if( rocnum == tsroc ) {
      TSROC = p + 1;
      tsrocLen = slen;
      break;
    }
    p += slen + 1;
  }

  return len;
}

Int_t QwCoda3Decoder::LoadTrigBankInfo( UInt_t i )
{
  // CODA3: Load tsEvType, evt_time, and trigger_bits for i-th event
  // in event block buffer. index_buffer must be < block size.

  assert(i < tbank.blksize);
  if( i >= tbank.blksize )
    return -1;
  tsEvType = tbank.evType[i];      // event type (configuration-dependent)
  if( tbank.evTS )
    evt_time = tbank.evTS[i];      // event time (4ns clock, I think)
  else if( tbank.TSROC ) {
    UInt_t struct_size = tbank.withTriggerBits() ? 3 : 2;
    evt_time = *(const uint64_t*) (tbank.TSROC + struct_size * i);
    // Only the lower 48 bits seem to contain the time
    evt_time &= 0x0000FFFFFFFFFFFF;
  }
  if( tbank.withTriggerBits() )
    // Trigger bits. Only the lower 6 bits seem to contain the actual bits
    trigger_bits = tbank.TSROC[2 + 3 * i] & 0x3F;

  return 0;
}
