//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/Disclosure/CZPDisclosureTriAngleOptions.cpp $
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
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPDisclosureTriAngleOptions.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPDisclosureTriAngleOptions>
class CZPDisclosureTriAngleOptions : public inheritClass
{
public:
						CZPDisclosureTriAngleOptions(IPMUnknown * boss);
	virtual				~CZPDisclosureTriAngleOptions();

	const RsrcID		&	GetCollapseRsrcID() const;
	void				SetCollapseRsrcID(
							const RsrcID		&		inCollapseRsrcID );

	const RsrcID		&	GetExpandedRsrcID() const;
	void				SetExpandedRsrcID(
							const RsrcID		&		inExpandedRsrcID );
	
	const PluginID	&	GetCollapseRsrcPluginID() const;
	void				SetCollapseRsrcPluginID(
							const PluginID	&			inCollapseRsrcPluginID );
	
	const PluginID	&	GetExpandedRsrcPluginID() const;
	void				SetExpandedRsrcPluginID(
							const PluginID	&			inExpandedRsrcPluginID );

	void					ReadWrite(IPMStream *s, ImplementationID prop);
protected:
private:
	RsrcID		mCollapseRsrcID;
	RsrcID		mExpandedRsrcID;
	PluginID	mCollapseRsrcPluginID;
	PluginID	mExpandedRsrcPluginID;
};


CREATE_PERSIST_PMINTERFACE(CZPDisclosureTriAngleOptions, kZPDisclosureTriAngleOptionsImpl)

CZPDisclosureTriAngleOptions::CZPDisclosureTriAngleOptions(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDisclosureTriAngleOptions::~CZPDisclosureTriAngleOptions()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetCollapseRsrcID
//----------------------------------------------------------------------------------------
const RsrcID		&
CZPDisclosureTriAngleOptions::GetCollapseRsrcID()const
{
	return mCollapseRsrcID;
}

//----------------------------------------------------------------------------------------
// SetCollapseRsrcID
//----------------------------------------------------------------------------------------
void
CZPDisclosureTriAngleOptions::SetCollapseRsrcID(
	const RsrcID		&		inCollapseRsrcID)
{
	mCollapseRsrcID = inCollapseRsrcID;
}

//----------------------------------------------------------------------------------------
// GetExpandedRsrcID
//----------------------------------------------------------------------------------------
const RsrcID		&
CZPDisclosureTriAngleOptions::GetExpandedRsrcID()const
{
	return mExpandedRsrcID;
}

//----------------------------------------------------------------------------------------
// SetExpandedRsrcID
//----------------------------------------------------------------------------------------
void
CZPDisclosureTriAngleOptions::SetExpandedRsrcID(
	const RsrcID		&		inExpandedRsrcID)
{
	mExpandedRsrcID = inExpandedRsrcID;
}

//----------------------------------------------------------------------------------------
// GetCollapseRsrcPluginID
//----------------------------------------------------------------------------------------
const PluginID	&
CZPDisclosureTriAngleOptions::GetCollapseRsrcPluginID()const
{
	return mCollapseRsrcPluginID;
}

//----------------------------------------------------------------------------------------
// SetCollapseRsrcPluginID
//----------------------------------------------------------------------------------------
void
CZPDisclosureTriAngleOptions::SetCollapseRsrcPluginID(
const PluginID	&			inCollapseRsrcPluginID)
{
	mCollapseRsrcPluginID = inCollapseRsrcPluginID;
}

//----------------------------------------------------------------------------------------
// GetExpandedRsrcPluginID
//----------------------------------------------------------------------------------------
const PluginID	&
CZPDisclosureTriAngleOptions::GetExpandedRsrcPluginID()const
{
	return mExpandedRsrcPluginID;
}

//----------------------------------------------------------------------------------------
// SetExpandedRsrcPluginID
//----------------------------------------------------------------------------------------
void
CZPDisclosureTriAngleOptions::SetExpandedRsrcPluginID(
	const PluginID	&			inExpandedRsrcPluginID)
{
	mExpandedRsrcPluginID = inExpandedRsrcPluginID;
}
#pragma mark -
//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPDisclosureTriAngleOptions::ReadWrite(
	IPMStream *s, ImplementationID prop)
{
	//Collapsed
	s->XferInt32( (int32&)mCollapseRsrcID );
	s->XferID( mCollapseRsrcPluginID );

	//Expanded
	s->XferInt32( (int32&)mExpandedRsrcID );
	s->XferID( mExpandedRsrcPluginID );
}
