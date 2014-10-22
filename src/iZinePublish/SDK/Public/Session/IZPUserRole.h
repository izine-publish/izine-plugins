//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/Session/IZPUserRole.h $
//	$Revision: 1230 $
//	$Date: 2010-05-26 04:10:46 +0200 (Wed, 26 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUserRole_
#define _h_IZPUserRole_
#pragma once

#include "ZPTypes.h"

#define STR_RoleAdmin "0d629cba-f55c-461a-831e-58053db7189f"
#define STR_RoleKeyUser "1313712b-7675-450a-a0d3-c774366dfe45"
#define STR_RoleUser "8b5be9fb-57ac-4c77-b00e-4c366b0e47f5"

class IZPUserRole : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUSERROLE };

	static PMString		GetRoleStr(
							enZPUserRole				inUserRole );
	static enZPUserRole	GetRole(
							const PMString &			inUserRoleStr );
	
	virtual PMString	GetRoleStr() const = 0;
	virtual enZPUserRole	GetRole() const = 0;

	virtual void		SetRole(
							enZPUserRole				inUserRole ) = 0;
	virtual void		SetRole(
							const PMString &			inUserRoleStr ) = 0;
};


#endif //_h_IZPUserRole_
