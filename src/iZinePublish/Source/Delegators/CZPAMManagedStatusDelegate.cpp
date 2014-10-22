//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Delegators/CZPAMManagedStatusDelegate.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IManagedStatus.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AM
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

static const char * gEditingState[] = {
	"enUnknownEditingState",	//	= 0,
	"enNotEditing",				//	= 1,	// asset is not currently being edited 
	"enEditingLocally",			//	= 2,	// this instance is being edited (edited here) 
	"enEditingRemotely",		//	= 3,	// being edited elsewhere 
	"enEditingConflict"						// editing here and elsewhere at the same time
};

static const char * gVersionState[] = {
		"enUnknownVersionState",	//= 0,	// default state 
		"enUpToDate",				//= 1,	// local cache identical to latest version 
		"enLocalIsNewer",			//= 2,	// local cache has changes not checked in 
		"enLocalIsOutOfDate",		//= 3,	// local cache is older than latest version 
		"enVersionConflict"			//= 4	// local cache has changes while newer version exists 
};

#define inheritClass CPMUnknown<IManagedStatus>
class CZPAMManagedStatusDelegate : public inheritClass
{
public:
						CZPAMManagedStatusDelegate(IPMUnknown * boss);
	virtual				~CZPAMManagedStatusDelegate();

/** This method adds an IAMStatusListener to the list for status notifications.
		@param listenerRef is a UIDRef of a boss with an IAMStatusListener interface.
		@see IAMStatusListener.
	*/						 
	virtual void AddListener( UIDRef listenerRef );
	
	/** Gets the UIDRef of the IManageableAsset boss associated with this status instance.
		@return a valid UIDRef unless not properly set.
		@see IManageableAsset.
	*/
	virtual UIDRef GetAsset();
	
	/** Retrieves the current EditingState for the asset.
		@return IManagedStatus::EditingState.
	*/	
	virtual IManagedStatus::EditingState GetEditingState();
	
	/** Retrieves a display icon ID.
		@param iconType is a predefined WGStatusType or other unique identifier.
		@return a valid RsrcID or 0.
	*/
	virtual RsrcID GetIconID( int32 iconType );

	/** Retrieves the list of IAMStatusListeners currently registered with this instance.
		@param listeners is populated with the UIDRef of the IAMStatusListeners.
		@return the number of UIDRefs put in the list.
		@see IAMStatusListener.
	*/
	virtual int32 GetListeners( IManagedStatus::UIDRefList & listeners );
	
	/** Retrieves a display status string.
		@param stringType is a predefined WGStatusType or other unique identifier.
		@param outString is set to the corresponding display text or empty if no status string is
			available.
	*/
	virtual bool16 GetStatusString( int32 stringType, PMString & outString );
	
	/** Retrieves the current VersionState.
		@return IManagedStatus::VersionState.
	*/
	virtual IManagedStatus::VersionState GetVersionState();
	
	/** This method imports the IAMStatusListeners from another IManagedStatus instance. This method
		is useful when a new IManagedStatus boss must be created to replace an old one.
		@param sourceStatus is a valid pointer to another IManagedStatus instance.
		@see IAMStatusListener.
	*/ 
	virtual void ImportListeners( IManagedStatus * sourceStatus );
	
	/** This method tests if this instance can communicate with the asset management system. For server
		base asset management systems, this tests if the client side has a connection with the server.
		@return kTrue if a connection exists; kFalse otherwise.
	*/
	virtual bool16 IsOnline();
	
	/** This method removes an IAMStatusListener from the status notification list.
		@param listenerRef is a UIDRef of a boss with an IAMStatusListener interface.
		@see IAMStatusListener.	
	*/
	virtual void RemoveListener( UIDRef listenerRef );

	/** This method sets the UIDRef of the IManageableAsset boss associated with this status. This method
		is typically set by the IAMServiceProvider::enInitialize service.
		@param assetRef is the UIDRef of an IManageableAsset.
		@see IManageableAsset.
		@see IAMServiceProvider.
	*/
	virtual void SetAsset( UIDRef assetRef );
	
