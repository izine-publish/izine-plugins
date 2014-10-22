//========================================================================================
//  
//  $File: //depot/indesign_7.0/gm/source/sdksamples/common/SDKLayoutHelper.h $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: pmbuilder $
//  
//  $DateTime: 2010/02/22 14:31:38 $
//  
//  $Revision: #1 $
//  
//  $Change: 742152 $
//  
//  Copyright 1997-2010 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#ifndef __SDKLayoutHelper_H_DEFINED__
#define __SDKLayoutHelper_H_DEFINED__

#include "IDocFileHandler.h" // or include from client code?
#include "PMPathPoint.h" // because I can't forward decl PMPathPointList?
class ITextModel;
class ITextFrameColumn;
class IGraphicFrameData;

/** Helper methods to perform basics of manipulating the layout
	content in a document.

	All frame creation methods expect the bounds of the object to be
	created to be given in the bounds of the parent. Normally the parent
	will be a spread layer (ISpreadLayer), which implies the bounds are given
	in spread co-ordinates.

	To assist in laying out content relative to the page origin (which is the
	top left corner of the page) a helper method SDKLayoutHelper::PageToSpread
	is provided. This will transform the bounds of a frame from page co-ordinates
	to spread co-ordinates ready to be passed to one of the frame creation methods
	like SDKLayoutHelper::CreateRectangleFrame.

	@ingroup sdk_layout
	
	@see IDocument
	@see IPageList
	@see ISpreadList
	@see ISpread
	@see ISpreadLayer
	@see ILayerList
	@see IDocumentLayer
	@see IGraphicFrameData
	@see IHierarchy
	@see ITransform
	@see IGeometry
	@ingroup sdk_common
*/
class SDKLayoutHelper {

	public:

		/**	Constructor.
		 */
		SDKLayoutHelper();
		
		/**	Destructor.
		 */
		virtual ~SDKLayoutHelper();

	public:
		// Document manipulation helper methods.

		/**	Create a document but without opening a layout view onto it.
			@param uiFlags IN (default kFullUI)
			@param width IN width of page in points (default 612)
			@param height IN height of page in points (default 792)
			@param pages IN number of pages (default 1)
			@param pagesPerSpread IN number of pages per spread (default 1)
			@return UIDRef of document created, or UIDRef::gNull if creation failed; the returned UIDRef can
				be used to instantiate an IDocument
			@postcondition a document will be created and opened if the input parameters are valid, but without
			a layout view. Call SDKLayoutHelper::OpenLayoutWindow to obtain an layout view onto the document.
		 */
		UIDRef CreateDocument(const UIFlags uiFlags = K2::kFullUI,
			const	PMReal& width = 612, 
			const	PMReal& height = 792, 
			const	int32 pages = 1,
#ifdef qME
			const	int32 pagesPerSpread = 1,
			const	int32 pageBinding = 0);
#else
			const	int32 pagesPerSpread = 1);
