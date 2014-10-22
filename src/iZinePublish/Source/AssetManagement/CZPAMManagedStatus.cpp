//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/CZPAMManagedStatus.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMLockable.h"
#include "IAMStatusListener.h"
#include "IManageableAsset.h"
#include "IManagedStatus.h"

//SDK General includes
#include "CAlert.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AM
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IManagedStatus>
class CZPAMManagedStatus : public inheritClass
{
public:
						CZPAMManagedStatus(IPMUnknown * boss);
	virtual				~CZPAMManagedStatus();
	virtual void		AddListener(
							UIDRef						listenerRef );
	virtual UIDRef		GetAsset();
	virtual IManagedStatus::EditingState GetEditingState() ;
	virtual RsrcID		GetIconID(
							int32						iconType );
	virtual int32		GetListeners(
							IManagedStatus::UIDRefList & listeners ) ;
	virtual bool16		GetStatusString(
							int32						stringType,
							PMString &					outString ) ;
	virtual IManagedStatus::VersionState GetVersionState() ;
	virtual void		ImportListeners(
							IManagedStatus *			sourceStatus ) ;
	virtual bool16		IsOnline();
	virtual void		RemoveListener(
							UIDRef						listenerRef );
	virtual void		SetAsset(
							UIDRef						assetRef );
	virtual bool16		Update(
							bool16						isOnline,
							IManagedStatus::EditingState editingState,
							IManagedStatus::VersionState versionState,
							const IManagedStatus::StatusStringTable & inStatusStrings,
							const IManagedStatus::StatusIconTable & inStatusIcons );
	virtual void		GetDisplayStatusData(
							IManagedStatus::StatusStringTable & outStatusStrings,
							IManagedStatus::StatusIconTable & outStatusIcons ) ;
	virtual bool16		Update(
							const IManagedStatus::StatusStringTable & inStatusStrings,
							const IManagedStatus::StatusIconTable & inStatusIcons ) ;
	
#if defined(InDnCS5) || defined(InDnCS5_5)
	void				AddNonPersistentListener( IAMStatusListener * listener );
	int32				GetNonPersistentListeners( IManagedStatus::ListenerList & listeners );
	void				RemoveNonPersistentListener( IAMStatusListener * listener );
	PMRsrcID			GetIconPMRsrcID( int32 iconType );
#endif
protected:
	virtual void		NotifyListeners() ;
private:
	IManagedStatus::StatusStringTable _statusStrings;
	IManagedStatus::StatusIconTable _statusIcons;
	
	IManagedStatus::EditingState _editingState;
	IManagedStatus::VersionState _versionState;

#if defined(InDnCS5) || defined(InDnCS5_5)
	IManagedStatus::ListenerList _listenerList;
#endif
	
	bool16 _online;
	
	UIDRef _assetRef;
	UIDRefList _listeners;
};


CREATE_PMINTERFACE(CZPAMManagedStatus, kZPAMManagedStatusImpl)

CZPAMManagedStatus::CZPAMManagedStatus(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMManagedStatus::~CZPAMManagedStatus()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddListener
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatus::AddListener(
	UIDRef						listenerRef )
{
	LogFunctionEnterExit;
	
	_listeners.insert(_listeners.end(), listenerRef);
}

//----------------------------------------------------------------------------------------
// GetAsset
//----------------------------------------------------------------------------------------
UIDRef
CZPAMManagedStatus::GetAsset()
{
	return (_assetRef);
}

//----------------------------------------------------------------------------------------
// GetEditingState
//----------------------------------------------------------------------------------------
IManagedStatus::EditingState
CZPAMManagedStatus::GetEditingState()
{
	LogFunctionEnterExit;

	return (_editingState);
}

//----------------------------------------------------------------------------------------
// GetIconID
//----------------------------------------------------------------------------------------
RsrcID
CZPAMManagedStatus::GetIconID(
	int32						iconType )
{
	LogFunctionEnterExit;

#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5: Test functionality
	return (_statusIcons[iconType]).fId;
#else
	return (_statusIcons[iconType]);
#endif
}

//----------------------------------------------------------------------------------------
// GetListeners
//----------------------------------------------------------------------------------------
int32
CZPAMManagedStatus::GetListeners(
	IManagedStatus::UIDRefList & listeners )
{
	LogFunctionEnterExit;

	std::copy (_listeners.begin(), _listeners.end(), listeners.begin());
	return (_listeners.size());
}

//----------------------------------------------------------------------------------------
// GetStatusString
//----------------------------------------------------------------------------------------
bool16
CZPAMManagedStatus::GetStatusString(
	int32						stringType,
	PMString &					outString )
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " (%s) \n",_statusStrings[stringType].GrabCString());

	InterfacePtr<IManageableAsset> asset(_assetRef,UseDefaultIID());
	ASSERT(asset);

	IAMLockable* lock = asset->GetLockable();
	if (lock) {
		// lock->VerifyState
		
		std::map<int32, PMString> stringTable;
		lock->GetLockStatusStrings( stringTable );
		outString = stringTable[stringType];
	}
	
	return (kTrue);
}