	/** This method updates status data then calls NotifyListeners() if an status data has changed.
		@param isOnline is the current online status for the asset.
		@param editingState is the current editing state for the asset.
		@param versionState is the current version state for the asset.
		@param inStatusStrings is the current StatusStringTable for displaying status.
		@param inStatusIcons is the current StatusIconTable for displaying status.
		@return kTrue if any status data has changed; kFalse otherwise.
	*/
	virtual bool16 Update( bool16 isOnline,
							IManagedStatus::EditingState editingState,	
							IManagedStatus::VersionState versionState,
							const IManagedStatus::StatusStringTable & inStatusStrings,
							const IManagedStatus::StatusIconTable & inStatusIcons
						  );
	
	/** This method retrieves display status data.
		@param outStatusStrings is populated with the current StatusStringTable.
		@param outStatusIcons is populated with the current StatusIconTable.
	*/
	virtual void GetDisplayStatusData( IManagedStatus::StatusStringTable & outStatusStrings, IManagedStatus::StatusIconTable & outStatusIcons );
						  
	/** This method updates only the display status data.
		@param inStatusStrings is the current StatusStringTable for displaying status.
		@param inStatusIcons is the current StatusIconTable for displaying status.	
		@return kTrue if the display status has changed; kFalse otherwise.
	*/
	virtual bool16 Update( const IManagedStatus::StatusStringTable & inStatusStrings, const IManagedStatus::StatusIconTable & inStatusIcons );				 						 

#if defined(InDnCS5) || defined(InDnCS5_5)
	void				AddNonPersistentListener( IAMStatusListener * listener );
	int32				GetNonPersistentListeners( IManagedStatus::ListenerList & listeners );
	void				RemoveNonPersistentListener( IAMStatusListener * listener );
	PMRsrcID			GetIconPMRsrcID( int32 iconType );
#endif
protected:	

	/** This method notifies listeners that the latest status update is available by calling the
		IAMStatusListener::Update() method for each IAMStatusListener.
		@see IAMStatusListener.
	*/
	virtual void NotifyListeners();
	void		Log(
					const IManagedStatus::StatusStringTable & inStatusStrings ) const;
	void		Log(
					const IManagedStatus::StatusIconTable & inStatusIcons ) const;
private:
	PMIID mDelegateIID;
};


CREATE_PMINTERFACE(CZPAMManagedStatusDelegate, kZPAMManagedStatusDelegateImpl)

CZPAMManagedStatusDelegate::CZPAMManagedStatusDelegate(IPMUnknown * boss)
: inheritClass(boss)
, mDelegateIID( IID_IZPMANAGEDSTATUS )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMManagedStatusDelegate::~CZPAMManagedStatusDelegate()
{

}

#pragma mark -
/** This method adds an IAMStatusListener to the list for status notifications.
	@param listenerRef is a UIDRef of a boss with an IAMStatusListener interface.
	@see IAMStatusListener.
*/						 
void
CZPAMManagedStatusDelegate::AddListener(
	UIDRef listenerRef)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " UID: 0x%08X ", listenerRef.GetUID().Get() );
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->AddListener( listenerRef );
}

/** Gets the UIDRef of the IManageableAsset boss associated with this status instance.
	@return a valid UIDRef unless not properly set.
	@see IManageableAsset.
*/
UIDRef
CZPAMManagedStatusDelegate::GetAsset()
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UIDRef toReturn = orgImpl->GetAsset(  );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : UID: 0x%08X ", toReturn.GetUID().Get() );
		return toReturn;
	}
	return UIDRef::gNull;
}

/** Retrieves the current EditingState for the asset.
	@return IManagedStatus::EditingState.
*/	
IManagedStatus::EditingState
CZPAMManagedStatusDelegate::GetEditingState()
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		IManagedStatus::EditingState toReturn = orgImpl->GetEditingState(  );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %s ", gEditingState[toReturn] );
		return toReturn;
	}
	return IManagedStatus::enUnknownEditingState;
}