#endif

		/**	Open an existing document for which the path is known, and acquire UIDRef, but don't open layout window onto it.
			@param sysFile IN specifies the path to the file
			@param uiFlags IN (default kFullUI)
			@precondition the file at specified path should exist and be a type the app can open, i.e. be supported by an open provider service
			@return UIDRef of the document opened, from which an IDocument can be instantiated.
			@postcondition document is in the open document list if opened successfully.
		*/
		UIDRef OpenDocument(const IDFile& sysFile,
			UIFlags uiFlags = K2::kFullUI);

		/**	Close the document, with option to save.
			@param documentUIDRef IN specifies the document to close
			@param saveOnClose kTrue if the document should be saved before closing, kFalse otherwise (default kFalse)
			@param uiFlags IN (default kFullUI)
			@param allowCancel IN kTrue if the user can cancel during the close, kFalse otherwise (default kTrue).
			@param cmdMode IN specifies if the close command should be processed immediately or scheduled (default kSchedule).
			@return kSuccess on success, kFailure otherwise
		*/
		ErrorCode CloseDocument(const UIDRef& documentUIDRef, 
			bool16 saveOnClose = kFalse,
			UIFlags uiFlags = K2::kFullUI, 
			bool8 allowCancel = kTrue, 
			IDocFileHandler::CloseCmdMode cmdMode = IDocFileHandler::kSchedule);

		/**	Save document to specified path. 
			@param documentUIDRef IN specifies document of interest
			@param sysFile IN path to save to
			@param uiFlags IN control over associated user interface (default kFullUI)
			@precondition documentUIDRef and sysFile should be valid 
			@return kSuccess if document could be saved, kFailure otherwise
			@postcondition document written out on success
		*/
		ErrorCode SaveDocumentAs(const UIDRef& documentUIDRef, 
			const IDFile& sysFile,		
			UIFlags uiFlags = kFullUI);

		/**	Open a layout view on the document.
			@param documentUIDRef references an IDocument object.
			@return kSuccess if window opened, other ErrorCode otherwise.
		 */
		ErrorCode OpenLayoutWindow(const UIDRef& documentUIDRef);

	public:
		// Spread & page manipulation:

		/** Acquire UIDRef of active ISpreadLayer. 
			@param spreadUIDRef IN references the spread whose active spread layer is wanted
			@param wantGuideLayer IN kTrue if guide spread layer wanted, kFalse for content spread layer (default kFalse)
			@return UIDRef of spreadlayer(ISpreadLayer) or UIDRef::gNull if no active spread layer is available.
		*/	
		UIDRef GetActiveSpreadLayerRef(const UIDRef& spreadUIDRef, 
								bool16 wantGuideLayer=kFalse);

		/** Acquire UIDRef of the spread layer (ISpreadLayer) associated with
			the indicated document layer where the desired document layer is
			indicated by its index into ILayerList. 
			@param spreadUIDRef reference to spread (ISpread) reference
			@param documentLayerIndex index in ILayerList of document layer whose spread layer is wanted. Default to 1 as 0 is the pages layer
				and we're likely interested in contents
			@param wantGuideLayer IN kTrue if guide spread layer wanted, kFalse for content spread layer (default kFalse)
			@return UIDRef of spreadlayer(ISpreadLayer) or UIDRef::gNull if no spread layer exists for this documentLayerIndex.
		*/	
		UIDRef GetSpreadLayerRef(const UIDRef& spreadUIDRef, 
								int32 documentLayerIndex=1, 
								bool16 wantGuideLayer=kFalse);

		/**	Return boundingBox transformed from page to spread co-ordinates. Laying
			out of content relative to a page is easy since the page origin is at the
			top left of the page. This call accounts for margins so the origin
			is adjusted to be the top left margin rather than the top left of the page (it
			is assumed that all content is being positioned within the margins).
			@param pageUIDRef references page the boundingBox lies on.
			@param boundsInPageCoords bounds in page co-ordinate space. For example PMRect(10,10,30,30)
				is specifies a 20 point by 20 point square 10 points in and down from 
				the top left margin on the page.
			@return the bounding box in spread's co-ordinate system
		 */
		PMRect	PageToSpread(const UIDRef& pageUIDRef, const PMRect& boundsInPageCoords);
	
	public:
		// Frame creation:

		/**	Create a rectangular frame and place a content file inside it, if the placed
			content is a text this will result in a text frame, otherwise a frame containing
			a picture will be created.
			@param contentFile file containing content to be placed in frame.
			@param parentUIDRef IN reference to an IHierarchy item. Normally this is a kSpreadLayerBoss 
				but could be another type of page item that will parent the new item.
			@param boundsInParentCoords IN bounds in parent's co-ordinate space. When the parent is a 
				kSpreadLayerBoss this is spread co-ordinates.
			@param uiFlags IN (default kFullUI) controls whethet the import provider displays
				its options for the import.
			@param retainFormat IN (default kTrue).
			@param convertQuotes IN (default kTrue).
			@param applyCJKGrid IN (default kFalse).
			@param uidPreview IN (default kInvalidUID) identifies a proxy
			@return UIDRef of frame(IGraphicFrameData) created, or UIDRef::gNull if failed.
		 */
		UIDRef PlaceFileInFrame(const IDFile& contentFile,
			const UIDRef& parentUIDRef,
			const PMRect& boundsInParentCoords, 
			const UIFlags uiFlags = K2::kFullUI, 
			const bool16 retainFormat = kTrue, 
			const bool16 convertQuotes = kTrue, 
			const bool16 applyCJKGrid = kFalse,
			const UID& uidPreview = kInvalidUID);

		/**	Create a rectangular graphic frame of specified dimensions.
			@param parentUIDRef IN reference to an IHierarchy item. Normally this is a kSpreadLayerBoss 
				but could be another type of page item that will parent the new item.
			@param boundsInParentCoords IN bounds in parent's co-ordinate space. When the parent is a 
				kSpreadLayerBoss this is spread co-ordinates.
			@return UIDRef of frame(IGraphicFrameData) created, or UIDRef::gNull if failed.
		 */
		UIDRef	CreateRectangleFrame(const UIDRef& parentUIDRef, const PMRect& boundsInParentCoords);

		/** Create a text frame of specified dimensions.
			@param parentUIDRef IN reference to an IHierarchy item. Normally this is a kSpreadLayerBoss 
				but could be another type of page item that will parent the new item.
			@param boundsInParentCoords IN bounds in parent's co-ordinate space. When the parent is a 
				kSpreadLayerBoss this is spread co-ordinates.
			@param numberOfColumns IN the number of columns (default 0 which means that the 
				text frame options preference number of columns will be used).
			@param verticalFrame IN kTrue to make the text frame vertical.
			@param storyUIDRef IN/OUT pass a pointer to get back the reference to the story's text model(ITextModel),
				pass nil if you don't care.
			@return UIDRef of frame created, from which an IGraphicFrameData can be instantiated if the call succeeded, 
				or UIDRef::gNull if failed.
		*/
		UIDRef CreateTextFrame(const UIDRef& parentUIDRef,
								const PMRect& boundsInParentCoords,
								int32 numberOfColumns = 0,
						  		bool16 verticalFrame = kFalse,
#ifdef qME
								UIDRef* storyUIDRef = nil,
								bool16 isLeftToRight = kTrue);
