//========================================================================================
//  
//  $HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/MemoryXferBytes.cpp $
//  
//  Owner: IZine Solutions
//  
//  $Author: rajkumar.sehrawat $
//  
//  $Date: 2010-11-10 07:34:00 +0100 (Wed, 10 Nov 2010) $
//  
//  $Revision: 2485 $
//..
//  Copyright 1997-2007 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPlugInHeaders.h"

// Interface includes:
// none.

// Project includes:
#include "MemoryXferBytes.h"
//#include "IZPID.h" // for kPrtMemPluginName

/* Constructor
*/
MemoryXferBytes::MemoryXferBytes(void)
: 	fMyBuffer(nil),
	fCurrentMaxBuf(ePrtMemInitialMaxBuf),
	fAbsolutePositionInBuffer(0),
	fCountBytesStored(0),
	fStreamState(kStreamStateGood)
{
	fMyBuffer = new uint8 [fCurrentMaxBuf];
	ASSERT(fMyBuffer);
	memset(fMyBuffer, 0,fCurrentMaxBuf);
	//TRACEFLOW(kIZPPluginName, "MemoryXferBytes constructor\n");

}

/* Destructor
*/
MemoryXferBytes::~MemoryXferBytes(void)
{
	//TRACEFLOW(kIZPPluginName, "MemoryXferBytes destructor\n");
	this->dumpState();
	if (fMyBuffer)
	{
		delete [] fMyBuffer;
		fMyBuffer = nil;
	}
}

/* resizeBuffer
*/
void MemoryXferBytes::resizeBuffer(uint32 newSize)
{
	do
	{
		bool16 preconditions = (newSize > ePrtMemInitialMaxBuf);
		if (preconditions == kFalse)
		{
			ASSERT_FAIL("preconditions are not met");
			break;
		}

		// create a new temporary buffer that is bigger.
		// (Don't just resize by a little amount...)
		uint32 newTargetBufferSize = ePrtMemInitialMaxBuf + newSize;
		uint8* tempBuffer = new uint8 [newTargetBufferSize];
		if (tempBuffer == nil)
		{
			ASSERT_FAIL("new operator failed to allocate for tempBuffer");
			break;
		}
		// initialize the temp buffer data to 0
		memset(tempBuffer, 0, newSize);
		ASSERT(tempBuffer);
		// copy the data from existing buffer to temp buffer 
		memcpy(tempBuffer, this->fMyBuffer, this->fCurrentMaxBuf);

		// delete the buffer (member variable)
		delete [] fMyBuffer;
		// reassign member pointers and size
		this->fMyBuffer = tempBuffer;
		this->fCurrentMaxBuf = newTargetBufferSize;

		// Postconditions: fMyBuffer is not nil
		ASSERT(tempBuffer);

		// It is able to store newTargetBufferSize bytes
	} while (kFalse);
}


/* Read
*/
uint32 MemoryXferBytes::Read(void* buffer, uint32 pnum)
{
	// TODO - test
	// ipaterso: I've not tested this at all, as I've only use a Write stream
	// with this code
	uint32 numToTransfer = 0;
	do
	{
		if (buffer == nil)
		{
			ASSERT_FAIL("buffer is nil!");
			break;
		}
		
		uint32 numLeftInBuffer = this->fCountBytesStored - this->fAbsolutePositionInBuffer;
	
		if (pnum > numLeftInBuffer)
		{
			numToTransfer = numLeftInBuffer;
			fStreamState = kStreamStateEOF;
		}
		else
		{
			numToTransfer = pnum;
		}
		// Something like:
		// memcpy (buffer, <somewhere in buffer>, numToTransfer);
		bool16 canRead = (fAbsolutePositionInBuffer + numToTransfer) < fCurrentMaxBuf;
		if (canRead)
		{
			memcpy(buffer, fMyBuffer + fAbsolutePositionInBuffer, numToTransfer);
		}
		else
		{
			ASSERT_FAIL("canRead is false, our buffer isn't big enough");
		}

	} while (false);
	return numToTransfer;

}


