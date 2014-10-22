//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/Disclosure/IZPDisclosureTriAngleOptions.h $
//	$Revision: 2975 $
//	$Date: 2011-05-10 12:18:23 +0200 (Tue, 10 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 10-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPDisclosureTriAngleOptions_
#define _h_IZPDisclosureTriAngleOptions_
#pragma once


class IZPDisclosureTriAngleOptions : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPDISCLOSURETRIANGLEOPTIONS };

	virtual const RsrcID&	GetCollapseRsrcID() const = 0;
	virtual void			SetCollapseRsrcID(
								const RsrcID &				inCollapseRsrcID ) = 0;

	virtual const RsrcID&	GetExpandedRsrcID() const = 0;
	virtual void			SetExpandedRsrcID(
								const RsrcID &				inExpandedRsrcID ) = 0;
	
	virtual const PluginID&	GetCollapseRsrcPluginID() const = 0;
	virtual void			SetCollapseRsrcPluginID(
								const PluginID &			inCollapseRsrcPluginID ) = 0;
	
	virtual const PluginID &GetExpandedRsrcPluginID() const = 0;
	virtual void			SetExpandedRsrcPluginID(
								const PluginID &			inExpandedRsrcPluginID ) = 0;
};


#endif //_h_IZPDisclosureTriAngleOptions_
