//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpGetConfigRequest.h $
//	$Revision: 3677 $
//	$Date: 2011-11-28 11:55:00 +0100 (Mon, 28 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 28-11-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpGetConfigRequest_
#define _h_CZExpGetConfigRequest_
#pragma once

#include "AZExpWSRequest.h"
class AZExpWSResponse;

class CZExpGetConfigRequest : public AZExpWSRequest
{
public:
						CZExpGetConfigRequest();
	virtual				~CZExpGetConfigRequest();

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZExpWSResponse *	CreateResponseObj();
private:
};

#endif //_h_CZExpGetConfigRequest_
