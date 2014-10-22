//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPCreateTaskRequest.h $
//	$Revision: 2318 $
//	$Date: 2010-10-15 13:35:17 +0200 (Fri, 15 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPCreateTaskRequest_
#define _h_CZPCreateTaskRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPCreateTaskRequest : public AZPSoapRequest
{
public:
						CZPCreateTaskRequest(
							enRequestType				inReqType = eReq_CreateTask);
	virtual				~CZPCreateTaskRequest();

	const ZPTaskInfo &	GetTaskInfo() const;
	
	const PMString &	GetTitleID() const;
	void				SetTitleID(
							const PMString &			inTitleID );

	const PMString &	GetEditionID() const;
	void				SetEditionID(
							const PMString &			inEditionID );

	const PMString &	GetSubject() const;
	void				SetSubject(
							const PMString &			inSubject );

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

	const PMString &	GetCategory() const;
	void				SetCategory(
							const PMString &			inCategory );

	const PMString &	GetSpread() const;
	void				SetSpread(
							const PMString &			inSpread );

	const PMString &	GetPage() const;
	void				SetPage(
							const PMString &			inPage );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:

	PMString		mTitleID;
	PMString		mEditionID;
	PMString		mSubject;

protected:
	ZPTaskInfo		mTaskInfo;
	//PMString		mDescription;
	//PMString		mDocumentID;
	//PMString		mAssignedToUserID;
	//PMString		mStatusID;
	//PMString		mCategory;
	//PMString		mSpread;
	//PMString		mPage;
};

#endif //_h_CZPCreateTaskRequest_
