//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpValidateMediaContentResponse.cpp $
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
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "CZExpValidateMediaContentResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpValidateMediaContentResponse::CZExpValidateMediaContentResponse()
: AZExpWSResponse( eResp_ValidateMediaContent )
, mIsItemPresent(false)
, mIsItemAddedToArticle(false)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpValidateMediaContentResponse::~CZExpValidateMediaContentResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetIsItemPresent
//----------------------------------------------------------------------------------------
bool
CZExpValidateMediaContentResponse::GetIsItemPresent()const
{
	return mIsItemPresent;
}

//----------------------------------------------------------------------------------------
// SetIsItemPresent
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentResponse::SetIsItemPresent(
	const bool					inIsItemPresent)
{
	mIsItemPresent = inIsItemPresent;
}

//----------------------------------------------------------------------------------------
// GetIsItemAddedToArticle
//----------------------------------------------------------------------------------------
bool
CZExpValidateMediaContentResponse::GetIsItemAddedToArticle()const
{
	return mIsItemAddedToArticle;
}

//----------------------------------------------------------------------------------------
// SetIsItemAddedToArticle
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentResponse::SetIsItemAddedToArticle(
	const bool					inIsItemAddedToArticle)
{
	mIsItemAddedToArticle = inIsItemAddedToArticle;
}
