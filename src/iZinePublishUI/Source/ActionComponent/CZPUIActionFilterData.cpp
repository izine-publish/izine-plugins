//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPUIActionFilterData.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IWorkspace.h"
#include "IPMStream.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "AssignmentUIID.h"
#include "PersistUIDList.h"
#include "k2VectorStreaming.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPUIActionFilterData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUIActionFilterData>
class CZPUIActionFilterData : public inheritClass
{
public:
						CZPUIActionFilterData(IPMUnknown * boss);
	virtual				~CZPUIActionFilterData();

	void				ClearList();
	ClassID				GetOldHandlerForAction(
							ActionID					inActionID ) const;
	void				SetOldHandlerForAction(
							ActionID					inActionID,
							ClassID						inHandler );
	void				ReadWrite(
							IPMStream *					s,
							ImplementationID			prop);
protected:
	static	IDataBase *	GetAppWorkspaceDB();
private:
	K2Vector<ActionID>	mActionIDs;					//Should we use K2Vector<ActionID>?
	K2Vector<ClassID>	mOldHandlerClassIDs;		//Class IDs of previous handler of nth action id in mActionIDs.
};


CREATE_PERSIST_PMINTERFACE(CZPUIActionFilterData, kZPUIActionFilterDataImpl)

CZPUIActionFilterData::CZPUIActionFilterData(IPMUnknown * boss)
: inheritClass(boss)
, mActionIDs( )
, mOldHandlerClassIDs( )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIActionFilterData::~CZPUIActionFilterData()
{

}
#pragma mark -
//----------------------------------------------------------------------------------------
// ClearList
//----------------------------------------------------------------------------------------
void
CZPUIActionFilterData::ClearList()
{
	PreDirty();
	mActionIDs.clear();
	mOldHandlerClassIDs.clear();
}

