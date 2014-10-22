//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPCompressionUtils.cpp $
//	$Revision: 2417 $
//	$Date: 2010-10-28 14:53:44 +0200 (Thu, 28 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes
#include "zlib.h"

#include "IZPID.h"

//IZP Interfaces
#include "IZPCompressionUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define INCREASE 10240
#ifndef max
#define max(x,y) ((x) > (y) ? (x) : (y))
#endif

#define inheritClass CPMUnknown<IZPCompressionUtils>
class CZPCompressionUtils : public inheritClass
{
public:
						CZPCompressionUtils(IPMUnknown * boss);
	virtual				~CZPCompressionUtils();

	int					Compress(
							const unsigned char *		inSourceBuff,
							unsigned int				inSourceLen,
							unsigned char **			oTargetBuff,
							unsigned int *				oTargetLen) const;

	int					Decompress(
							const unsigned char *		inSourceBuff,
							unsigned int				inSourceLen,
							unsigned char **			oTargetBuff,
							unsigned int *				oTargetLen) const;
protected:
private:
};


CREATE_PMINTERFACE(CZPCompressionUtils, kZPCompressionUtilsImpl)

CZPCompressionUtils::CZPCompressionUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCompressionUtils::~CZPCompressionUtils()
{

}

#pragma mark -
/** Compress data by GZIP in Memory
 
 @param inSourceBuff Pointer to the data to be compressed
 @param inSourceLen len of sourcedata
 @param oTargetBuff Pointer to the result. Caller has to 'free' it.
 @param oTargetLen Len of targetdata
 @return always 0
 **/
int
CZPCompressionUtils::Compress(
	const unsigned char *		inSourceBuff,
	unsigned int				inSourceLen,
	unsigned char **			oTargetBuff,
	unsigned int *				oTargetLen) const
{
	LogFunctionEnterExit;
	
	z_stream c_stream;
	memset(&c_stream, 0, sizeof(c_stream));
	
	int ret = 0;
	int err;
	int alloclen = max(inSourceLen, INCREASE);
	
	c_stream.zalloc = NULL;
	c_stream.zfree = NULL;
	c_stream.opaque = NULL;
	
	*oTargetBuff = (unsigned char *) malloc(alloclen);
	
	// Initialisation, so that GZIP will be created
	if (deflateInit2(&c_stream,Z_BEST_COMPRESSION,Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK)
	{
		c_stream.next_in  = (Bytef*)inSourceBuff;
		c_stream.avail_in = inSourceLen;
		c_stream.next_out = *oTargetBuff;
		c_stream.avail_out = alloclen;
		
		while (c_stream.total_in != inSourceLen && c_stream.total_out < *oTargetLen)
		{
			err = deflate(&c_stream, Z_NO_FLUSH);
			// CHECK_ERR(err, "deflate");
			if (c_stream.avail_out == 0)
			{
				// Alloc new memory
				int now = alloclen;
				alloclen += alloclen / 10 + INCREASE;
				*oTargetBuff = (unsigned char *) realloc(*oTargetBuff, alloclen);
				c_stream.next_out = *oTargetBuff + now;
				c_stream.avail_out = alloclen - now;
			}
		}
		// Finish the stream
		for (;;)
		{
			err = deflate(&c_stream, Z_FINISH);
			if (err == Z_STREAM_END) break;
			if (c_stream.avail_out == 0)
			{
				// Alloc new memory
				int now = alloclen;
				alloclen += alloclen / 10 + INCREASE;
				*oTargetBuff = (unsigned char *) realloc(*oTargetBuff, alloclen);
				c_stream.next_out = *oTargetBuff + now;
				c_stream.avail_out = alloclen - now;
			}
			// CHECK_ERR(err, "deflate");
		}
		
		err = deflateEnd(&c_stream);
		// CHECK_ERR(err, "deflateEnd");
	}
	*oTargetLen = c_stream.total_out;
	// free remaining memory
	*oTargetBuff = (unsigned char *) realloc(*oTargetBuff, *oTargetLen);
	
	return ret;
}

/** Inflate data with GZIP
 
 @param inSourceBuff Pointer to the compressed data
 @param inSourceLen Len of compressed data
 @param oTargetBuff Pointer to the inflated data, Caller has to 'free' it.
 @param oTargetLen Len of inflated data
 @return always 0
 **/
int
CZPCompressionUtils::Decompress(
	const unsigned char *		inSourceBuff,
	unsigned int				inSourceLen,
	unsigned char **			oTargetBuff,
	unsigned int *				oTargetLen) const
{
	LogFunctionEnterExit;
	
	z_stream d_stream;	//decompression stream
	memset(&d_stream, 0, sizeof(d_stream));
	
	int ret = 0;
	int err = Z_OK;
	int alloclen = max(inSourceLen * 2, INCREASE);
	
	d_stream.zalloc = NULL;
	d_stream.zfree = NULL;
	d_stream.opaque = NULL;
	
	*oTargetBuff = (unsigned char *) malloc(alloclen+1);
	*oTargetLen = 0;
	
	err = inflateInit2(&d_stream, 15 + 16);
	if ( err == Z_OK)
	{
		d_stream.next_in  = (Bytef*)inSourceBuff;
		d_stream.avail_in = inSourceLen;
		d_stream.next_out = *oTargetBuff;
		d_stream.avail_out = alloclen;

		while (d_stream.total_in < inSourceLen )//&& d_stream.total_out < alloclen)
		{
			err = inflate(&d_stream, Z_NO_FLUSH);
			if( err == Z_STREAM_END )	//All data is uncompressed
				break;
			else if( err == Z_DATA_ERROR )	//Input is wrong, get out of here
				break;
			// CHECK_ERR(err, "deflate");
			if ( err == Z_BUF_ERROR || d_stream.avail_out == 0)
			{
				// Alloc new memory
				int now = alloclen;
				alloclen += alloclen / 10 + INCREASE;
				*oTargetBuff = (unsigned char *) realloc(*oTargetBuff, alloclen+1);
				d_stream.next_out = *oTargetBuff + now;
				d_stream.avail_out = alloclen - now;
			}
			else	//Can't handle it here.
				break;
		}
		// Finish the stream
		for (;;)
		{
			err = inflate(&d_stream, Z_FINISH);
			if (err == Z_STREAM_END) break;
			if (d_stream.avail_out == 0)
			{
				// alloc new memory
				int now = alloclen;
				alloclen += alloclen / 10 + INCREASE;
				*oTargetBuff = (unsigned char *) realloc(*oTargetBuff, alloclen+1);
				d_stream.next_out = *oTargetBuff + now;
				d_stream.avail_out = alloclen - now;
			}
			// CHECK_ERR(err, "deflate");
		}
		
		err = inflateEnd(&d_stream);
		// CHECK_ERR(err, "deflateEnd");
	}
	*oTargetLen = d_stream.total_out;
	// Free remaining memory
	*oTargetBuff = (unsigned char *) realloc(*oTargetBuff, *oTargetLen);
	
	return 0;
}