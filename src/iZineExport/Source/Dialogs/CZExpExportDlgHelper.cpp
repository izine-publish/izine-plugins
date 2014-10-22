//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Dialogs/CZExpExportDlgHelper.cpp $
//	$Revision: 3939 $
//	$Date: 2012-02-22 10:31:14 +0100 (Wed, 22 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 15-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDropDownListController.h"
#include "IPanelControlData.h"
#include "IStringListControlData.h"
#include "IWorkspace.h"

#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpExportDlgHelper.h"
#include "IZExpGlobalData.h"
#include "IZExpPrefs.h"
#include "IZExpServerData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpExportDlgHelper>
class CZExpExportDlgHelper : public inheritClass
{
public:
						CZExpExportDlgHelper(IPMUnknown * boss);
	virtual				~CZExpExportDlgHelper();

	void					FillTitles() const;
	void					FillEditions() const;
	const PMString &		GetTitleId() const;
	const PMString &		GetEditionId() const;

	void					GetConfig() const;

protected:
private:
	void					ClearTitles() const;
};


CREATE_PMINTERFACE(CZExpExportDlgHelper, kZExpExportDlgHelperImpl)

CZExpExportDlgHelper::CZExpExportDlgHelper(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpExportDlgHelper::~CZExpExportDlgHelper()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FillTitles
//----------------------------------------------------------------------------------------
void
CZExpExportDlgHelper::FillTitles()const
{
#ifdef InDnCS5
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif

	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);

	InterfacePtr<const IZExpPrefs> iZExpPrefs( theWorkspace, UseDefaultIID() );
	ASSERT(iZExpPrefs);
	const PMString currentPluginId = iZExpPrefs->GetPluginId();

	InterfacePtr<IZExpGlobalData> gGlobalData( gSession, UseDefaultIID() );
	InterfacePtr<IZExpServerData> serverData( this, UseDefaultIID() );
	const ZExpIDPairList & titleList = serverData->GetTitles();
	if (titleList.size() == 0)
	{
		serverData->FetchTitles();
		gGlobalData->SetLastPluginId(currentPluginId);
	}
	else
	{
		const PMString lastPluginId = gGlobalData->GetLastPluginId();
		if(lastPluginId.IsEmpty() || lastPluginId.IsEqual(currentPluginId, kFalse) == kFalse)
		{
			this->ClearTitles();
			serverData->FetchTitles();
			gGlobalData->SetLastPluginId(currentPluginId);
		}
	}

	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);
	if (selfPanelControlData)
	{
		IControlView* widgetView = selfPanelControlData->FindWidget( kZExpExportEditionDropDownWidgetID );
		ASSERT(widgetView);
		InterfacePtr<IStringListControlData> stringListControlDataEditions(widgetView, UseDefaultIID());
		ASSERT(stringListControlDataEditions);
		if (stringListControlDataEditions)
			stringListControlDataEditions->Clear(kFalse, kFalse);		//This will clear edition list

		widgetView = selfPanelControlData->FindWidget( kZExpExportTitleDropDownWidgetID );
		ASSERT(widgetView);

		InterfacePtr<IStringListControlData> stringListControlDataTitles(widgetView, UseDefaultIID());
		ASSERT(stringListControlDataTitles);
		if (stringListControlDataTitles)
		{
			int totalCount = titleList.size();
			if (totalCount == 0)
				stringListControlDataTitles->Clear();
			else
				stringListControlDataTitles->Clear(kFalse, kFalse);

			int count = 0;
			ZExpIDPairList::const_iterator iter;
			for (iter = titleList.begin(); iter < titleList.end(); ++iter) 
			{
				count++;
				ZExpIDPair obj = *iter;
				if (count == totalCount)
					stringListControlDataTitles->AddString(obj.mName.SetTranslatable(kFalse));
				else
					stringListControlDataTitles->AddString(obj.mName.SetTranslatable(kFalse), IStringListControlData::kEnd,kFalse, kFalse);
			}
		}
	}
}

