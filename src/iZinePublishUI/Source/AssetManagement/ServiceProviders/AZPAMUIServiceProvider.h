//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/ServiceProviders/AZPAMUIServiceProvider.h $
//	$Revision: 1580 $
//	$Date: 2010-06-30 09:42:10 +0200 (Wed, 30 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPAMUIServiceProvider_
#define _h_AZPAMUIServiceProvider_
#pragma once

#define inheritClass_ZPAMUIServiceP CPMUnknown<IAMUIServiceProvider>

class AZPAMUIServiceProvider : public inheritClass_ZPAMUIServiceP
{
public:
	virtual				~AZPAMUIServiceProvider();

#pragma mark -
	virtual void		Enable(
							bool16 b );
	virtual bool16		IsEnabled( void );
	
	//virtual IAMUIService*	GetService(
	//						int32 uiServiceID );
	//virtual bool16		IsServiceAvailable(
	//						UIDRef assetRef, int32 assetServiceID );
	virtual void		ShowAllWarningDialogs();
#pragma mark -

protected:
						AZPAMUIServiceProvider(IPMUnknown * boss);
	virtual const char * GetProviderName() const = 0;

	virtual bool		IsLoggedIn() const;

private:
	bool16		mIsEnabled;
};

#endif //_h_AZPAMUIServiceProvider_