/* Write
*/
uint32 MemoryXferBytes::Write(void* buffer, uint32 pnum)
{

	uint32 numToTransfer = 0;
	do
	{
		if (buffer == nil)
		{
			ASSERT_FAIL("buffer is nil!");
			break;
		}

		numToTransfer = pnum;
		// We want to add this data at fAbsolutePositionInBuffer...
		uint32 endOfWriteBytes = (this->fAbsolutePositionInBuffer + numToTransfer);
		bool16 noOverflow = endOfWriteBytes < fCurrentMaxBuf;
		if (noOverflow == kFalse)
		{
			this->resizeBuffer(endOfWriteBytes + 1);
		}
		noOverflow = endOfWriteBytes < fCurrentMaxBuf;
		ASSERT(noOverflow);

		if (numToTransfer > 0 && noOverflow)
		{
			memcpy(this->fMyBuffer + this->fAbsolutePositionInBuffer, buffer, numToTransfer);
			this->fAbsolutePositionInBuffer += numToTransfer;
			if (fAbsolutePositionInBuffer > this->fCountBytesStored)
			{
				this->fCountBytesStored = this->fAbsolutePositionInBuffer;
			}
		}
	} while (false);
	return numToTransfer;
}

/* Seek
*/
uint32 MemoryXferBytes::Seek(int32 numberOfBytes, SeekFromWhere fromHere)
{
	if (fStreamState == kStreamStateEOF)
	{
		fStreamState = kStreamStateGood;
	}
	switch (fromHere)
	{
	case kSeekFromStart:
		this->fAbsolutePositionInBuffer = numberOfBytes;
		break;
	case kSeekFromCurrent:
		this->fAbsolutePositionInBuffer = this->fAbsolutePositionInBuffer + numberOfBytes;
		break;
	case kSeekFromEnd:
		this->fAbsolutePositionInBuffer = this->fCountBytesStored + numberOfBytes;
		break;
	}
	return fAbsolutePositionInBuffer;
}

/* Flush
*/
void MemoryXferBytes::Flush(void)
{
	// Do nothing.
	// I guess we'd only care about this in file-based implementation,
	// at which point we'd flush to disk.
}

/* GetStreamState
*/  
StreamState MemoryXferBytes::GetStreamState(void)
{
	return this->fStreamState;
}

/* SetEndOfStream
*/  
void MemoryXferBytes::SetEndOfStream(void)
{
	this->fCountBytesStored = this->fAbsolutePositionInBuffer;
}

/* getBufferPtr
*/
const uint8* MemoryXferBytes::getBufferPtr(void) const
{
	return this->fMyBuffer;
}

/* dumpState
*/
void MemoryXferBytes::dumpState(void) const
{
#ifdef DEBUG
	//TRACEFLOW(kIZPPluginName, "MemoryXferBytes::dumpState()\n");
	//TRACEFLOW(kIZPPluginName, "-- Abs. position in buffer = 0x%x\n", this->fAbsolutePositionInBuffer);
	//TRACEFLOW(kIZPPluginName, "-- Bytes stored = 0x%x\n", this->fCountBytesStored);
	//TRACEFLOW(kIZPPluginName, "-- Current maxbuf = 0x%x\n\n", this->fCurrentMaxBuf);
	PMString data;
	int32 nbytes = 2048;
	if( nbytes > fCountBytesStored )
		nbytes = fCountBytesStored;
	data.SetXString(reinterpret_cast<const char*>(this->getBufferPtr()), nbytes);
	std::string platformStr = data.GetPlatformString();
	//TRACEFLOW(kIZPPluginName, "First %d bytes: \n%s\n\n\n", nbytes, platformStr.c_str());
#endif
}

// End, MemoryXferBytes.cpp.