//----------------------------------------------------------------------------------------
// GetVersionState
//----------------------------------------------------------------------------------------
IManagedStatus::VersionState
CZPAMManagedStatus::GetVersionState()
{
	LogFunctionEnterExit;

	return (_versionState);
}

//----------------------------------------------------------------------------------------
// ImportListeners
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatus::ImportListeners(
	IManagedStatus *			sourceStatus )
{
	LogFunctionEnterExit;
	
	UIDRefList sourceListeners;
	sourceStatus->GetListeners(sourceListeners);
	for (int i=0; i<sourceListeners.size(); i++)
	{
		this->AddListener(sourceListeners[i]);
	}
}

//----------------------------------------------------------------------------------------
// IsOnline
//----------------------------------------------------------------------------------------
bool16
CZPAMManagedStatus::IsOnline()
{
	LogFunctionEnterExit;
	
	//TODO:
	return kTrue;
}

//----------------------------------------------------------------------------------------
// RemoveListener
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatus::RemoveListener(
	UIDRef						listenerRef )
{
	LogFunctionEnterExit;
	std::remove (_listeners.begin(), _listeners.end(), listenerRef);
}

//----------------------------------------------------------------------------------------
// SetAsset
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatus::SetAsset(
	UIDRef						assetRef )
{
	LogFunctionEnterExit;
	
	this->_assetRef = assetRef;
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
bool16
CZPAMManagedStatus::Update(
	bool16										isOnline,
	IManagedStatus::EditingState				editingState,
	IManagedStatus::VersionState				versionState,
	const IManagedStatus::StatusStringTable &	inStatusStrings,
	const IManagedStatus::StatusIconTable &		inStatusIcons )
{
	LogFunctionEnterExit;
	
	IManagedStatus::StatusStringTable::const_iterator it1 = inStatusStrings.begin();
	for (; it1!=inStatusStrings.end(); it1++)
	{
		CAlert::InformationAlert(_statusStrings[it1->first]);
	}
	
	this->_editingState = editingState;
	this->_versionState = versionState;
	
	this->_online = isOnline;
	
	NotifyListeners();
	
	return (kTrue);
}

//----------------------------------------------------------------------------------------
// GetDisplayStatusData
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatus::GetDisplayStatusData(
	IManagedStatus::StatusStringTable &		outStatusStrings,
	IManagedStatus::StatusIconTable &		outStatusIcons )
{
	LogFunctionEnterExit;
	IManagedStatus::StatusStringTable::iterator it1 = _statusStrings.begin();
	for (; it1!=_statusStrings.end(); it1++)
	{
		outStatusStrings[it1->first] = it1->second;
	}
	
	IManagedStatus::StatusIconTable::iterator it2 = _statusIcons.begin();
	for (; it2!=_statusIcons.end(); it2++)
	{
		outStatusIcons[it2->first] = it2->second;
	}
	
	NotifyListeners();
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
bool16
CZPAMManagedStatus::Update(
	const IManagedStatus::StatusStringTable &	inStatusStrings,
	const IManagedStatus::StatusIconTable &		inStatusIcons )
{
	LogFunctionEnterExit;
	
	IManagedStatus::StatusStringTable::const_iterator it1 = inStatusStrings.begin();
	for (; it1!=inStatusStrings.end(); it1++)
	{
		_statusStrings[it1->first] = it1->second;
	}
	
	IManagedStatus::StatusIconTable::const_iterator it2 = inStatusIcons.begin();
	for (; it2!=inStatusIcons.end(); it2++)
	{
		_statusIcons[it2->first] = it2->second;
	}
	
	return (kTrue);
}

//----------------------------------------------------------------------------------------
// NotifyListeners
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatus::NotifyListeners()
{
	LogFunctionEnterExit;
	
	for (int i=0; i<_listeners.size(); i++)
	{
		InterfacePtr<IAMStatusListener> listener(_listeners[i],UseDefaultIID());
		listener->Update(this);
	}
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// AddNonPersistentListener
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatus::AddNonPersistentListener(
	IAMStatusListener * listener )
{
	//TODO: CS5: Implement functionality
	_listenerList.insert(_listenerList.end(), listener);
}

//----------------------------------------------------------------------------------------
// GetNonPersistentListeners
//----------------------------------------------------------------------------------------
int32
CZPAMManagedStatus::GetNonPersistentListeners(
	IManagedStatus::ListenerList & listeners )
{
	//TODO: CS5: Implement functionality
	std::copy (_listenerList.begin(), _listenerList.end(), listeners.begin());
	return (_listenerList.size());
	//return 0;
}

//----------------------------------------------------------------------------------------
// RemoveNonPersistentListener
//----------------------------------------------------------------------------------------
void
CZPAMManagedStatus::RemoveNonPersistentListener(
	IAMStatusListener * listener )
{
	//TODO: CS5: Implement functionality
	std::remove (_listenerList.begin(), _listenerList.end(), listener);
}

//----------------------------------------------------------------------------------------
// GetIconPMRsrcID
//----------------------------------------------------------------------------------------
PMRsrcID
CZPAMManagedStatus::GetIconPMRsrcID(
	int32						iconType )
{
	LogFunctionEnterExit;

	//TODO: CS5: Test functionality
	return (_statusIcons[iconType]);
}

#endif
