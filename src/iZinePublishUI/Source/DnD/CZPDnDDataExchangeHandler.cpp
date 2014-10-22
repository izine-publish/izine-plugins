//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/DnD/CZPDnDDataExchangeHandler.cpp $
//	$Revision: 2939 $
//	$Date: 2011-05-05 11:27:24 +0200 (Thu, 05 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"

//SDK General includes
#include "CDataExchangeHandlerFor.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDataExchangeHandlerFor
class CZPDnDDataExchangeHandler : public inheritClass
{
public:
						CZPDnDDataExchangeHandler(IPMUnknown * boss);
	virtual				~CZPDnDDataExchangeHandler();

	/**
			indicates the type of object this data exchange handler can deal with.
			@return the flavor supported by the handler
		*/
		virtual	PMFlavor GetFlavor() const { return kIZPAssetIDDragDropCustomFlavor; }

		/**
			clears the scrap.
		*/
		virtual void				Clear();
		/**
			reports whether the scrap is empty or not.
			@return kTrue if the scrap is empty, kFalse otherwise.
		*/
		virtual bool16			IsEmpty() const;
protected:
private:
};


CREATE_PMINTERFACE(CZPDnDDataExchangeHandler, kZPDnDDataExchangeHandlerImpl)

CZPDnDDataExchangeHandler::CZPDnDDataExchangeHandler(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDnDDataExchangeHandler::~CZPDnDDataExchangeHandler()
{

}

//	clears the scrap.
void 
CZPDnDDataExchangeHandler::Clear()
{
	InterfacePtr<IStringData> assetID(this, IID_IZPDATAID);
	ASSERT(assetID);
	if (assetID)
		assetID->Set(kNullString);
	
}

//	reports whether the asset id is empty or not.
bool16 
CZPDnDDataExchangeHandler::IsEmpty() const
{
	bool16 toReturn = kTrue;
	InterfacePtr<IStringData> assetID(this, IID_IZPDATAID);
	ASSERT(assetID);
	if (assetID)
		toReturn = assetID->Get().IsEmpty();

	return toReturn;
}

#pragma mark -
