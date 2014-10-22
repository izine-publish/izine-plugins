//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPAssetMgr.h $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPAssetMgr_
#define _h_IZPAssetMgr_
#pragma once

#include "ZPTypes.h"
#include "ZPWSTypeDefs.h"

class IStringData;
class IZPSoapRequest;

class IZPAssetMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETMGR };

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		UpdateAssetList(
							const IStringData *			inEditionID,
							const ZPIDPairSList &		inAssetList,
							const ZPAssetDetailSList &	inAssetInfoList ) = 0;

	virtual void		UpdateAssetLockInfo(
							const ZPAssetLockInfo &		inAssetLockInfo ) = 0;

	virtual void		UpdateAsset(
							const IStringData *			inAssetID,
							const PMString *			inAssetName,
							const ZPAssetDetailInfo *	inAssetInfo ) const = 0;

	virtual void		UpdateAssetName(
							const IStringData *			inDbAssetID,
							const PMString &			inAssetName ) const = 0;

	virtual void		UpdateAssetName(
							const PMString &			inAssetID,
							const PMString &			inAssetName ) const = 0;

	virtual void		UpdateAssetStatus(
							const PMString &			inAssetID,
							const PMString &			inStatusID ) const = 0;

	virtual void		AddAsset(
							const IStringData *			inEditionID,
							const PMString &			inAssetID,			//New AssetID
							const PMString *			inAssetName,
							const ZPAssetDetailInfo *	inAssetInfo ) = 0;

	virtual const IStringData *
						GetAssetByID(
							const PMString &			inAssetID ) const = 0;

	virtual void		FetchAssetsForEdition(
							const PMString &			inEditionID ) const = 0;

	virtual void		DeleteAsset(					//Sends WS request async
							const PMString &			inAssetID ) const = 0;

	virtual void		HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							const AZPSoapResponse *			inResponse,
							ZPWSError						inStatus) = 0;

	virtual void		DecrementReferenceForAssets(
							const ZPAssetIDsList &		inAssetIDs ) = 0;


#if defined(MACINTOSH) && (defined(InDnCS5) || defined(InDnCS5_5))
	static	enTitleStatusState	PLUGIN_DECL GetAssetState(
							const IStringData *			inDBAssetID );
#else
	static	enTitleStatusState	PUBLIC_DECL GetAssetState(
							const IStringData *			inDBAssetID );
#endif
};

#endif //_h_IZPAssetMgr_
