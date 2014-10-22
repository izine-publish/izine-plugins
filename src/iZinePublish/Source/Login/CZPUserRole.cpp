//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Login/CZPUserRole.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPUserRole.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUserRole>
class CZPUserRole : public inheritClass
{
public:
						CZPUserRole(IPMUnknown * boss);
	virtual				~CZPUserRole();

	PMString	GetRoleStr() const;
	static PMString		GetRoleStr(
							enZPUserRole				inUserRole );
	enZPUserRole	GetRole() const;

	void		SetRole(
							enZPUserRole				inUserRole );
	void		SetRole(
							const PMString &			inUserRoleStr );

protected:
private:
	enZPUserRole		mRole;
};


CREATE_PMINTERFACE(CZPUserRole, kZPUserRoleImpl)

CZPUserRole::CZPUserRole(IPMUnknown * boss)
: inheritClass(boss)
, mRole( eUserRole_Default )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUserRole::~CZPUserRole()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetRoleStr
//----------------------------------------------------------------------------------------
PMString
IZPUserRole::GetRoleStr(
	enZPUserRole				inUserRole)
{
	const char * roleStr = nil;
	switch( inUserRole )
	{
	case eUserRole_User:
		roleStr = STR_RoleUser;	break;
	case eUserRole_Admin:
		roleStr = STR_RoleAdmin;	break;
	case eUserRole_KeyUser:
		roleStr = STR_RoleKeyUser;	break;
	default:
		return GetRoleStr( eUserRole_Default );
	}
	
#if defined(InDnCS5) || defined(InDnCS5_5)
	PMString toReturn( roleStr , PMString::kDontTranslateDuringCall );
#else
	PMString toReturn( roleStr , PMString::kEncodingASCII, PMString::kNoTranslate );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetRoleStr
//----------------------------------------------------------------------------------------
enZPUserRole
IZPUserRole::GetRole(
	const PMString &			inUserRoleStr)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
	if( inUserRoleStr.Compare(kFalse, PMString( STR_RoleUser , PMString::kDontTranslateDuringCall ))== 0 )
		return eUserRole_User;
	else if( inUserRoleStr.Compare(kFalse, PMString( STR_RoleAdmin , PMString::kDontTranslateDuringCall ))== 0 )
		return eUserRole_Admin;
	else if( inUserRoleStr.Compare(kFalse, PMString( STR_RoleKeyUser , PMString::kDontTranslateDuringCall ))== 0 )
		return eUserRole_KeyUser;
	return eUserRole_Default;
#else
	if( inUserRoleStr.Compare(kFalse, PMString( STR_RoleUser , PMString::kEncodingASCII, PMString::kNoTranslate ))== 0 )
		return eUserRole_User;
	else if( inUserRoleStr.Compare(kFalse, PMString( STR_RoleAdmin , PMString::kEncodingASCII, PMString::kNoTranslate ))== 0 )
		return eUserRole_Admin;
	else if( inUserRoleStr.Compare(kFalse, PMString( STR_RoleKeyUser , PMString::kEncodingASCII, PMString::kNoTranslate ))== 0 )
		return eUserRole_KeyUser;
	return eUserRole_Default;
#endif
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetRoleStr
//----------------------------------------------------------------------------------------
PMString
CZPUserRole::GetRoleStr()const
{
	return IZPUserRole::GetRoleStr( mRole );
}

//----------------------------------------------------------------------------------------
// GetRole
//----------------------------------------------------------------------------------------
enZPUserRole
CZPUserRole::GetRole()const
{
	return mRole;
}

//----------------------------------------------------------------------------------------
// SetRole
//----------------------------------------------------------------------------------------
void
CZPUserRole::SetRole(
	enZPUserRole				inUserRole)
{
	mRole = inUserRole;
	//TODO: do we need to send message for change.
}

//----------------------------------------------------------------------------------------
// SetRole
//----------------------------------------------------------------------------------------
void
CZPUserRole::SetRole(
	const PMString &			inUserRoleStr)
{
	mRole = IZPUserRole::GetRole( inUserRoleStr );
}
