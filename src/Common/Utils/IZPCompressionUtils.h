//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/IZPCompressionUtils.h $
//	$Revision: 1247 $
//	$Date: 2010-05-27 07:33:14 +0200 (Thu, 27 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPCompressionUtils_
#define _h_IZPCompressionUtils_
#pragma once


class IZPCompressionUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPCOMPRESSIONUTILS };

	virtual int			Compress(
							const unsigned char *		inSourceBuff,
							unsigned int				inSourceLen,
							unsigned char **			oTargetBuff,
							unsigned int *				oTargetLen) const = 0;

	virtual int			Decompress(
							const unsigned char *		inSourceBuff,
							unsigned int				inSourceLen,
							unsigned char **			oTargetBuff,
							unsigned int *				oTargetLen) const = 0;
};


#endif //_h_IZPCompressionUtils_