/** Retrieves a display icon ID.
	@param iconType is a predefined WGStatusType or other unique identifier.
	@return a valid RsrcID or 0.
*/
RsrcID
CZPAMManagedStatusDelegate::GetIconID(
	int32 iconType)
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		RsrcID toReturn = orgImpl->GetIconID( iconType );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : RscrID: 0x%08X ", toReturn );
		return toReturn;
	}
	return 0;
}

/** Retrieves the list of IAMStatusListeners currently registered with this instance.
	@param listeners is populated with the UIDRef of the IAMStatusListeners.
	@return the number of UIDRefs put in the list.
	@see IAMStatusListener.
*/
int32
CZPAMManagedStatusDelegate::GetListeners(
	IManagedStatus::UIDRefList & listeners)
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GetListeners( listeners );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %d ", toReturn );
		return toReturn;
	}
	return 0;
}

/** Retrieves a display status string.
	@param stringType is a predefined WGStatusType or other unique identifier.
	@param outString is set to the corresponding display text or empty if no status string is
		available.
*/
bool16
CZPAMManagedStatusDelegate::GetStatusString(
	int32 stringType, PMString & outString)
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->GetStatusString( stringType, outString );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : stringType : %d, return : %hd, string : %s ", stringType, toReturn, outString.GrabCString() );
		return toReturn;
	}
	return kFalse;
}

/** Retrieves the current VersionState.
	@return IManagedStatus::VersionState.
*/
IManagedStatus::VersionState
CZPAMManagedStatusDelegate::GetVersionState()
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		IManagedStatus::VersionState toReturn = orgImpl->GetVersionState(  );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : version state : %s ", gVersionState[toReturn] );
		return toReturn;
	}
	return IManagedStatus::enUnknownVersionState;
}

/** This method imports the IAMStatusListeners from another IManagedStatus instance. This method
	is useful when a new IManagedStatus boss must be created to replace an old one.
	@param sourceStatus is a valid pointer to another IManagedStatus instance.
	@see IAMStatusListener.
*/ 
void
CZPAMManagedStatusDelegate::ImportListeners(
	IManagedStatus * sourceStatus)
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->ImportListeners( sourceStatus );

}

/** This method tests if this instance can communicate with the asset management system. For server
	base asset management systems, this tests if the client side has a connection with the server.
	@return kTrue if a connection exists; kFalse otherwise.
*/
bool16
CZPAMManagedStatusDelegate::IsOnline()
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsOnline(  );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd ", toReturn );
		return toReturn;
	}
	return kFalse;
}

/** This method removes an IAMStatusListener from the status notification list.
	@param listenerRef is a UIDRef of a boss with an IAMStatusListener interface.
	@see IAMStatusListener.	
*/
void
CZPAMManagedStatusDelegate::RemoveListener(
	UIDRef listenerRef)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " UID: 0x%08X ", listenerRef.GetUID().Get() );
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->RemoveListener( listenerRef );
}

/** This method sets the UIDRef of the IManageableAsset boss associated with this status. This method
	is typically set by the IAMServiceProvider::enInitialize service.
	@param assetRef is the UIDRef of an IManageableAsset.
	@see IManageableAsset.
	@see IAMServiceProvider.
*/
void
CZPAMManagedStatusDelegate::SetAsset(
	UIDRef assetRef)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " UID: 0x%08X ", assetRef.GetUID().Get() );
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->SetAsset( assetRef );
}

