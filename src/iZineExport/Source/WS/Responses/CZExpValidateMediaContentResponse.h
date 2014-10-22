//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpValidateMediaContentResponse.h $
//	$Revision: 3946 $
//	$Date: 2012-02-24 10:44:05 +0100 (Fri, 24 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 22-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpValidateMediaContentResponse_
#define _h_CZExpValidateMediaContentResponse_
#pragma once

#include "AZExpWSResponse.h"

class CZExpValidateMediaContentResponse : public AZExpWSResponse
{
public:
						CZExpValidateMediaContentResponse();
	virtual				~CZExpValidateMediaContentResponse();

	bool				GetIsItemPresent() const;
	void				SetIsItemPresent(
							const bool					inIsItemPresent );

	bool				GetIsItemAddedToArticle() const;
	void				SetIsItemAddedToArticle(
							const bool					inIsItemAddedToArticle );

protected:
private:
	bool			mIsItemPresent;
	bool			mIsItemAddedToArticle;
};

#endif //_h_CZExpValidateMediaContentResponse_