//----------------------------------------------------------------------------------------
// GetOldHandlerForAction
//----------------------------------------------------------------------------------------
ClassID
CZPUIActionFilterData::GetOldHandlerForAction(
	ActionID					inActionID) const
{

	int actionIndex = mActionIDs.Location( inActionID );
	if( actionIndex != -1 )
	{
		return mOldHandlerClassIDs.at( actionIndex );
	}
	else
	{	ASSERT( actionIndex != -1 );
		switch( inActionID.Get() )
		{
#if 0	//InCopy logs
0x00000104, OldHandler = 0x00017202	//kSaveActionID			//kInCopyFileActionsComponentBoss, this boss has different value in object model dump and that in header file.
0x00000105, OldHandler = 0x00017202	//kSaveAsActionID
0x00018F07, OldHandler = 0x00018F01	//kICBridgeSubmitSelActionID
0x00018F0C, OldHandler = 0x00018F01	//kICBridgeSubmitAllActionID
0x00018F0F, OldHandler = 0x00018F01	//kICBridgeSLSubmitActionID
0x00018F10, OldHandler = 0x00018F01	//kICBridgeSLSubmitAllActionID
0x00019AF1, OldHandler = 0x00019A01	//kForwardPackagedAssignmentActionID
0x00019AF2, OldHandler = 0x00019A01	//kForwardAndEmailPackagedAssignmentActionID
0x00019AF3, OldHandler = 0x00019A01	//kReturnPackagedAssignmentActionID
0x00019AF4, OldHandler = 0x00019A01	//kReturnAndEmailPackagedAssignmentActionID
0x00019AF5, OldHandler = 0x00019A01	//kOpenPackagedAssignmentActionID
0x00019A04, OldHandler = 0x00019A01	//kSLAssignmentOptionsActionID
#endif
		case 0x00018F07:	//kICBridgeSubmitSelActionID
		case 0x00018F0C:	//kICBridgeSubmitAllActionID
		case 0x00018F0F:	//kICBridgeSLSubmitActionID
		case 0x00018F10:	//kICBridgeSLSubmitAllActionID
			return 0x18F01;			//kICBridgeMenuBoss
		case 0x00019A04:	//kSLAssignmentOptionsActionID
			return 0x19A01;			//kStoryListPanelMenuBoss
		case 0x00019A15:	//kSLAddToNewAssignmentActionID
		case 0x00019A1D:	//kSLAddToAssignmentDynamicActionID
			return 0x19A17;			//kAddToAssignmentMenuBoss
		case 0x00019A0D:	//kSLRelinkAssignmentActionID
		case 0x00019AED:	//kPackageAssignmentActionID
		case 0x00019AF0:	//kPackageAndEmailAssignmentActionID
		case 0x00019AF5:	//kOpenPackagedAssignmentActionID
		case 0x00019AF7:	//kCancelPackagedAssignmentActionID
		case 0x00019AF1:	//kForwardPackagedAssignmentActionID
		case 0x00019AF2:	//kForwardAndEmailPackagedAssignmentActionID
		case 0x00019AF3:	//kReturnPackagedAssignmentActionID
		case 0x00019AF4:	//kReturnAndEmailPackagedAssignmentActionID
			return 0x19A01;			//kStoryListPanelMenuBoss
		case 0x00019A17:	//kSLAddSelToNewAssignmentActionID
		case 0x00019A51:	//kSLAddSelToAssignmentDynamicActionID
			return 0x19A18;			//kAddSelToAssignmentMenuBoss
		case 0x00019A1C:	//kSLAddSelToNewAssignmentLayoutContextActionID
		case 0x00019AD3:	//kSLAddSelToAssignmentLayoutContextDynamicActionID
			return 0x19A1D;			//kAddSelToAssignmentLayoutContextMenuBoss
		case 0x00019A18:	//kSLAddLayerToNewAssignmentActionID
		case 0x00019A6B:	//kSLAddLayerToAssignmentDynamicActionID
			return 0x19A19;			//kAddLayerToAssignmentMenuBoss
		case 0x00019A19:	//kSLAddAllStoriesToNewAssignmentActionID
		case 0x00019A85:	//kSLAddAllStoriesToAssignmentDynamicActionID
			return 0x19A1A;			//kAddAllStoriesToAssignmentMenuBoss
		case 0x00000104:	//kSaveActionID
		case 0x00000105:	//kSaveAsActionID
			return 0xE03;			//kDocumentComponentBoss
			//In InCopy it should be 0x00017202
		default:
			if( inActionID >=	kSLAddLayerToAssignmentDynamicActionID &&	inActionID	<=	kSLAddLayerToAssignmentDynamicEndActionID )
				return 0x19A19;			//kAddLayerToAssignmentMenuBoss
			else if( inActionID	>=	kSLAddAllStoriesToAssignmentDynamicActionID &&	inActionID	<=	kSLAddAllStoriesToAssignmentDynamicEndActionID )
				return 0x19A1A;			//kAddAllStoriesToAssignmentMenuBoss
			else if( inActionID	>=	kSLAddSelToAssignmentDynamicActionID &&		inActionID	<=	kSLAddSelToAssignmentDynamicEndActionID )
				return 0x19A18;			//kAddSelToAssignmentMenuBoss
			else if( inActionID	>=	kSLAddSelToAssignmentLayoutContextDynamicActionID &&	inActionID	<=	kSLAddSelToAssignmentLayoutContextDynamicEndActionID )
				return 0x19A1D;			//kAddSelToAssignmentLayoutContextMenuBoss
			else if( inActionID	>=	kSLAddToAssignmentDynamicActionID &&	inActionID	<=	kSLAddToAssignmentDynamicEndActionID )
				return 0x19A17;			//kAddToAssignmentMenuBoss
		}
	}
	return kInvalidClass;
}

//----------------------------------------------------------------------------------------
// SetOldHandlerForAction
//----------------------------------------------------------------------------------------
void
CZPUIActionFilterData::SetOldHandlerForAction(
	ActionID					inActionID,
	ClassID						inHandler)
{
	int actionIndex = mActionIDs.Location( inActionID );
	if( actionIndex != -1 )
	{
		if( mOldHandlerClassIDs[ actionIndex ] != inHandler )
		{
			PreDirty();
			mOldHandlerClassIDs[ actionIndex ] = inHandler;
		}
	}
	else
	{
		PreDirty();
		mActionIDs.push_back( inActionID );
		mOldHandlerClassIDs.push_back( inHandler );
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPUIActionFilterData::ReadWrite(
	IPMStream *					s,
	ImplementationID			prop)
{
#if 0
	int32 arrayLen = mActionIDs.size();
	s->XferInt32( arrayLen );
	mActionIDs.reserve( arrayLen );
	mOldHandlerClassIDs.reserve( arrayLen );

	s->XferID( mActionIDs.begin(), arrayLen );
	s->XferID( mOldHandlerClassIDs.begin(), arrayLen );
#else
	if (s->IsReading())
	{
		K2Read(s, mActionIDs);
		K2Read(s, mOldHandlerClassIDs);
	}
	else
	{
		K2Write(s, mActionIDs);
		K2Write(s, mOldHandlerClassIDs);
	}
#endif
	//mActionIDs.ReadWriteObject( s, prop );
}

//----------------------------------------------------------------------------------------
// GetAppWorkspaceDB
//----------------------------------------------------------------------------------------
IDataBase *
CZPUIActionFilterData::GetAppWorkspaceDB()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	return ::GetDataBase( iWorkspace );
}