//----------------------------------------------------------------------------------------
// FillEditions
//----------------------------------------------------------------------------------------
void
CZExpExportDlgHelper::FillEditions()const
{
	do
	{
		InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
		ASSERT(selfPanelControlData);
		if (!selfPanelControlData)
			break;

		IControlView* widgetView = selfPanelControlData->FindWidget( kZExpExportEditionDropDownWidgetID );
		ASSERT(widgetView);
		if (!widgetView)
			break;

		InterfacePtr<IStringListControlData> stringListControlDataEditions(widgetView, UseDefaultIID());
		ASSERT(stringListControlDataEditions);
		if (stringListControlDataEditions)
			stringListControlDataEditions->Clear(kFalse, kFalse);

		const PMString & titleId = this->GetTitleId();
		if( titleId.empty())
			break;

		InterfacePtr<IZExpServerData> serverData( this, IID_IZEXPSERVERDATA );
		serverData->FetchEditions(titleId);

		const ZExpIDPairList & editionsList = serverData->GetEditions();
		ZExpIDPairList::const_iterator iter;
		for (iter = editionsList.begin(); iter < editionsList.end(); ++iter) 
		{
			ZExpIDPair obj = *iter;
			stringListControlDataEditions->AddString(obj.mName.SetTranslatable(kFalse), IStringListControlData::kEnd,kFalse,kFalse);
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// GetTitleId
//----------------------------------------------------------------------------------------
const PMString &
CZExpExportDlgHelper::GetTitleId()const
{
	do
	{
		InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
		ASSERT(selfPanelControlData);
		if (!selfPanelControlData)
			break;

		IControlView* widgetView = selfPanelControlData->FindWidget( kZExpExportTitleDropDownWidgetID );
		ASSERT(widgetView);
		if (!widgetView)
			break;

		InterfacePtr<const IDropDownListController> dropDown(widgetView, UseDefaultIID());
		ASSERT(dropDown);
		if (!dropDown)
			break;
		
		int32 index = dropDown->GetSelected();

		InterfacePtr<const IZExpServerData> serverData( this, UseDefaultIID());
		const ZExpIDPairList & titleList = serverData->GetTitles();
		if (index < 0 || index >= titleList.size())
			break;

		return serverData->GetTitleIDByIndex(index);
	}while(false);

	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetEditionId
//----------------------------------------------------------------------------------------
const PMString &
CZExpExportDlgHelper::GetEditionId()const
{
	do
	{
		InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
		ASSERT(selfPanelControlData);
		if (!selfPanelControlData)
			break;

		IControlView* widgetView = selfPanelControlData->FindWidget( kZExpExportEditionDropDownWidgetID );
		ASSERT(widgetView);
		if (!widgetView)
			break;

		InterfacePtr<const IDropDownListController> dropDown(widgetView, UseDefaultIID());
		ASSERT(dropDown);
		if (!dropDown)
			break;
		
		int32 index = dropDown->GetSelected();

		InterfacePtr<const IZExpServerData> serverData( this, UseDefaultIID());
		const ZExpIDPairList & editionList = serverData->GetEditions();
		if (index < 0 || index >= editionList.size())
			break;

		return serverData->GetEditionIDByIndex(index);
	}while(false);

	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetConfig
//----------------------------------------------------------------------------------------
void
CZExpExportDlgHelper::GetConfig()const
{
	InterfacePtr<IZExpServerData> serverData( this, UseDefaultIID());
	serverData->GetConfig();
}

//----------------------------------------------------------------------------------------
// ClearTitles
//----------------------------------------------------------------------------------------
void
CZExpExportDlgHelper::ClearTitles()const
{
	do
	{
		InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
		ASSERT(selfPanelControlData);
		if (!selfPanelControlData)
			break;

		IControlView* widgetView = selfPanelControlData->FindWidget( kZExpExportTitleDropDownWidgetID );
		ASSERT(widgetView);
		if (!widgetView)
			break;

		InterfacePtr<IStringListControlData> stringListControlDataTitles(widgetView, UseDefaultIID());
		ASSERT(stringListControlDataTitles);
		if (stringListControlDataTitles)
			stringListControlDataTitles->Clear(kFalse, kFalse);		//This will clear title list

	}while(kFalse);
}