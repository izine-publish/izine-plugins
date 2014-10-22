//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPTaskInfo.cpp $
//	$Revision: 2400 $
//	$Date: 2010-10-26 14:35:10 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPTaskInfo.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPTaskInfo>
class CZPTaskInfo : public inheritClass
{
public:
						CZPTaskInfo(IPMUnknown * boss);
	virtual				~CZPTaskInfo();

	const ZPTaskInfo &	GetTaskInfo() const;
	void				SetTaskInfo(
							const ZPTaskInfo &			inTaskInfo );

	const PMString &	GetDescription() const;
	void				SetDescription(
							const PMString &			inDescription );

	const PMString &	GetDocumentID() const;
	void				SetDocumentID(
							const PMString &			inDocumentID );

	const PMString &	GetAssignedToUserID() const;
	void				SetAssignedToUserID(
							const PMString &			inAssignedToUserID );

	const PMString &	GetStatusID() const;
	void				SetStatusID(
							const PMString &			inStatusID );

	const PMString &	GetCategoryID() const;
	void				SetCategoryID(
							const PMString &			inCategoryID );

	const PMString &	GetSpread() const;
	void				SetSpread(
							const PMString &			inSpread );

	const PMString &	GetPage() const;
	void				SetPage(
							const PMString &			inPage );

	int					GetCurrentVersion() const;

	void				Copy(
							const IZPTaskInfo *			inSrc );
protected:
	void				BroadcastMessage(
							const ClassID&					inTheChange = kZPTaskInfoChangedMsg,
							void *							inChangedBy = nil) const;
private:
	ZPTaskInfo		mTaskInfo;
	//PMString		mDescription;
	//PMString		mDocumentID;
	//PMString		mAssignedToUserID;
	//PMString		mStatusID;
	//PMString		mCategory;
	//PMString		mSpread;
	//PMString		mPage;
};


CREATE_PMINTERFACE(CZPTaskInfo, kZPTaskInfoImpl)

CZPTaskInfo::CZPTaskInfo(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskInfo::~CZPTaskInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Copy
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::Copy(
	const IZPTaskInfo *			inSrc)
{
	if( !inSrc )
	{
		this->mTaskInfo = ZPTaskInfo();
		return;
	}

	this->mTaskInfo = inSrc->GetTaskInfo();
}

//----------------------------------------------------------------------------------------
// GetTaskInfo
//----------------------------------------------------------------------------------------
const ZPTaskInfo &
CZPTaskInfo::GetTaskInfo()const
{
	return mTaskInfo;
}

//----------------------------------------------------------------------------------------
// SetTaskInfo
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::SetTaskInfo(
	const ZPTaskInfo &			inTaskInfo)
{
	mTaskInfo = inTaskInfo;
	this->BroadcastMessage();
}

//----------------------------------------------------------------------------------------
// GetDescription
//----------------------------------------------------------------------------------------
const PMString &
CZPTaskInfo::GetDescription()const
{
	return mTaskInfo.mDescription;
}

//----------------------------------------------------------------------------------------
// SetDescription
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::SetDescription(
	const PMString &			inDescription)
{
	mTaskInfo.mDescription = inDescription;
	this->BroadcastMessage();
}

//----------------------------------------------------------------------------------------
// GetDocumentID
//----------------------------------------------------------------------------------------
const PMString &
CZPTaskInfo::GetDocumentID()const
{
	return mTaskInfo.mDocumentID;
}

//----------------------------------------------------------------------------------------
// SetDocumentID
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::SetDocumentID(
	const PMString &			inDocumentID)
{
	mTaskInfo.mDocumentID = inDocumentID;
	this->BroadcastMessage();
}

//----------------------------------------------------------------------------------------
// GetAssignedToUserID
//----------------------------------------------------------------------------------------
const PMString &
CZPTaskInfo::GetAssignedToUserID()const
{
	return mTaskInfo.mAssignedToUserID;
}

//----------------------------------------------------------------------------------------
// SetAssignedToUserID
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::SetAssignedToUserID(
	const PMString &			inAssignedToUserID)
{
	mTaskInfo.mAssignedToUserID = inAssignedToUserID;
	this->BroadcastMessage();
}

//----------------------------------------------------------------------------------------
// GetStatusID
//----------------------------------------------------------------------------------------
const PMString &
CZPTaskInfo::GetStatusID()const
{
	return mTaskInfo.mStatusID;
}

//----------------------------------------------------------------------------------------
// SetStatusID
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::SetStatusID(
	const PMString &			inStatusID)
{
	mTaskInfo.mStatusID = inStatusID;
	this->BroadcastMessage();
}

//----------------------------------------------------------------------------------------
// GetCategory
//----------------------------------------------------------------------------------------
const PMString &
CZPTaskInfo::GetCategoryID()const
{
	return mTaskInfo.mCategoryID;
}

//----------------------------------------------------------------------------------------
// SetCategory
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::SetCategoryID(
	const PMString &			inCategoryID )
{
	mTaskInfo.mCategoryID = inCategoryID;
	this->BroadcastMessage();
}

//----------------------------------------------------------------------------------------
// GetSpread
//----------------------------------------------------------------------------------------
const PMString &
CZPTaskInfo::GetSpread()const
{
	return mTaskInfo.mSpread;
}

//----------------------------------------------------------------------------------------
// SetSpread
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::SetSpread(
	const PMString &			inSpread)
{
	mTaskInfo.mSpread = inSpread;
	this->BroadcastMessage();
}

//----------------------------------------------------------------------------------------
// GetPage
//----------------------------------------------------------------------------------------
const PMString &
CZPTaskInfo::GetPage()const
{
	return mTaskInfo.mPage;
}

//----------------------------------------------------------------------------------------
// SetPage
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::SetPage(
	const PMString &			inPage)
{
	mTaskInfo.mPage = inPage;
	this->BroadcastMessage();
}

//----------------------------------------------------------------------------------------
// GetCurrentVersion
//----------------------------------------------------------------------------------------
int
CZPTaskInfo::GetCurrentVersion()const
{
	return mTaskInfo.mCurrVersionNumber;
}
#pragma mark -
//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPTaskInfo::BroadcastMessage(
	const ClassID&					inTheChange,
	void *							inChangedBy ) const
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->Change( inTheChange, IZPTaskInfo::kDefaultIID, inChangedBy );
}
