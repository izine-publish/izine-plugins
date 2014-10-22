//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Controls/Disclosure/IZPDisclosureTriAngleOptions.h $
//	$Revision: 3974 $
//	$Date: 2012-03-06 12:53:00 +0100 (Tue, 06 Mar 2012) $
//	$Author: rajkumar.sehrawat $
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
	enum { kDefaultIID = IID_IZEXPDISCLOSURETRIANGLEOPTIONS };

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
