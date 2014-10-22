//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMData/iZMPrefs.h $
//	$Revision: 2860 $
//	$Date: 2011-04-27 15:07:13 +0200 (Wed, 27 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-9-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPPrefs_
#define _h_IZPPrefs_
#pragma once


class IZMPrefs : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMPREFS };
	
	virtual	const PMString &	GetRepositoryPath( ) const = 0;
	virtual void				SetRepositoryPath(
									const PMString &					inPath ) = 0;

	virtual const PMString &	GetAdFileNamePrefix() const = 0;
	virtual void				SetAdFileNamePrefix(
									const PMString &					inAdFileNamePrefix ) = 0;
};

#endif //_h_IZPPrefs_
