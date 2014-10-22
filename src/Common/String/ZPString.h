//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/String/ZPString.h $
//	$Revision: 3998 $
//	$Date: 2012-04-02 13:17:17 +0200 (Mon, 02 Apr 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Light weight string class, for performance 
//========================================================================================
#ifndef _h_ZPString_
#define _h_ZPString_

#pragma once

template <const int charSize>
class ZPString_
{
public:
						ZPString_(
							const char *		inValStr = 0,
							int					inLen = -1);
		virtual			~ZPString_();
						ZPString_(
							const ZPString_ &	inStr );
		ZPString_ &		operator = (
							const ZPString_ &	inStr );


		const char *	operator()() const;
		void			SetStr(
							const char *		inStr,
							int					inLen  = -1);

		bool			operator ==(
							const ZPString_ &	inStr );

		void			Reserve(
							int					inSize );
protected:
private:

	int			mSize;
	char		mVal[charSize];
	char *		mLongVal;
	int			mCapacity;
};

typedef ZPString_<38>	ZPString38;		//For string IDs.36+1+1(padding)
typedef ZPString_<64>	ZPString64;		//Used with user name, pwd, etc.
typedef ZPString_<256>	ZPString256;
typedef ZPString_<512>	ZPString512;


#endif //_h_ZPString_
