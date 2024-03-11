#ifndef QWCODA3DECODER_H
#define QWCODA3DECODER_H

#include "QwOptions.h"
#include <stdexcept>
class QwCoda3Decoder
{
public:
	QwCoda3Decoder() { }
protected:
	// Virtual Parsing Functions that need to be overridden in QwEventBuffer	
 	virtual Int_t DecodeEventIDBankCoda3(const UInt_t* evbuffer) = 0;
	virtual Int_t physics_decode( const UInt_t* evbuffer ) = 0; 
  virtual Int_t interpretCoda3( const UInt_t* evbuffer ) = 0;
  virtual Int_t FindRocsCoda3(const UInt_t *evbuffer)    = 0; // CODA3 version
  virtual Int_t trigBankDecode( const UInt_t* evbuffer ) = 0;

  // Parsing support functions	
	void debug_print(const UInt_t event_type, const UInt_t* evbuffer ) const;
  static UInt_t InterpretBankTag( UInt_t tag );

  // Optional functionality that may be implemented by derived classes
  virtual ULong64_t GetEvTime() const { return evt_time; }
  void    SetEvTime( ULong64_t evtime ) { evt_time = evtime; }
protected:
	UInt_t run_type;
	
  // CODA3 stuff
  UInt_t blkidx;  // Event block index (0 <= blkidx < block_size)
  Bool_t fMultiBlockMode, fBlockIsDone;
  UInt_t tsEvType, bank_tag, block_size;
  ULong64_t evt_time; // Event time (for CODA 3.* this is a 250 Mhz clock)
  UInt_t trigger_bits;

public:
  // CodaDecoder.h Error Handling
  
  // Return codes for DecodeEventIDBankCoda3
  enum { HED_OK = 0, HED_WARN = -63, HED_ERR = -127, HED_FATAL = -255 };

  // CODA file format exception, thrown by LoadEvent/LoadFromMultiBlock
  class coda_format_error : public std::runtime_error {
  public:
    explicit coda_format_error( const std::string& what_arg )
      : std::runtime_error(what_arg) {}
    explicit coda_format_error( const char* what_arg )
      : std::runtime_error(what_arg) {}
  };

public:
// Class for TI Trigger Bank Data
  class TBOBJ {
  public:
     TBOBJ() : blksize(0), tag(0), nrocs(0), len(0), tsrocLen(0), evtNum(0),
               runInfo(0), start(nullptr), evTS(nullptr), evType(nullptr),
               TSROC(nullptr) {}
     void     Clear() { memset(this, 0, sizeof(*this)); }
     uint32_t Fill( const uint32_t* evbuffer, uint32_t blkSize, uint32_t tsroc );
     bool     withTimeStamp()   const { return (tag & 1) != 0; }
     bool     withRunInfo()     const { return (tag & 2) != 0; }
     bool     withTriggerBits() const { return (tsrocLen > 2*blksize);}

     uint32_t blksize;          /* total number of triggers in the Bank */
     uint16_t tag;              /* Trigger Bank Tag ID = 0xff2x */
     uint16_t nrocs;            /* Number of ROC Banks in the Event Block (val = 1-256) */
     uint32_t len;              /* Total Length of the Trigger Bank - including Bank header */
     uint32_t tsrocLen;         /* Number of words in TSROC array */
     uint64_t evtNum;           /* Starting Event # of the Block */
     uint64_t runInfo;          /* Run Info Data (optional) */
     const uint32_t *start;     /* Pointer to start of the Trigger Bank */
     const uint64_t *evTS;      /* Pointer to the array of Time Stamps (optional) */
     const uint16_t *evType;    /* Pointer to the array of Event Types */
     const uint32_t *TSROC;     /* Pointer to Trigger Supervisor ROC segment data */
   };

protected:
   Int_t LoadTrigBankInfo( UInt_t index_buffer );
   TBOBJ tbank;

public:
	// KEYWORDS
  static const UInt_t MAX_PHYS_EVTYPE  = 14;  // Types up to this are physics
  static const UInt_t SYNC_EVTYPE      = 16;
  static const UInt_t PRESTART_EVTYPE  = 17;
  static const UInt_t GO_EVTYPE        = 18;
  static const UInt_t PAUSE_EVTYPE     = 19;
  static const UInt_t END_EVTYPE       = 20;
  static const UInt_t TS_PRESCALE_EVTYPE  = 120;
  // should be able to load special event types from crate map
  static const UInt_t EPICS_EVTYPE     = 131; // default in Hall A
  static const UInt_t PRESCALE_EVTYPE  = 133;
  static const UInt_t DETMAP_FILE      = 135;
  static const UInt_t DAQCONFIG_FILE1  = 137;
  static const UInt_t DAQCONFIG_FILE2  = 138;
  static const UInt_t TRIGGER_FILE     = 136;
  static const UInt_t SCALER_EVTYPE    = 140;
  static const UInt_t SBSSCALER_EVTYPE = 141;
  static const UInt_t HV_DATA_EVTYPE   = 150;

// ClassDef(QwCoda3Decoder,0) // Decoder for CODA event buffer
};
#endif