#else								
								UIDRef* storyUIDRef = nil);
#endif
		
		/**	Create a rectangle of specified dimensions.
			@param parentUIDRef IN reference to an IHierarchy item. Normally this is a kSpreadLayerBoss 
				but could be another type of page item that will parent the new item.
			@param boundsInParentCoords IN bounds in parent's co-ordinate space. When the parent is a 
				kSpreadLayerBoss this is spread co-ordinates.
			@return UIDRef of graphic created, from which an IGraphicFrameData interface can be instantiated, 
				or UIDRef::gNull if failed.
		 */
		UIDRef	CreateRectangleGraphic(const UIDRef& parentUIDRef, const PMRect& boundsInParentCoords);

		/**	Create a spline described by the given pathPointList translated and scaled to
			the bounds in the parent's co-ordinate space given by boundsInParentCoords.
			@param parentUIDRef IN reference to an IHierarchy item. Normally this is a kSpreadLayerBoss 
				but could be another type of page item that will parent the new item.
			@param boundsInParentCoords IN bounds in parent's co-ordinate space. When the parent is a 
				kSpreadLayerBoss this is spread co-ordinates.
			@param boundsInInnerCoords IN bounds of the pathPointList in inner co-ordinates.
			@param pathPointList IN/OUT on input contains a list of PMPathPoint's in inner co-ordinates.
				On output these are transformed so the path is translated and scaled to the
				given boundsInParentCoords.
			@param bClosedPath kTrue if pathPointList describes a closed path, kFalse if open.
			@return UIDRef of spline created, from which an IGraphicFrameData can be instantiated,
				or UIDRef::gNull if failed.
		 */
		UIDRef	CreateSplineGraphic(const UIDRef& parentUIDRef, 
									const PMRect& boundsInParentCoords, 
									const PMRect& boundsInInnerCoords, 
									PMPathPointList& pathPointList, 
									bool16 bClosedPath = kTrue);

	public:
		// Layout model navigation helpers.

		/**	Get reference to the graphic frame associated with the given text frame.
			@param textFrameColumn reference to a text frame (ITextFrameColumn), 
				normally a kMultiColumnItemBoss object or a kFrameItemBoss object but can be text on path.
			@return UIDRef of the associated graphic frame, 
				from which an IGraphicFrameData can be instantiated, or UIDRef::gNull if none.
		*/
		UIDRef GetGraphicFrameRef(const InterfacePtr<ITextFrameColumn>& textFrameColumn);

		/** Get reference to the text model associated with the given graphic frame.
			@param graphicFrameData reference to a graphic frame, normally a kSplineItemBoss object.
			@return UIDRef of the associated story's text model (from which an ITextModel can be instantiated), 
				or UIDRef::gNull if none.
		 */
		UIDRef GetTextModelRef(const InterfacePtr<IGraphicFrameData>& graphicFrameData);

};



#endif // __SDKLayoutHelper_H_DEFINED__

