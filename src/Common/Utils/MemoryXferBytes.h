//========================================================================================
//  
//  $HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/MemoryXferBytes.h $
//  
//  Owner: IZine Solutions
//  
//  $Author: rajkumar.sehrawat $
//  
//  $Date: 2010-05-31 14:52:19 +0200 (Mon, 31 May 2010) $
//  
//  $Revision: 1267 $
//..
//  Copyright 1997-2007 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#ifndef __MemoryXferBytes_h__
#define __MemoryXferBytes_h__

// Interface/General includes:
#include "IXferBytes.h" /* This is NOT an boss interface. */


/** Implements IXferBytes over a memory buffer.
 * 	We associate this with a stream (IPMStream) by calling
 * 	at some point IPMStream::SetXferBytes
 * 	@ingroup printmemorystream
*/
class  MemoryXferBytes : public IXferBytes
{
public:
	/** Constructor
	*/
	MemoryXferBytes(void);

	/** Destructor
	*/
	virtual ~MemoryXferBytes(void);

	//--- The IXferBytes interface we must implement  ---

	/** See IXferBytes::Read
	*/
	virtual uint32 Read(void* buffer, uint32 num);

	/** See IXferBytes::Write
	*/
	virtual uint32 Write(void* buffer, uint32 num);

	/** See IXferBytes::Seek
	*/
	virtual uint32 Seek(int32 numberOfBytes, SeekFromWhere fromHere);

	/** See IXferBytes::Flush
	*/
	virtual void Flush(void);

	/** See IXferBytes::GetStreamState
	*/
	virtual StreamState GetStreamState(void);

	/** See IXferBytes::SetEndOfStream
	*/
	virtual void SetEndOfStream(void);

protected:
	const uint8*  getBufferPtr(void) const;
	void resizeBuffer(uint32 newSize);
	void dumpState(void) const;

private:

	StreamState fStreamState;
	uint32 fCountBytesStored;
	uint32 fAbsolutePositionInBuffer;
	uint8* fMyBuffer;
	uint32 fCurrentMaxBuf;

	enum
	{
		ePrtMemInitialMaxBuf = 1024*1024
	};
};

#endif // __MemoryXferBytes_h__

// End, MemoryXferBytes.h.
