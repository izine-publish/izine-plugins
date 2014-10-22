//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Utils/CZMSAXAdContentHandler.cpp $
//	$Revision: 2682 $
//	$Date: 2011-04-05 11:58:29 +0200 (Tue, 05 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 31-3-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"
#include "CSAXContentHandler.h"
#include "ISAXAttributes.h"
#include "ErrorUtils.h"
#include "ISAXServices.h"
#include "IXMLServicesError.h"
#include "IDataStringUtils.h"

//SDK Interfaces

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces

//IZP General includes
#include "ZMTypes.h"
#include "ZMConstants.h"
#include "IZMAdFileData.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ALL
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CSAXContentHandler
class CZMSAXAdContentHandler : public inheritClass
{
public:
						CZMSAXAdContentHandler(IPMUnknown * boss);
	virtual				~CZMSAXAdContentHandler();

	virtual void  StartDocument (ISAXServices *saxServices);
	virtual void  StartElement (const WideString &uri, const WideString &localname, const WideString &qname, ISAXAttributes *attrs);
	virtual void  EndElement (const WideString &uri, const WideString &localname, const WideString &qname) ;
	virtual void  Characters (const WideString &chars);

protected:
private:
	//Flags are used for checking valid xml tags
	bool					mIsTagClassWizardDone;
	bool					mIsProcessingTagAds;
	bool					mIsProcessingTagAd;

	ISAXServices *			mSaxServices;
	ZMstAdSList				mAdsList;

	//Used in finding tags
	WideString				mWStr_ClassWizardImposition;
	WideString				mWStr_Ads;
	WideString				mWStr_Ad;

	virtual void				fillAd(
									ISAXAttributes &					attrs,
									stAd &								oAd);
};


CREATE_PMINTERFACE(CZMSAXAdContentHandler, kZMSAXAdContentHandlerImpl)

CZMSAXAdContentHandler::CZMSAXAdContentHandler(IPMUnknown * boss)
: inheritClass(boss)
{
	mIsTagClassWizardDone = false;
	mIsProcessingTagAds = false;
	mIsProcessingTagAd = false;

	//Tags
	mWStr_ClassWizardImposition = WideString( STRXMLTag_ClassWizardImposition );
	mWStr_Ads = WideString( STRXMLTag_Ads );
	mWStr_Ad = WideString( STRXMLTag_Ad );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMSAXAdContentHandler::~CZMSAXAdContentHandler()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// StartDocument
//----------------------------------------------------------------------------------------
void
CZMSAXAdContentHandler::StartDocument (
	ISAXServices *saxServices)
{
	mSaxServices = saxServices;
	inheritClass::StartDocument(saxServices);
}

//----------------------------------------------------------------------------------------
// StartElement
//----------------------------------------------------------------------------------------
void
CZMSAXAdContentHandler::StartElement (
	const WideString &			uri,
	const WideString &			localname,
	const WideString &			qname,
	ISAXAttributes *			attrs)
{
	if (localname == mWStr_Ad)
	{
		if (mIsTagClassWizardDone == true && mIsProcessingTagAds == true && mIsProcessingTagAd == false)
		{
			mIsProcessingTagAd = true;

			stAd * ad = new stAd();
			this->fillAd(*attrs, *ad);

			if (ad->mHeight > 0 && ad->mWidth > 0)		//Only add to list when height and width is greater then 0
				mAdsList.push_back(ad);
			else
				delete ad;
		}
		else
		{
			if (mSaxServices)
			{
				InterfacePtr<IXMLServicesError> serviceErr (mSaxServices, UseDefaultIID());
				serviceErr->SetErrorCode (kIZMInvalidXMLFileError);
			}
		}
	}
	else if (localname == mWStr_Ads)
	{
		if (mIsTagClassWizardDone == true && mIsProcessingTagAds == false && mIsProcessingTagAd == false)
			mIsProcessingTagAds = true;
		else
		{
			if (mSaxServices)
			{
				InterfacePtr<IXMLServicesError> serviceErr (mSaxServices, UseDefaultIID());
				serviceErr->SetErrorCode (kIZMInvalidXMLFileError);
			}
		}
	}
	else if (localname == mWStr_ClassWizardImposition)
	{
		mAdsList.DeleteData();
		if (mIsTagClassWizardDone == false && mIsProcessingTagAds == false && mIsProcessingTagAd == false)
			mIsTagClassWizardDone = true;
		else
		{
			if (mSaxServices)
			{
				InterfacePtr<IXMLServicesError> serviceErr (mSaxServices, UseDefaultIID());
				serviceErr->SetErrorCode (kIZMInvalidXMLFileError);
			}
		}
	}
}

//----------------------------------------------------------------------------------------
// EndElement
//----------------------------------------------------------------------------------------
void
CZMSAXAdContentHandler::EndElement (
	const WideString &			uri,
	const WideString &			localname,
	const WideString &			qname)
{
	if (localname == mWStr_Ad)
	{
		mIsProcessingTagAd = false;
	}
	else if (localname == mWStr_Ads)
	{
		mIsProcessingTagAds = false;
	}
	else if (localname == mWStr_ClassWizardImposition)
	{
		InterfacePtr<IZMAdFileData> zmAdList(this, IZMAdFileData::kDefaultIID);
		if (zmAdList)
		{
			ZMstAdSList & adsList= zmAdList->GetAdsList();
			adsList.Copy(mAdsList, true);
		}
	}
}

//----------------------------------------------------------------------------------------
// Characters
// Gives tag content
//----------------------------------------------------------------------------------------
void
CZMSAXAdContentHandler::Characters (
	const WideString &chars)
{
	if (mIsProcessingTagAd)
	{
		//If want to get tag text then use this function
	}
}

//----------------------------------------------------------------------------------------
// fillAd
//----------------------------------------------------------------------------------------
void
CZMSAXAdContentHandler::fillAd(
	ISAXAttributes &				attrs,
	stAd &							oAd)
{
	Utils<IDataStringUtils> stringUtil;

	WideString id;
	attrs.GetValue(WideString(STRXMLAttr_ID), id);				//For field id
	oAd.mId = id;

	WideString refkey;
	attrs.GetValue(WideString(STRXMLAttr_RefKey), refkey);		//For field refkey
	oAd.mRefKey = refkey;

	WideString customer;
	attrs.GetValue(WideString(STRXMLAttr_Customer), customer);	//For field customer
	oAd.mCustomer = customer;

	WideString height;
	attrs.GetValue(WideString(STRXMLAttr_Height), height);		//For field height
	PMReal heightR = stringUtil->StringToPMReal(height, 4);
	oAd.mHeight = heightR / 1000;	//Value in xml is multiplied by 1000 avoid decimal

	WideString width;
	attrs.GetValue(WideString(STRXMLAttr_Width), width);		//For field width
	PMReal widthR = stringUtil->StringToPMReal(width, 4);
	oAd.mWidth = widthR / 1000;

	WideString url;
	attrs.GetValue(WideString(STRXMLAttr_URL), url);			//For field url
	oAd.mUrl = url;

	WideString purl;
	attrs.GetValue(WideString(STRXMLAttr_PURL), purl);			//For field p-url
	oAd.mPUrl = purl;

	WideString comment;
	attrs.GetValue(WideString(STRXMLAttr_Comment), comment);	//For field comment
	oAd.mComment = comment;
}