/** This method updates status data then calls NotifyListeners() if an status data has changed.
	@param isOnline is the current online status for the asset.
	@param editingState is the current editing state for the asset.
	@param versionState is the current version state for the asset.
	@param inStatusStrings is the current StatusStringTable for displaying status.
	@param inStatusIcons is the current StatusIconTable for displaying status.
	@return kTrue if any status data has changed; kFalse otherwise.
*/
bool16
CZPAMManagedStatusDelegate::Update(
	bool16 isOnline,
	IManagedStatus::EditingState editingState,
	IManagedStatus::VersionState versionState,
	const IManagedStatus::StatusStringTable & inStatusStrings,
	const IManagedStatus::StatusIconTable & inStatusIcons)
{
	LogFunctionEnterExit;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "isOnline = %hd"
						"editing state : %s "
						"version state : %s "
						, isOnline, gEditingState[editingState], gVersionState[versionState] );
	this->Log( inStatusStrings );
	this->Log( inStatusIcons );

	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Update( isOnline, editingState, versionState, inStatusStrings, inStatusIcons );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd ", toReturn );
		return toReturn;
	}
	return kFalse;
}

/** This method retrieves display status data.
	@param outStatusStrings is populated with the current StatusStringTable.
	@param outStatusIcons is populated with the current StatusIconTable.
*/
void
CZPAMManagedStatusDelegate::GetDisplayStatusData(
	IManagedStatus::StatusStringTable & outStatusStrings, IManagedStatus::StatusIconTable & outStatusIcons)
{
	LogFunctionEnterExit;
	this->Log( outStatusStrings );
	this->Log( outStatusIcons );
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetDisplayStatusData( outStatusStrings, outStatusIcons );
}

/** This method updates only the display status data.
	@param inStatusStrings is the current StatusStringTable for displaying status.
	@param inStatusIcons is the current StatusIconTable for displaying status.	
	@return kTrue if the display status has changed; kFalse otherwise.
*/
bool16
CZPAMManagedStatusDelegate::Update(
	const IManagedStatus::StatusStringTable & inStatusStrings,
	const IManagedStatus::StatusIconTable & inStatusIcons)
{
	LogFunctionEnterExit;
	this->Log( inStatusStrings );
	this->Log( inStatusIcons );
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->Update( inStatusStrings, inStatusIcons );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %hd ", toReturn );
		return toReturn;
	}
	return kFalse;
}

/** This method notifies listeners that the latest status update is available by calling the
	IAMStatusListener::Update() method for each IAMStatusListener.
	@see IAMStatusListener.
*/
void
CZPAMManagedStatusDelegate::NotifyListeners()
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	//if( orgImpl )
	//	orgImpl->NotifyListeners();
}

//----------------------------------------------------------------------------------------
// Log
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatusDelegate::Log(
	const IManagedStatus::StatusStringTable & inStatusStrings) const
{

}

//----------------------------------------------------------------------------------------
// Log
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatusDelegate::Log(
	const IManagedStatus::StatusIconTable & inStatusIcons) const
{

}
#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// AddNonPersistentListener
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatusDelegate::AddNonPersistentListener(
	IAMStatusListener * listener )
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		orgImpl->AddNonPersistentListener( listener );
	}
}

//----------------------------------------------------------------------------------------
// GetNonPersistentListeners
//----------------------------------------------------------------------------------------
int32
CZPAMManagedStatusDelegate::GetNonPersistentListeners(
	IManagedStatus::ListenerList & listeners )
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GetNonPersistentListeners( listeners );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %d ", toReturn );
		return toReturn;
	}
	return 0;
}

//----------------------------------------------------------------------------------------
// RemoveNonPersistentListener
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatusDelegate::RemoveNonPersistentListener(
	IAMStatusListener * listener )
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		orgImpl->RemoveNonPersistentListener( listener );
	}
}

//----------------------------------------------------------------------------------------
// GetIconPMRsrcID
//----------------------------------------------------------------------------------------
PMRsrcID
CZPAMManagedStatusDelegate::GetIconPMRsrcID(
	int32						iconType )
{
	LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		PMRsrcID toReturn = orgImpl->GetIconPMRsrcID( iconType );
		//IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out : %d ", toReturn );
		return toReturn;
	}
	
	return PMRsrcID();//(_statusIcons[iconType]);
}

#endif
