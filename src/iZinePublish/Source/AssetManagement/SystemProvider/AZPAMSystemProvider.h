//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/SystemProvider/AZPAMSystemProvider.h $
//	$Revision: 1571 $
//	$Date: 2010-06-29 07:41:04 +0200 (Tue, 29 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPAMSystemProvider_
#define _h_AZPAMSystemProvider_
#pragma once

#define inheritClass_ZPAMSystemP CPMUnknown<IAMSystemProvider>
class AZPAMSystemProvider : public inheritClass_ZPAMSystemP
{
public:
	virtual				~AZPAMSystemProvider();

	virtual IAssetMonitor*			GetAssetMonitor();
	virtual IAMServiceProvider*		GetServiceProvider();
	virtual IAMUIServiceProvider*	GetUIServiceProvider();
	virtual ErrorCode				Init();
protected:
						AZPAMSystemProvider(IPMUnknown * boss);
	virtual const char *		GetProviderName() const = 0;
protected:
	bool		mProviderIsAvailable;
private:
};

#endif //_h_AZPAMSystemProvider_
