//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CActionStateList.cpp $
//	$Revision: 3448 $
//	$Date: 2011-07-05 15:38:17 +0200 (Tue, 05 Jul 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 17-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "CActionStateList.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

CActionStateList::CActionStateList()
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CActionStateList::~CActionStateList()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Length
//----------------------------------------------------------------------------------------
int32
CActionStateList::Length()const
{
	return mActionList.size();
}

//----------------------------------------------------------------------------------------
// GetNthAction
//----------------------------------------------------------------------------------------
ActionID
CActionStateList::GetNthAction(
	int32 index) const
{
	if (index < 0 || index > mActionList.size())
		return 0;

	return mActionList[index].mActionId;
}

//----------------------------------------------------------------------------------------
// SetNthActionState
//----------------------------------------------------------------------------------------
void
CActionStateList::SetNthActionState(
	int32 index, int16 state)
{
	if (index < 0 || index > mActionList.size())
		return;

	mActionList[index].mActionState = state;
}

//----------------------------------------------------------------------------------------
// GetNthActionState
//----------------------------------------------------------------------------------------
int16
CActionStateList::GetNthActionState(
	int32 index) const
{
	if (index < 0 || index > mActionList.size())
		return 0;

	return mActionList[index].mActionState;
}

//----------------------------------------------------------------------------------------
// SetNthActionName
//----------------------------------------------------------------------------------------
void
CActionStateList::SetNthActionName(
	int32 index, const PMString& name)
{
	if (index < 0 || index > mActionList.size())
		return;

	mActionList[index].mActionName = name;
}

//----------------------------------------------------------------------------------------
// GetNthActionName
//----------------------------------------------------------------------------------------
const PMString&
CActionStateList::GetNthActionName(
	int32 index) const
{
	if (index < 0 || index > mActionList.size())
		return kNullString;

	return mActionList[index].mActionName;
}

//----------------------------------------------------------------------------------------
// AddAction
//----------------------------------------------------------------------------------------
void
CActionStateList::AddAction(
	const ActionID &		inActionId,
	const PMString &		inActionName,
	const int16 &			inActionState)
{
	stActionState actionState;
	actionState.mActionId = inActionId;
	actionState.mActionName = inActionName;
	actionState.mActionState = inActionState;

	mActionList.push_back(actionState);
}
