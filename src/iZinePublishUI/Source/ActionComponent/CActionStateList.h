//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CActionStateList.h $
//	$Revision: 3283 $
//	$Date: 2011-06-17 12:56:59 +0200 (Fri, 17 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 17-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CActionStateList_
#define _h_CActionStateList_
#pragma once

#include "IActionStateList.h"
#include "K2Vector.tpp"

#define inheritClass_CActionStateList IActionStateList
class CActionStateList : public inheritClass_CActionStateList
{
public:
						CActionStateList();
	virtual				~CActionStateList();

	int32 Length() const;
	
	/**  Get the nth action from this list
		@param index which action
		@return nth ActionID
	*/		
	ActionID GetNthAction(int32 index) const;

	/**  Set the enable/selected state for the nth action in this list. Enable state for all actions defaults to kDisabled_Unselected
		@param index which action
		@param state new state for this action. Some combination of kDisabled_Unselected,kEnabledAction,kSelectedAction,kMultiSelectedAction,kAltSelectedAction
	*/		
	void SetNthActionState(int32 index, int16 state);
	/**  Get the enable/selected state for the nth action in this list
		@param index which action
		@return state for this action. Some combination of kDisabled_Unselected,kEnabledAction,kSelectedAction,kMultiSelectedAction,kAltSelectedAction
	*/		
	int16 GetNthActionState(int32 index) const;

	/**  Set the name for the nth action in this list. This only needs to be done if the name of the action is changing.
		@param index which action
		@param name new name for this action. Typically used to dynamically change the name of a menu item that does different things based on some state.
	*/		
	void SetNthActionName(int32 index, const PMString& name);
	/**  Get the name for the nth action in this list
		@param index which action
		@return name for this action. 
	*/		
	const PMString& GetNthActionName(int32 index) const;

	void				AddAction(
									const ActionID &		inActionId,
									const PMString &		inActionName,
									const int16 &			inActionState);
protected:
private:
	struct stActionState
	{
		typedef  object_type   data_type;	//To support K2Vector

		ActionID		mActionId;
		PMString		mActionName;
		int16			mActionState;
	};

	K2Vector<stActionState> mActionList;
};
#endif //_h_CActionStateList_
