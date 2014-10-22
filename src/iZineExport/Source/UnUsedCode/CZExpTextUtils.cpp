//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/UnUsedCode/CZExpTextUtils.cpp $
//	$Revision: 2608 $
//	$Date: 2011-03-15 13:14:03 +0100 (Tue, 15 Mar 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-3-2011
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "ITextUtils.h"
#include "ParcelKey.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ALL
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<ITextUtils>
class CZExpTextUtils : public inheritClass
{
public:
					CZExpTextUtils(IPMUnknown * boss);
					~CZExpTextUtils();

					PMIID mDelegateIID;


	virtual ICommand* QueryCopyStoryCommand(const UIDRef& source, const UIDRef& dest, const StoryRange& sourceRange, const StoryRange& destRange, const bool16 ignoreAttributes = kFalse );

	/** Function for copying text from one story to another.
	This copies the specified text from the source story to the end of the destination story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on copy, default to kFalse
	*/
	virtual ICommand* QueryCopyStoryToEndCommand(const UIDRef& source, const UIDRef& dest, const StoryRange& sourceRange, const bool16 ignoreAttributes = kFalse );

	/** Function for copying all of the text from one story to another.
	This replaces the destination story with a copy of the source story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on copy, default to kFalse
	*/
	virtual ICommand* QueryCopyStoryFromAllToAllCommand(const UIDRef& source, const UIDRef& dest, const bool16 ignoreAttributes = kFalse );

	/** Function for copying text from one story to another.
	This copies the entire source story to the end of the destination story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on copy, default to kFalse
	*/
	virtual ICommand* QueryCopyStoryFromAllToEndCommand(const UIDRef& source, const UIDRef& dest, const bool16 ignoreAttributes = kFalse );

	/** Function for moving text from one story to another.
	This replaces the range in the destination story with the specified text from the source story.
	The source range is removed from the source story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on move, default to kFalse
	*/
	virtual ICommand* QueryMoveStoryCommand(const UIDRef& source, const UIDRef& dest, const StoryRange& sourceRange, const StoryRange& destRange, const bool16 ignoreAttributes = kFalse );

	/** Function for moving text from one story to another.
	The entire source story is moved to the destination range.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on move, default to kFalse
	*/
	virtual ICommand* QueryMoveStoryFromAllCommand(const UIDRef& source, const UIDRef& dest, const StoryRange& destRange, const bool16 ignoreAttributes = kFalse );

	/** Function for moving text from one story to another.
	This replaces the destination story with the source story.
	The source range is removed from the source story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on move, default to kFalse
	*/
	virtual ICommand* QueryMoveStoryFromAllToAllCommand(const UIDRef& source, const UIDRef& dest, const bool16 ignoreAttributes = kFalse);

	/** Function for moving text from one story to another.
	This moves the entire source story to the end of the destination story.
	The source range is removed from the source story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on move, default to kFalse
	*/
	virtual ICommand* QueryMoveStoryFromAllToEndCommand(const UIDRef& source, const UIDRef& dest, const bool16 ignoreAttributes = kFalse);

	/**
	Strip various types of characters from some text.
	@param rText is the source text.
	@param pStripped will be the converted text. If nil, nothing occurs and we return rText.
	@return Returns the converted text (pStripped) unless nil, then return rText.
	*/
	virtual const WideString& ConvertTextModelText( const WideString &rText,
														WideString *pStripped,
														bool16 stripHardHyphens = kTrue,
														bool16 stripDiscretionaryHyphens = kTrue,
														bool16 convertApostrophes = kTrue,
														bool16 stripNobreakNonspace = kTrue,
														bool16 convertHyphens = kTrue,
														bool16 stripLeadingSpaces = kTrue );

	/**
	Collects UIDs of items with specified interface from the text focus. e.g. IID_IGRAPHICFRAMEDATA, IID_ITOPFRAMEDATA
	Can be a costly function with a text selection that spans many pages. See FocusSpansMultiplePageItems.
	*/
	virtual bool16 CollectPageItemsFromFocus(const ITextFocus *pFocus, const PMIID& iid, UIDList *pList );

	/** Does the focus span more than one page item frames.
	*/
	virtual bool16 FocusSpansMultiplePageItems(const ITextFocus *pFocus);

	/** returns the index of a multicolumn frame in its thread
	*/
	virtual int32 GetMultiColFrameIndex( IPMUnknown* multiColFrame );

	/**
	Find the word at a text location.
	@param pModel The text model in question.
	@param nPosition The text location in question.
	@param pLength OUT the length of the word.
	@param aFunction a function that determines what characters are considered word breaking.
	@return the beginning location of the surrounding word.
	@see IComposeScanner.
	@see ILanguage.
	*/
	virtual	TextIndex FindSurroundingWord( const ITextModel *pModel, TextIndex nPosition, int32 *pLength, FuncIsWordChar aFunction );

	/**
	Find the word at a text location.
	@param pModel The text model in question.
	@param nPosition The text location in question.
	@param pLength OUT the length of the word.
	@param aFunction a function that determines what characters are considered word breaking.
	@return the beginning location of the surrounding word.
	@see IComposeScanner.
	@see ILanguage.
	*/
	virtual	TextIndex FindSurroundingWordUsingWordCharFuncOnly( const ITextModel *pModel, TextIndex nPosition, int32 *pLength, const FuncIsWordChar aFunction );

	/** Is this style name considered plain? (For example, "Roman", "Regular", "")
	*/
	virtual	bool16 IsPlainStyleName( const PMString &styleName );

	/** Is this style name considered italic?  (For example, "Italic", "Oblique")
	*/
	virtual	bool16 IsItalicStyleName( const PMString &styleName );

	/** Is this style name considered bold?  (For example, "Bold", "Heavy", "Black")
	*/
	virtual	bool16 IsBoldStyleName( const PMString &styleName );

	/** Is this style name considered bold and italic?
	*/
	virtual	bool16 IsBoldItalicStyleName( const PMString &styleName );

	/**
	used by ApplyTextAttrToWorkspaceCmd, UserApplyAttrCmd, and TextState.
	*/
	virtual bool16 GetFontStyleName( ITextAttrFont *fromFont, ITextAttrFont *toFont, PMString *styleName );

	/**
	Remap font styles tries to choose the expected family/style and allow roundtripping.
	*/
	virtual bool16 RemapFontStyles( IDataBase *dataBase, const AttributeBossList &stateOverrides, const AttributeBossList &newOverrides,
						PMString *styleName, ClassID textAttrFontUIDBoss = kTextAttrFontUIDBoss, ClassID textAttrFontStyleBoss = kTextAttrFontStyleBoss,
						bool16 remapExactFontStylesOnly = kFalse);

	/**	Is this font an opentype font?
	*/
	virtual	bool16 IsOpenTypeFont( const IPMFont *font );

	/**	Is this font family an opentype font family?
	*/
	virtual	bool16 IsOpenTypeFamily( IFontFamily *fontFamily );

	// Check the current char is a word break or not.
	virtual bool16 IsWordBreak(const UTF32TextChar& curChar);

	/**	Gets the display font names for font
		@param *fontGroup	font group for font
		@param *font		font to get name from
		@param &familyName	OUT
		@param &styleName	OUT
		@param namesFlag	See enum DisplayNamesFlag.
	 */
	virtual void GetDisplayFontNames( IFontGroup *fontGroup, IPMFont *font, PMString &familyName, PMString &styleName, DisplayNamesFlag namesFlag );


	/**	Gets the display font names for font
		@param *dataBase	database of family UID
		@param fontUID		family UID
		@param fontStyle	style of font
		@param &familyName	OUT
		@param &styleName	OUT
		@param namesFlag	See enum DisplayNamesFlag.
	 */
	virtual void GetDisplayFontNames( IDataBase *dataBase, const ITextAttrUID* fontUID, const ITextAttrFont* fontStyle,
								PMString &familyName, PMString &styleName, DisplayNamesFlag namesFlag );

	/**
	Returns the list of owned items associated with a TextRange
	@param model	The TextModel
	@param start	The starting TextIndex of Text range to collect from.
	@param len		The length of the Text range to collect from.
	@param resultList
	@param nested 	If kFalse, only the OwnedItems in the specified Text range
					are returned. If kTrue, then all nested OwnedItems, that
					is OwnedItems that are contained in StoryRanges managed
					by found OwnedItems, are also returned.
					See CollectStoryRanges() for more information on nesting.
	*/
	virtual void CollectOwnedItems(const ITextModel* model,
									TextIndex start, int32 len,
									OwnedItemDataList* resultList,
									bool16 nested = kFalse);

	/**
	returns the list of owned items associated with a TextRange that have an IVisitorHelper interface.
	*/
	virtual void CollectVisitableItems(ITextModel* model,
							TextIndex start, int32 len,
							VisitableItems *resultList);

	/**
	returns the child for each Inline UID in the owned item strand
	*/
	virtual void GetUIDListOfInlines(IParcel* parcel, bool16 bWithDropCaps,
										UIDList* resultList);

	/**
	Returns the list of *logically* sequential story ranges that derive
	from the specified span of the text model. For a typical model span
	with no special content this will simply return a single StoryRange
	as (start, start + len).
	If a table, or other type of owned item which contains nested text is
	within the specified span then the specified span will be returned as
	two ranges with the appropriate nested text range(s) inserted between
	them.
	The hIndex and hIndexList are optional parameters which allows the
	caller to identify the starting TextIndex of the hierarchical element
	that "owns" the equivalent range in the resultList. Ranges from the main
	story thread (which starts at TextIndex 0) will have the hIndex of
	kInvalidTextIndex. Hierarchical elements that are found in the main
	story thread will have hIndex values within the main story thread and
	if those elements have sub elements then those will be in other story
	threads.
	@param bIncludeAnchorSpan is optional parameter which provides flexibility
		to client if they want to include anchor when calculating net span.
		For example, we don't want to include anchor span(such as table)
		when when do line count, e.g., for a 4x4 table, we want the line count
		as 16 instead of 17.
	*/
	virtual void CollectStoryRanges(const ITextModel* model,
										TextIndex start, int32 len,
										StoryRangeList* resultList,
										TextIndex hIndex = kInvalidTextIndex,
										ITextUtils::HIndexList *hIndexList = nil,
										bool16 bIncludeAnchorSpan = kTrue);

	/**
	Builds a list of text ranges for the specified story that are in
	logical order. The first range will begin at 'start' and the rest of
	the ranges will wrap around the end of the story to end at 'start - 1'.
	*/
	virtual void BuildLogicalStoryRangeList(ITextModel* model,
												TextIndex start,
												StoryRangeList* resultList);

	/**
	Return the PMMatrix to transform a point in WaxCoordinates in a
	Parcel to the owning Frame. Useful if you are an inline and you want
	to find out where you are relative to the Frame.
	*/
	virtual PMMatrix GetWaxToFrameMatrix(const IParcel* parcel);

	/**
	Get the Content Bounds of the Parcel at the specified TextIndex.
	If there is no Parcel at that TextIndex then it returns kFalse. Note
	that if the text at that location isn't fully composed then the Parcel
	you get may not be the same Parcel after composition.
	*/
	virtual bool16 GetParcelContentBounds(ITextParcelList* tpl,
											TextIndex at, PMRect *pCBounds);

	/** Is the text parcel list overset?
	@return returns kTrue if the specified ParcelList has any characters in addition
	to the final carriage return not mapped to a Parcel. If it's ONLY the
	final carriage return that is overset that it is NOT considered overset.
	*/
	virtual bool16 IsOverset(const IFrameList* fl);

	/** returns false if writingScript and text are contradicting.
	*/
	virtual	bool16 IsScriptOfText( const char *anyText, int32 writingScript );

	/** returns the best guess for the text's writing script, preferredWritingScript is being used as a hint.
	*/
	virtual	int32 GuessScriptOfText( const char *anyText, int32 preferredWritingScript = kDontKnowScript );

	/** returns whether the char needs to be skipped during remapping.
	Used by Text::RemapCharacterValue.
	*/
	virtual	bool16 SkipTextCharForRemapping( const UTF32TextChar& textChar );

	/** returns whether the char needs to be skipped during remapping.
	Used by UserApplyAttrCmd, ScriptUserEditCmdResp, and CJKUserEditCmdResp.
	*/
	virtual	bool16 SkipTextCharForFontLocking( const UTF32TextChar& textChar );

	/** returns the remapped textchar after converting to platform encoding using oldFont and back to newFont.
	*/
	virtual	UTF32TextChar RemapCharacterValue( IPMFont *oldFont, IPMFont *newFont, const UTF32TextChar& textChar );

	/** Create a new story in the specified database.
	@param defaults specified whether the user's default attributes should be applied to the new story
	*/
	virtual UID CreateNewStory(IDataBase *db, bool16 defaults = kTrue);

	/** Writing script to Language ID conversion.
	*/
	virtual LanguageID	ScriptToLanguageID( int32 writingScript );

	/**
	Returns a command to force recomposition (use to avoid CmdBehaviorMonitor
	asserts if you need to force recomposition from within another command)
	*/
	virtual ICommand* CreateRecomposeThruNthParcelCmd( const ITextParcelList* tpl, int32 parcelIndex ) ;

	/**
	Returns a command to force recomposition (use to avoid CmdBehaviorMonitor
	asserts if you need to force recomposition from within another command)
	*/
	virtual ICommand* CreateRecomposeThruTextIndexCmd( const ITextParcelList* tpl, TextIndex textIndex ) ;

	/**
	Determines the pair kern at a particular text index.
	@param kernService refers to the ClassID of a boss that has the IPairKernDict interface to use.
	@return this method returns an ErrorCode (kSuccess, kFailure, kCancel)
	*/
	virtual int32 DeterminePairKern(const ITextModel* model, TextIndex index, ClassID kernService, PMReal& result);

		/**
	Returns the requested attributes from the preferences of the document specified. prefsFromDocument should
	be non nil. For paragraph and character overrides only continuing attributes are returned.
	*/
	virtual void GetDefaultTextAttrPreferences(const WhichTextAttrs which, TextAttrs& out, IDocument* prefsFromDocument);

	/**
	2.0: Tables use one or more characters to "anchor" the table to the
	text flow. Inserting within these ranges is considered bad form and
	will lead to crashes. You may use this method to launder the ranges
	to encompass all of the table instead of just part.
	*/
	virtual void AdjustForTableRange(const UIDRef& story, TextIndex* pStart, TextIndex* pEnd, TextIndex* nextLine);

	/**
	Align text in the primary story thread in the specified text frame. This
	is NOT suitable for alignment in any other TextStoryThread.
	*/
	virtual bool16 AlignToBaselineGrid(IGeometry* pageItem, IParcel* parcel,
									const IBaselineGridPrefs *gridPrefs,
									PMReal *pYTop, PMReal *pYBottom,
									Text::GridAlignmentMetric alignment,
									PMReal alignmentMetricOffset) const;

	/**
	Looks for findOldItemUI from "start" and replaces it with newItemUID in the item strand.
	This is typically used by commands that create owned items during import.
	In case the owned item is contained in a table and there is text after the end of the table (ie after
	the table in the tables containing text story thread) then the TextIndex stored in the command may not
	be valid. The returned TextIndex is the new location at which findOldItemUID was found in the item strand.
	*/
	virtual TextIndex FindResetOwnedItemUID(IItemStrand* itemStrand, const ClassID objectType, const UID findOldItemUID, const TextIndex start, const UID newItemUID);

	/**
	returns the position of first remapped textchar in buffer and copies the remapped textchars in  replacedChars after converting to platform encoding using oldFont and back to newFont.
	*/
	virtual	int32 RemapCharacterValues( IPMFont *oldFont, IPMFont *newFont, const textchar * buffer,  int32 bufferLength, WideString *replacedChars );

	/** Counts number of Unicode characters in a UTF16TextChar buffer
	*/
	virtual int32 CountChars(const UTF16TextChar *buffer, int32 length) const;

	/**
	Translates character (can be multiple UTF16s) offset into UTF16 offset in UTF16TextChar buffer
	*/
	virtual int32 CharOffsetToUTF16Offset(const UTF16TextChar *buffer, int32 length, int32 charOffset) const;


	/**  GetSelectedTextItemsFromTextTarget gathers UIDs for the text frames that are currently selected.
		@param textTarget IN The current ITextTarget you want to gather selected text items from.
		@param textframes OUT The text frames that are currently selected.
		@param mcfs OUT The multi column frames that are currently selected.
		@param splineItems OUT The spline items that are currently selected.
	*/
	virtual void GetSelectedTextItemsFromTextTarget(const ITextTarget * textTarget, UIDList * textframes, UIDList * mcfs, UIDList * splineItems);

	/**  GetSelectedTextItemsFromLayoutTarget gathers UIDs for the text frames that are currently selected.
		@param layoutTarget IN The current ILayoutTarget you want to gather selected text items from.
		@param textframes OUT The text frames that are currently selected.
		@param mcfs OUT The multi column frames that are currently selected.
		@param splineItems OUT The spline items that are currently selected.
	*/
	virtual void GetSelectedTextItemsFromLayoutTarget(const ILayoutTarget * layoutTarget, UIDList * textframes, UIDList * mcfs, UIDList * splineItems);

	/**  GetSelectedTextItemsFromLayoutTarget gathers UIDs for the text frames that are currently selected.
		@param UIDList IN The pageitems to look through for text frames
		@param textframes OUT The text frames that are currently selected.
		@param mcfs OUT The multi column frames that are currently selected.
		@param splineItems OUT The spline items that are currently selected.
	*/
	virtual void GetSelectedTextItemsFromUIDList(const UIDList& pageITems, UIDList * textframes, UIDList * mcfs, UIDList * splineItems);

	/** GetTextFramesContaining returns the range of TextFrames that the
		Parcels containing the Text Range map to. The Range MUST be within
		a single StoryThread. pFirstFrameIndex will be -1 if the Text Range
		does not map to a Text Frame.
	*/
	virtual void	GetTextFramesContaining(const ITextModel *textModel,
											const RangeData& range,
											int32 *pFirstFrameIndex,
											int32 *pLastFrameIndex);

	virtual UIDList* CreateOppositeDirectionFrameList (UIDList *pStoryList,
					ITextOptions::WritingDirection dir);

	/** GetUIDListOfTextFrames returns a list of TextFrames that the Parcels
		containing the specified Text Range map to. The Range MAY cross
		StoryThread boundries.
	*/
	virtual void	GetUIDListOfTextFramesFromRange(const ITextModel* textModel, const RangeData& range, UIDList* resultList);

	/**
	Fills in inkResourceData with the known inks in the specified Range. It incorporates
	handling of the last CR in the Story Thread, so callers need not take care of removing
	it from the range.

	Notes:
		1. This routine only examines Text attributes from a canned list and is currently not extensible.
		2. The caller is responsible for making sure that the specified range is fully composed.
	*/
	virtual void	GetUsedInks(const ITextModel* textModel, TextIndex threadEnd,
								TextIndex start, int32 len, IInkResourceData* inkResourceData);

	/**
	Notifies the last Frame in the FrameList that its overset state may have
	changed. This call simply posts the appropriate inval rect for the location
	of the overset indicator and adds the Frame to the RecomposedFrames list.
	Note that the caller must ensure that at least one Frame in the FrameList
	is damaged or else the inval may never be played until the Frame becomes
	damaged and is recomposed.
	@param frameList
	*/
	virtual void	NotifyOversetChanged(const IFrameList* frameList);

	/** Return whether or not the given object is a MultiColumn TextFrame
	@param obj can be any object
	*/
	virtual bool16	IsMultiColumnFrame(const IPMUnknown* obj) const;
		
	/** Given an ITextFrameColumn, return the associated MultiColumn TextFrame.
	@param tfc Pointer to ITextFrameColumn
	*/
	virtual IMultiColumnTextFrame*	QueryMultiColumnFrame(ITextFrameColumn* tfc) const;

	/** Const version of QueryMultiColumnFrame
	*/
	virtual const IMultiColumnTextFrame*	QueryMultiColumnFrame(const ITextFrameColumn* tfc) const;

	/** UID version of QueryMultiColumnFrame
	*/
	virtual UID	QueryMultiColumnFrameUID(const ITextFrameColumn* tfc) const;

	/** If the spline contains text, return the associated multi-column object.
	If spline doesn't contain text and if the spline has TOP, return the multi-column object associated with kTOPFrameItemBoss.
	return nil, for other conditions.
	*/
	virtual	IMultiColumnTextFrame*	QueryMCFOrTOPFromSpline(const IGraphicFrameData* gfd) const;
	virtual	UID	GetMCFOrTOPFromSpline(const IGraphicFrameData* gfd) const;

	/** Given an IMultiColumnTextFrames, return the spline associated with it
	@param mcf Pointer to IMultiColumnTextFrame
	@return Pointer to IHierarchy
	*/
	virtual IHierarchy* QuerySplineFromTextFrame(const IMultiColumnTextFrame* mcf) const;

	/** Given an ITextFrameColumn, return the spline associated with the parent MultiColumnTextFrame.
	@param tfc Pointer to ITextFrameColumn
	@return Pointer to IHierarchy
	*/
	virtual IHierarchy* QuerySplineFromTextFrame(const ITextFrameColumn* tfc) const;

	/**
	*/
	virtual ErrorCode ProcessScaleParcelContent(const ITextParcelList* tpl,
											ParcelKey key,
											const PMReal& xScale,
											const PMReal& yScale,
											IScaleObject::ScaleContext& context) const;

	/**
		Returns the uid of the page which this text frame is one, can return kInvalidUID
		@param pageItem -- pageitem to find page UID of
		@return UID
	 */
	virtual UID GetTextFramesPageItemUID(IGeometry* pageItem) const;

	/**
	   Do character, word, paragraph and line count

	   @param startIndex		--- start index for counting
	   @param lengthToCount		--- how far should count stop.
	   @param action			--- just update line or update all.
	   @param state				--- start or continue counting.
	   @param wordStarted		--- left over state(word is started or not) of last time.
	   @param characters		--- count of characters. (passed out parameter)
	   @param words				--- count of words. (passed out parameter)
	   @param paragraphs		--- count of paragraphs. (passed out parameter)
	   @param lines				--- count of lines. (passed out parameter)
	   @param oversetCharacters	--- count of characters in overset. (passed out parameter)
	   @param oversetWords		--- count of words in overset. (passed out parameter)
	   @param oversetParagraphs	--- count of paragraphs in overset. (passed out parameter)
	   @param oversetLines		--- count of lines in overset. (passed out parameter)
    */
	
	virtual void GetWordCountInfo(
						const ITextModel* iTextModel,
						const uint32& startIndex,
						const int32& lengthToCount,
						const ITextUtils::WordCountUpdateAction& action,
						const ITextUtils::WordCountUpdateState& state,
						bool16& wordStarted,
						uint32& characters,
						uint32& words,
						uint32& paragraphs,
						uint32& lines,
						uint32& oversetCharacters,
						uint32& oversetWords,
						uint32& oversetParagraphs,
						uint32& oversetLines,
						bool16& bWordAcrossOverset,
						TextStoryThreadHelper& helper
					);

	/**
	   Do roman character, all character, katakana, hiragana, kanji, paragraph and line count

	   @param startIndex			--- start index for counting
	   @param lengthToCount			--- how far should count stop.
	   @param action				--- just update line or update all.
	   @param state					--- start or continue counting.
	   @param wordStarted		    --- left over state(word is started or not) of last time.
	   @param fullWidthChars		--- count of full-width chars including full-width numbers, symbols and punctuations. (passed out parameter)
	   @param oversetFullWidthChars	--- count of full-width chars including full-width numbers, symbols and punctuations in overset. (passed out parameter)
	   @param halfWidthChars		--- count of half-width chars including half-width numbers, symbols and punctuations. (passed out parameter)
	   @param oversetHalfWidthChars	--- count of half-width chars including half-width numbers, symbols and punctuations in overset. (passed out parameter)
	   @param japaneseChars         --- count of japanese chars including full and half-width Hiragana and Katakana. (passed out parameter)
	   @param oversetJapaneseChars  --- count of japanese chars including full and half-width Hiragana and Katakana in overset. (passed out parameter)
	   @param kanjis				--- count of kanjis. (passed out parameter)
	   @param oversetKanjis			--- count of kanjis in overset. (passed out parameter)
	   @param words					--- count of words. (passed out parameter)
	   @param oversetWords		    --- count of words in overset. (passed out parameter)
	   @param lines					--- count of lines. (passed out parameter)
	   @param oversetLines			--- count of lines in overset. (passed out parameter)
	   @param paragraphs			--- count of paragraphs. (passed out parameter)
	   @param oversetParagraphs		--- count of paragraphs in overset. (passed out parameter)
	   @param allChars				--- count of all characters(passed out parameter)
	   @param oversetAllChars       --- count of all characters in overset. (passed out parameter)
    */
	virtual void GetWordCountInfo(
						const ITextModel* iTextModel,
						const uint32& startIndex,
						const int32& lengthToCount,
						const ITextUtils::WordCountUpdateAction& action,
						const ITextUtils::WordCountUpdateState& state,
						bool16&  wordStarted,
						uint32&  fullWidthChars, uint32& oversetFullWidthChars,
						uint32&  halfWidthChars, uint32& oversetHalfWidthChars,
						uint32&  japaneseChars,  uint32& oversetJapaneseChars,
						uint32&  kanjis,		 uint32& oversetKanjis,
						uint32&  words,          uint32& oversetWords,
						uint32&  lines,          uint32& oversetLines,
						uint32&  paragraphs,     uint32& oversetParagraphs,
						uint32&  allChars,       uint32& oversetAllChars,
						bool16&  bWordAcrossOverset,
						TextStoryThreadHelper& helper
					);

	/**
		return the number of overset lines in a given text model
	 */
	virtual int32 CountOversetLines(const UIDRef &textModelUIDRef);

	/**
		returns whether the given ITextFocus is a footnote and optionally returns the UIDRef for that footnote
	   @param pFocus					--- The ITextFocus in question
	   @param pFootnoteUIDRef		---  If non-nil and there is a footnote at that model-position, the UIDRef of the footnote
		@return Whether the focus is a footnote.
	 */
	virtual bool16 IsFocusAFootnote(const ITextFocus *pFocus, UIDRef* pFootnoteUIDRef = nil);

	/**
		returns whether the position in the model is a footnote and optionally returns the UIDRef for that footnote
	   @param model					--- The model in question
	   @param position				--- The position in the given model
	   @param pFootnoteUIDRef		---  If non-nil and there is a footnote at that model-position, the UIDRef of the footnote
		@return Whether that model-position is a footnote.
	 */
	virtual bool16 IsFootnote(const ITextModel* model, TextIndex position, UIDRef* pFootnoteUIDRef = nil);

	/**
		returns whether the range in the model contains a footnote
	   @param model					--- The model in question
	   @param range					--- The range in the given model
		@return Whether that model-range contains a footnote.
	 */
	virtual bool16 AnyFootnotes(const ITextModel* model, const RangeData& range);

	/**
		Populates the two passed in attribute boss lists with all the character attributes and paragraph attributes,
		respectively, that can exist in a style and therefore the attributes that can be "cleared".
		@param workspace			--- Workspace to use as the basis for collecting all possible "style" attributes.
		@param characterAttributes	--- AttributeBossList to populate with the complete list of character attributes that can be cleared (in a style)
		@param paragraphAttributes	--- AttributeBossList to populate with the complete list of paragraph attributes that can be cleared (in a style)
	 */
	virtual void GenerateListsOfAttributesThatCanBeCleared(IWorkspace * workspace, AttributeBossList * characterAttributes, AttributeBossList * paragraphAttributes);

	/** Calculate the range in the main story thread that ultimately owns the given range.
		e.g. Works to backtrack from a footnote or note or inline to the owning text index.
		@param point1 [IN/OUT] storyRef, the story that ultimately "owns" the given range.
		@param point2 [IN/OUT] range, the "owning" range of the original given range.
	*/
	virtual void GetPrimaryStoryAnchor(UIDRef& storyRef, RangeData& range) const;

	/** Filter illegal control characters from the string.
		@param data is the string to filter
		@param func is a function that returns kTrue if it is passed a character that should be stripped
		@return the filtered string or nil if no filtering was required
	*/
	typedef bool16 (*StripMatchingCharactersFunction)( const UTF32TextChar& n ) ;
	virtual	K2::shared_ptr<WideString> FilterInsertString( const WideString* data, StripMatchingCharactersFunction func ) const ;

	/** Calculate the range in the main story thread that owns the given range.
		within the same TextModel. Unlike GetPrimaryStoryAnchor() above, this
		method does NOT attempt to back out of inlines into a different story.
		@param model TextModel to work on
		@param range [IN/OUT] the "owning" range of the original given range.
	*/
	virtual void GetPrimaryThreadAnchor(const ITextModel* model, RangeData& range) const;

	/** Link two text frames together as follows:
		If prepend is kFalse, then the textFrameA will be linked to textFrameB. Thus, existing text will flow from textFrameA to textFrameB.
		If prepend is kTrue, then the textFrameB will be linked to textFrameA. Thus, existing text will flow from textFrameB to textFrameA.
		In both cases,  textFrameB will inherit textFrameA's attributes (e.g. vertical or horizontal frame).
		@param textFrameA - see desc above.
		@param textFrameB - see desc above.
		@param prepend - see desc above.
	*/
	virtual ErrorCode LinkTextFrames(const IMultiColumnTextFrame* textFrameA, const IMultiColumnTextFrame *textFrameB,  bool16 prepend);

	/** Convert the textFrame to a member of IHierarchy (even if the textFrame is Text-on-a-Path)
	*/
	virtual IHierarchy* QueryForHierarchyMember(IDataBase *db, const ITextFrameColumn *textFrame) const;

	/** Return the UIDRef for the page the given textFrame is on
	@param target  The textFrame or OwnedItem whose page number is desired.
	*/
	virtual UIDRef GetPageUIDRef(const UIDRef& textFrameRef) const;

	/** Return the page number for the page the given textFrame is on
	*/
	virtual int32 GetPageNumber(const UIDRef& textFrameRef) const;

	/** Return the number formatter for the page the given textFrame is on.  Formatter is also applicable for chapter numbers.
	*/
	virtual ClassID GetPageNumberFormatter(const UIDRef& textFrameRef) const;

	/** Return the formatted string for the current, previous, or next page the given textFrame is on.
	*/
	virtual void GetFormattedPageNumberString(const UIDRef& textFrameRef, PMString* s, IDrawingStyle::PositionIndicator whichPage = IDrawingStyle::kDataPosition_ThisPage) const;

	/** Return the formatted string for the section the given textFrame is on
	*/
	virtual void GetFormattedSectionNameString(const UIDRef& textFrameRef, PMString* s) const;

	/** Return the chapter number
	@param target The relevant text frame or an owned item
	*/
	virtual int32 GetChapterNumber(IDataBase* db) const;

	/** Return the chapter number as a string.
	@param target The relevant database
	@param s The returned formatted string
	@param v The optional value to format.  If v==kInvalidChapterNumber, the value is retrieved from GetChapterNumber.
	*/
	virtual void GetFormattedChapterNumberString(IDataBase* db, PMString* s, int32 v=kInvalidChapterNumber) const;

	
	/** Return the page count based on the given scope
	@param target The relevant text frame or an owned item
	*/
	virtual int32 GetPageCount(const UIDRef& target, enum PageCountScopes scope) const;

	/** Return the UIDRef for the page number previous to the one the given textFrame is on
	*/
	virtual UIDRef GetPreviousPageNumberFrameRef(const UIDRef& textFrameRef) const;

	/** Return the UIDRef for the page number after to the one the given textFrame is on
	*/
	virtual UIDRef GetNextPageNumberFrameRef(const UIDRef& textFrameRef) const;

	/** Return whether or not the container for the given target is continued from elsewhere (e.g. a table is continued from the previous frame)
	*/
	virtual bool16 GetTargetIsInContinuedContainer(const UIDRef& target) const;

	/** Return whether or not the container for the given target continues elsewhere (e.g. a table continues to another frame)
	*/
	virtual bool16 GetTargetContainerContinues(const UIDRef& target) const;

	/**
	*/
	virtual TextIndex GetTextIndexForTrackedParagraph(const UIDRef& paraStrandDiskPageRef,
												UID trackerUID,
												uint32 trackerData) const;

	/** Return the ICU Locale for the given target and position.
		If the target is a TextModel, then if the position is valid, it will return the LanguageID applied to that location.
		Otherwise, if target refers to a valid document, the default languageID for the document is returned.
		Otherwise, kLanguageNeutral is returned.
	@param target Preferably a document or a TextModel, but can be an empty UIDRef or any reference to the desired document.
	@param position The position in the model.  Optional.
	@return The ID of the language applied to the given location.
	*/
	virtual void GetLocaleFromTextIndex(const UIDRef& modelRef, const TextIndex position, bool16 &noLang, void *locale) const;

	/** Return the last document in the current active book and whether it was already opened or not.
	*/
	virtual IDocument* GetLastDocInBook(bool16& docWasAlreadyOpen, int32& startPageNum) const;

	/** Description: Fills in list of ITextFrameColumns known to the spread by
	calling the ISpreadMultiColumnFrames interface on the specified Spread.
	@param spreadRef UIDRef to spread to return TFCs from
	@param pList UIDList to fill
	@return Length of pList
	*/
	virtual int32 GetSpreadTextFrameColumns(const UIDRef& spreadRef,
												UIDList* pList) const;

	/** Description: Fills in list of IMultiColumnTextFrames known to the
	spread. by calling the ISpreadMultiColumnFrames interface on the specified
	Spread.
	@param spreadRef UIDRef to spread to return MCFs from
	@param pList UIDList to fill
	@return Length of pList
	*/
	virtual int32 GetSpreadMultiColumnTextFrames(const UIDRef& spreadRef,
												UIDList* pList) const;

	/** Description: Edit the given target style with the given style attributes.
	@param targetDB [IN] target database
	@param targetStyle [IN] target style
	@param attributesToApply [IN] the attributes in the target database to apply
	@param uidList [IN] itemList to apply
	*/
	virtual void EditTargetStyle(IDataBase* targetDB, const UID& targetStyle, AttributeBossList* attributesToApply, const UIDList& uidList) const;

	/** Description: Convert the attributes from the source database to target database first and 
	then apply the converted attributes to target style.
	@param sourceDB [IN] source database
	@param sourceStyle [IN] source style
	@param targetDB [IN] target database
	@param targetStyle [IN] target style
	@param uidList [IN] itemList to apply
	*/
	virtual void ApplySourceStyleAttrsToTargetStyle(IDataBase* sourceDB, const UID& sourceStyle, IDataBase* targetDB, const UID& targetStyle, 
												    const UIDList& uidList) const;

	/** Checks if the specified text is valid for insertion in the text model.
		Some control characters are not accepted by the text model.
		@see TextChar::IsIllegalControlChar().
		@param text [IN] text to be checked for validity.
		@return kTrue if the text is valid.
	*/
	virtual bool16 IsTextValidForInsertion(WideString const& text) const;

private:
};


CREATE_PMINTERFACE(CZExpTextUtils, kZExpTextUtilsImpl)

	CZExpTextUtils::CZExpTextUtils(IPMUnknown * boss)
	: inheritClass(boss)
{
	mDelegateIID = IID_IZEXPTEXTUTILS;
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpTextUtils::~CZExpTextUtils()
{

}

#pragma mark -


ICommand*
CZExpTextUtils::QueryCopyStoryCommand(
	const UIDRef& source, const UIDRef& dest, const StoryRange& sourceRange, const StoryRange& destRange, const bool16 ignoreAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->QueryCopyStoryCommand( source, dest, sourceRange, destRange, ignoreAttributes );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/** Function for copying text from one story to another.
	This copies the specified text from the source story to the end of the destination story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on copy, default to kFalse
	*/
ICommand*
CZExpTextUtils::QueryCopyStoryToEndCommand(
	const UIDRef& source, const UIDRef& dest, const StoryRange& sourceRange, const bool16 ignoreAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->QueryCopyStoryToEndCommand( source, dest, sourceRange, ignoreAttributes );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/** Function for copying all of the text from one story to another.
	This replaces the destination story with a copy of the source story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on copy, default to kFalse
	*/
ICommand*
CZExpTextUtils::QueryCopyStoryFromAllToAllCommand(
	const UIDRef& source, const UIDRef& dest, const bool16 ignoreAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->QueryCopyStoryFromAllToAllCommand( source, dest, ignoreAttributes );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/** Function for copying text from one story to another.
	This copies the entire source story to the end of the destination story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on copy, default to kFalse
	*/
ICommand*
CZExpTextUtils::QueryCopyStoryFromAllToEndCommand(
	const UIDRef& source, const UIDRef& dest, const bool16 ignoreAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->QueryCopyStoryFromAllToEndCommand( source, dest, ignoreAttributes );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/** Function for moving text from one story to another.
	This replaces the range in the destination story with the specified text from the source story.
	The source range is removed from the source story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on move, default to kFalse
	*/
ICommand*
CZExpTextUtils::QueryMoveStoryCommand(
	const UIDRef& source, const UIDRef& dest, const StoryRange& sourceRange, const StoryRange& destRange, const bool16 ignoreAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->QueryMoveStoryCommand( source, dest, sourceRange, destRange, ignoreAttributes );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/** Function for moving text from one story to another.
	The entire source story is moved to the destination range.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on move, default to kFalse
	*/
ICommand*
CZExpTextUtils::QueryMoveStoryFromAllCommand(
	const UIDRef& source, const UIDRef& dest, const StoryRange& destRange, const bool16 ignoreAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->QueryMoveStoryFromAllCommand( source, dest, destRange, ignoreAttributes );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/** Function for moving text from one story to another.
	This replaces the destination story with the source story.
	The source range is removed from the source story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on move, default to kFalse
	*/
ICommand*
CZExpTextUtils::QueryMoveStoryFromAllToAllCommand(
	const UIDRef& source, const UIDRef& dest, const bool16 ignoreAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->QueryMoveStoryFromAllToAllCommand( source, dest, ignoreAttributes );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/** Function for moving text from one story to another.
	This moves the entire source story to the end of the destination story.
	The source range is removed from the source story.
	@param source The UIDRef of the source ITextModel.
	@param dest The UIDRef of the destination ITextModel.
	@param ignoreAttributes Ignore the attributes on move, default to kFalse
	*/
ICommand*
CZExpTextUtils::QueryMoveStoryFromAllToEndCommand(
	const UIDRef& source, const UIDRef& dest, const bool16 ignoreAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->QueryMoveStoryFromAllToEndCommand( source, dest, ignoreAttributes );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/**
	Strip various types of characters from some text.
	@param rText is the source text.
	@param pStripped will be the converted text. If nil, nothing occurs and we return rText.
	@return Returns the converted text (pStripped) unless nil, then return rText.
	*/
const WideString&
CZExpTextUtils::ConvertTextModelText(
	const WideString &rText,
	WideString *pStripped,
	bool16 stripHardHyphens,
	bool16 stripDiscretionaryHyphens,
	bool16 convertApostrophes,
	bool16 stripNobreakNonspace,
	bool16 convertHyphens,
	bool16 stripLeadingSpaces)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		const WideString& toReturn = orgImpl->ConvertTextModelText( rText, pStripped, stripHardHyphens, stripDiscretionaryHyphens, convertApostrophes, stripNobreakNonspace, convertHyphens, stripLeadingSpaces );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	//return ;
}




	/**
	Collects UIDs of items with specified interface from the text focus. e.g. IID_IGRAPHICFRAMEDATA, IID_ITOPFRAMEDATA
	Can be a costly function with a text selection that spans many pages. See FocusSpansMultiplePageItems.
	*/
bool16
CZExpTextUtils::CollectPageItemsFromFocus(
	const ITextFocus *pFocus, const PMIID& iid, UIDList *pList)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->CollectPageItemsFromFocus( pFocus, iid, pList );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** Does the focus span more than one page item frames.
	*/
bool16
CZExpTextUtils::FocusSpansMultiplePageItems(
	const ITextFocus *pFocus)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->FocusSpansMultiplePageItems( pFocus );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** returns the index of a multicolumn frame in its thread
	*/
int32
CZExpTextUtils::GetMultiColFrameIndex(
	IPMUnknown* multiColFrame)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GetMultiColFrameIndex( multiColFrame );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/**
	Find the word at a text location.
	@param pModel The text model in question.
	@param nPosition The text location in question.
	@param pLength OUT the length of the word.
	@param aFunction a function that determines what characters are considered word breaking.
	@return the beginning location of the surrounding word.
	@see IComposeScanner.
	@see ILanguage.
	*/
TextIndex
CZExpTextUtils::FindSurroundingWord(
	const ITextModel *pModel, TextIndex nPosition, int32 *pLength, FuncIsWordChar aFunction)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		TextIndex toReturn = orgImpl->FindSurroundingWord( pModel, nPosition, pLength, aFunction );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( TextIndex )0;
}




	/**
	Find the word at a text location.
	@param pModel The text model in question.
	@param nPosition The text location in question.
	@param pLength OUT the length of the word.
	@param aFunction a function that determines what characters are considered word breaking.
	@return the beginning location of the surrounding word.
	@see IComposeScanner.
	@see ILanguage.
	*/
TextIndex
CZExpTextUtils::FindSurroundingWordUsingWordCharFuncOnly(
	const ITextModel *pModel, TextIndex nPosition, int32 *pLength, const FuncIsWordChar aFunction)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		TextIndex toReturn = orgImpl->FindSurroundingWordUsingWordCharFuncOnly( pModel, nPosition, pLength, aFunction );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( TextIndex )0;
}




	/** Is this style name considered plain? (For example, "Roman", "Regular", "")
	*/
bool16
CZExpTextUtils::IsPlainStyleName(
	const PMString &styleName)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsPlainStyleName( styleName );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** Is this style name considered italic?  (For example, "Italic", "Oblique")
	*/
bool16
CZExpTextUtils::IsItalicStyleName(
	const PMString &styleName)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsItalicStyleName( styleName );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** Is this style name considered bold?  (For example, "Bold", "Heavy", "Black")
	*/
bool16
CZExpTextUtils::IsBoldStyleName(
	const PMString &styleName)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsBoldStyleName( styleName );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** Is this style name considered bold and italic?
	*/
bool16
CZExpTextUtils::IsBoldItalicStyleName(
	const PMString &styleName)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsBoldItalicStyleName( styleName );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**
	used by ApplyTextAttrToWorkspaceCmd, UserApplyAttrCmd, and TextState.
	*/
bool16
CZExpTextUtils::GetFontStyleName(
	ITextAttrFont *fromFont, ITextAttrFont *toFont, PMString *styleName)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->GetFontStyleName( fromFont, toFont, styleName );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**
	Remap font styles tries to choose the expected family/style and allow roundtripping.
	*/
bool16
CZExpTextUtils::RemapFontStyles(
	IDataBase *dataBase, const AttributeBossList &stateOverrides, const AttributeBossList &newOverrides,
	PMString *styleName, ClassID textAttrFontUIDBoss, ClassID textAttrFontStyleBoss,
	bool16 remapExactFontStylesOnly)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->RemapFontStyles( dataBase, stateOverrides, newOverrides, styleName, textAttrFontUIDBoss, textAttrFontStyleBoss, remapExactFontStylesOnly );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**	Is this font an opentype font?
	*/
bool16
CZExpTextUtils::IsOpenTypeFont(
	const IPMFont *font)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsOpenTypeFont( font );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**	Is this font family an opentype font family?
	*/
bool16
CZExpTextUtils::IsOpenTypeFamily(
	IFontFamily *fontFamily)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsOpenTypeFamily( fontFamily );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	// Check the current char is a word break or not.
bool16
CZExpTextUtils::IsWordBreak(
	const UTF32TextChar& curChar)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsWordBreak( curChar );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**	Gets the display font names for font
		@param *fontGroup	font group for font
		@param *font		font to get name from
		@param &familyName	OUT
		@param &styleName	OUT
		@param namesFlag	See enum DisplayNamesFlag.
	 */
void
CZExpTextUtils::GetDisplayFontNames(
	IFontGroup *fontGroup, IPMFont *font, PMString &familyName, PMString &styleName, DisplayNamesFlag namesFlag)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetDisplayFontNames( fontGroup, font, familyName, styleName, namesFlag );
}





	/**	Gets the display font names for font
		@param *dataBase	database of family UID
		@param fontUID		family UID
		@param fontStyle	style of font
		@param &familyName	OUT
		@param &styleName	OUT
		@param namesFlag	See enum DisplayNamesFlag.
	 */
void
CZExpTextUtils::GetDisplayFontNames(
	IDataBase *dataBase, const ITextAttrUID* fontUID, const ITextAttrFont* fontStyle,
	PMString &familyName, PMString &styleName, DisplayNamesFlag namesFlag)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetDisplayFontNames( dataBase, fontUID, fontStyle, familyName, styleName, namesFlag );
}




	/**
	Returns the list of owned items associated with a TextRange
	@param model	The TextModel
	@param start	The starting TextIndex of Text range to collect from.
	@param len		The length of the Text range to collect from.
	@param resultList
	@param nested 	If kFalse, only the OwnedItems in the specified Text range
					are returned. If kTrue, then all nested OwnedItems, that
					is OwnedItems that are contained in StoryRanges managed
					by found OwnedItems, are also returned.
					See CollectStoryRanges() for more information on nesting.
	*/
void
CZExpTextUtils::CollectOwnedItems(
	const ITextModel* model,
	TextIndex start, int32 len,
	OwnedItemDataList* resultList,
	bool16 nested)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->CollectOwnedItems( model, start, len, resultList, nested );
}




	/**
	returns the list of owned items associated with a TextRange that have an IVisitorHelper interface.
	*/
void
CZExpTextUtils::CollectVisitableItems(
	ITextModel* model,
	TextIndex start, int32 len,
	VisitableItems *resultList)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->CollectVisitableItems( model, start, len, resultList );
}




	/**
	returns the child for each Inline UID in the owned item strand
	*/
void
CZExpTextUtils::GetUIDListOfInlines(
	IParcel* parcel, bool16 bWithDropCaps,
	UIDList* resultList)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetUIDListOfInlines( parcel, bWithDropCaps, resultList );
}




	/**
	Returns the list of *logically* sequential story ranges that derive
	from the specified span of the text model. For a typical model span
	with no special content this will simply return a single StoryRange
	as (start, start + len).
	If a table, or other type of owned item which contains nested text is
	within the specified span then the specified span will be returned as
	two ranges with the appropriate nested text range(s) inserted between
	them.
	The hIndex and hIndexList are optional parameters which allows the
	caller to identify the starting TextIndex of the hierarchical element
	that "owns" the equivalent range in the resultList. Ranges from the main
	story thread (which starts at TextIndex 0) will have the hIndex of
	kInvalidTextIndex. Hierarchical elements that are found in the main
	story thread will have hIndex values within the main story thread and
	if those elements have sub elements then those will be in other story
	threads.
	@param bIncludeAnchorSpan is optional parameter which provides flexibility
		to client if they want to include anchor when calculating net span.
		For example, we don't want to include anchor span(such as table)
		when when do line count, e.g., for a 4x4 table, we want the line count
		as 16 instead of 17.
	*/
void
CZExpTextUtils::CollectStoryRanges(
	const ITextModel* model,
	TextIndex start, int32 len,
	StoryRangeList* resultList,
	TextIndex hIndex,
	ITextUtils::HIndexList *hIndexList,
	bool16 bIncludeAnchorSpan)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->CollectStoryRanges( model, start, len, resultList, hIndex, hIndexList, bIncludeAnchorSpan );
}




	/**
	Builds a list of text ranges for the specified story that are in
	logical order. The first range will begin at 'start' and the rest of
	the ranges will wrap around the end of the story to end at 'start - 1'.
	*/
void
CZExpTextUtils::BuildLogicalStoryRangeList(
	ITextModel* model,
	TextIndex start,
	StoryRangeList* resultList)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->BuildLogicalStoryRangeList( model, start, resultList );
}




	/**
	Return the PMMatrix to transform a point in WaxCoordinates in a
	Parcel to the owning Frame. Useful if you are an inline and you want
	to find out where you are relative to the Frame.
	*/
PMMatrix
CZExpTextUtils::GetWaxToFrameMatrix(
	const IParcel* parcel)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		PMMatrix toReturn = orgImpl->GetWaxToFrameMatrix( parcel );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return  PMMatrix();
}




	/**
	Get the Content Bounds of the Parcel at the specified TextIndex.
	If there is no Parcel at that TextIndex then it returns kFalse. Note
	that if the text at that location isn't fully composed then the Parcel
	you get may not be the same Parcel after composition.
	*/
bool16
CZExpTextUtils::GetParcelContentBounds(
	ITextParcelList* tpl,
	TextIndex at, PMRect *pCBounds)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->GetParcelContentBounds( tpl, at, pCBounds );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** Is the text parcel list overset?
	@return returns kTrue if the specified ParcelList has any characters in addition
	to the final carriage return not mapped to a Parcel. If it's ONLY the
	final carriage return that is overset that it is NOT considered overset.
	*/
bool16
CZExpTextUtils::IsOverset(
	const IFrameList* fl)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsOverset( fl );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** returns false if writingScript and text are contradicting.
	*/
bool16
CZExpTextUtils::IsScriptOfText(
	const char *anyText, int32 writingScript)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsScriptOfText( anyText, writingScript );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** returns the best guess for the text's writing script, preferredWritingScript is being used as a hint.
	*/
int32
CZExpTextUtils::GuessScriptOfText(
	const char *anyText, int32 preferredWritingScript)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GuessScriptOfText( anyText, preferredWritingScript );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/** returns whether the char needs to be skipped during remapping.
	Used by Text::RemapCharacterValue.
	*/
bool16
CZExpTextUtils::SkipTextCharForRemapping(
	const UTF32TextChar& textChar)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->SkipTextCharForRemapping( textChar );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** returns whether the char needs to be skipped during remapping.
	Used by UserApplyAttrCmd, ScriptUserEditCmdResp, and CJKUserEditCmdResp.
	*/
bool16
CZExpTextUtils::SkipTextCharForFontLocking(
	const UTF32TextChar& textChar)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->SkipTextCharForFontLocking( textChar );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** returns the remapped textchar after converting to platform encoding using oldFont and back to newFont.
	*/
UTF32TextChar
CZExpTextUtils::RemapCharacterValue(
	IPMFont *oldFont, IPMFont *newFont, const UTF32TextChar& textChar)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UTF32TextChar toReturn = orgImpl->RemapCharacterValue( oldFont, newFont, textChar );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UTF32TextChar )0;
}




	/** Create a new story in the specified database.
	@param defaults specified whether the user's default attributes should be applied to the new story
	*/
UID
CZExpTextUtils::CreateNewStory(
	IDataBase *db, bool16 defaults)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UID toReturn = orgImpl->CreateNewStory( db, defaults );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UID )0;
}




	/** Writing script to Language ID conversion.
	*/
LanguageID
CZExpTextUtils::ScriptToLanguageID(
	int32 writingScript)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		LanguageID toReturn = orgImpl->ScriptToLanguageID( writingScript );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( LanguageID )0;
}




	/**
	Returns a command to force recomposition (use to avoid CmdBehaviorMonitor
	asserts if you need to force recomposition from within another command)
	*/
ICommand*
CZExpTextUtils::CreateRecomposeThruNthParcelCmd(
	const ITextParcelList* tpl, int32 parcelIndex)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->CreateRecomposeThruNthParcelCmd( tpl, parcelIndex );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/**
	Returns a command to force recomposition (use to avoid CmdBehaviorMonitor
	asserts if you need to force recomposition from within another command)
	*/
ICommand*
CZExpTextUtils::CreateRecomposeThruTextIndexCmd(
	const ITextParcelList* tpl, TextIndex textIndex)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ICommand* toReturn = orgImpl->CreateRecomposeThruTextIndexCmd( tpl, textIndex );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ICommand* )0;
}




	/**
	Determines the pair kern at a particular text index.
	@param kernService refers to the ClassID of a boss that has the IPairKernDict interface to use.
	@return this method returns an ErrorCode (kSuccess, kFailure, kCancel)
	*/
int32
CZExpTextUtils::DeterminePairKern(
	const ITextModel* model, TextIndex index, ClassID kernService, PMReal& result)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->DeterminePairKern( model, index, kernService, result );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




		/**
	Returns the requested attributes from the preferences of the document specified. prefsFromDocument should
	be non nil. For paragraph and character overrides only continuing attributes are returned.
	*/
void
CZExpTextUtils::GetDefaultTextAttrPreferences(
	const WhichTextAttrs which, TextAttrs& out, IDocument* prefsFromDocument)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetDefaultTextAttrPreferences( which, out, prefsFromDocument );
}




	/**
	2.0: Tables use one or more characters to "anchor" the table to the
	text flow. Inserting within these ranges is considered bad form and
	will lead to crashes. You may use this method to launder the ranges
	to encompass all of the table instead of just part.
	*/
void
CZExpTextUtils::AdjustForTableRange(
	const UIDRef& story, TextIndex* pStart, TextIndex* pEnd, TextIndex* nextLine)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->AdjustForTableRange( story, pStart, pEnd, nextLine );
}




	/**
	Align text in the primary story thread in the specified text frame. This
	is NOT suitable for alignment in any other TextStoryThread.
	*/
bool16
CZExpTextUtils::AlignToBaselineGrid(
	IGeometry* pageItem, IParcel* parcel,
	const IBaselineGridPrefs *gridPrefs,
	PMReal *pYTop, PMReal *pYBottom,
	Text::GridAlignmentMetric alignment,
	PMReal alignmentMetricOffset) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->AlignToBaselineGrid( pageItem, parcel, gridPrefs, pYTop, pYBottom, alignment, alignmentMetricOffset );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**
	Looks for findOldItemUI from "start" and replaces it with newItemUID in the item strand.
	This is typically used by commands that create owned items during import.
	In case the owned item is contained in a table and there is text after the end of the table (ie after
	the table in the tables containing text story thread) then the TextIndex stored in the command may not
	be valid. The returned TextIndex is the new location at which findOldItemUID was found in the item strand.
	*/
TextIndex
CZExpTextUtils::FindResetOwnedItemUID(
	IItemStrand* itemStrand, const ClassID objectType, const UID findOldItemUID, const TextIndex start, const UID newItemUID)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		TextIndex toReturn = orgImpl->FindResetOwnedItemUID( itemStrand, objectType, findOldItemUID, start, newItemUID );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( TextIndex )0;
}




	/**
	returns the position of first remapped textchar in buffer and copies the remapped textchars in  replacedChars after converting to platform encoding using oldFont and back to newFont.
	*/
int32
CZExpTextUtils::RemapCharacterValues(
	IPMFont *oldFont, IPMFont *newFont, const textchar * buffer,  int32 bufferLength, WideString *replacedChars)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->RemapCharacterValues( oldFont, newFont, buffer, bufferLength, replacedChars );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/** Counts number of Unicode characters in a UTF16TextChar buffer
	*/
int32
CZExpTextUtils::CountChars(
	const UTF16TextChar *buffer, int32 length) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->CountChars( buffer, length );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/**
	Translates character (can be multiple UTF16s) offset into UTF16 offset in UTF16TextChar buffer
	*/
int32
CZExpTextUtils::CharOffsetToUTF16Offset(
	const UTF16TextChar *buffer, int32 length, int32 charOffset) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->CharOffsetToUTF16Offset( buffer, length, charOffset );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}





	/**  GetSelectedTextItemsFromTextTarget gathers UIDs for the text frames that are currently selected.
		@param textTarget IN The current ITextTarget you want to gather selected text items from.
		@param textframes OUT The text frames that are currently selected.
		@param mcfs OUT The multi column frames that are currently selected.
		@param splineItems OUT The spline items that are currently selected.
	*/
void
CZExpTextUtils::GetSelectedTextItemsFromTextTarget(
	const ITextTarget * textTarget, UIDList * textframes, UIDList * mcfs, UIDList * splineItems)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetSelectedTextItemsFromTextTarget( textTarget, textframes, mcfs, splineItems );
}




	/**  GetSelectedTextItemsFromLayoutTarget gathers UIDs for the text frames that are currently selected.
		@param layoutTarget IN The current ILayoutTarget you want to gather selected text items from.
		@param textframes OUT The text frames that are currently selected.
		@param mcfs OUT The multi column frames that are currently selected.
		@param splineItems OUT The spline items that are currently selected.
	*/
void
CZExpTextUtils::GetSelectedTextItemsFromLayoutTarget(
	const ILayoutTarget * layoutTarget, UIDList * textframes, UIDList * mcfs, UIDList * splineItems)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetSelectedTextItemsFromLayoutTarget( layoutTarget, textframes, mcfs, splineItems );
}




	/**  GetSelectedTextItemsFromLayoutTarget gathers UIDs for the text frames that are currently selected.
		@param UIDList IN The pageitems to look through for text frames
		@param textframes OUT The text frames that are currently selected.
		@param mcfs OUT The multi column frames that are currently selected.
		@param splineItems OUT The spline items that are currently selected.
	*/
void
CZExpTextUtils::GetSelectedTextItemsFromUIDList(
	const UIDList& pageITems, UIDList * textframes, UIDList * mcfs, UIDList * splineItems)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetSelectedTextItemsFromUIDList( pageITems, textframes, mcfs, splineItems );
}




	/** GetTextFramesContaining returns the range of TextFrames that the
		Parcels containing the Text Range map to. The Range MUST be within
		a single StoryThread. pFirstFrameIndex will be -1 if the Text Range
		does not map to a Text Frame.
	*/
void
CZExpTextUtils::GetTextFramesContaining(
	const ITextModel *textModel,
	const RangeData& range,
	int32 *pFirstFrameIndex,
	int32 *pLastFrameIndex)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetTextFramesContaining( textModel, range, pFirstFrameIndex, pLastFrameIndex );
}




UIDList*
CZExpTextUtils::CreateOppositeDirectionFrameList (
	UIDList *pStoryList,
	ITextOptions::WritingDirection dir)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UIDList* toReturn = orgImpl->CreateOppositeDirectionFrameList ( pStoryList, dir );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UIDList* )0;
}




	/** GetUIDListOfTextFrames returns a list of TextFrames that the Parcels
		containing the specified Text Range map to. The Range MAY cross
		StoryThread boundries.
	*/
void
CZExpTextUtils::GetUIDListOfTextFramesFromRange(
	const ITextModel* textModel, const RangeData& range, UIDList* resultList)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetUIDListOfTextFramesFromRange( textModel, range, resultList );
}




	/**
	Fills in inkResourceData with the known inks in the specified Range. It incorporates
	handling of the last CR in the Story Thread, so callers need not take care of removing
	it from the range.

	Notes:
		1. This routine only examines Text attributes from a canned list and is currently not extensible.
		2. The caller is responsible for making sure that the specified range is fully composed.
	*/
void
CZExpTextUtils::GetUsedInks(
	const ITextModel* textModel, TextIndex threadEnd,
	TextIndex start, int32 len, IInkResourceData* inkResourceData)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetUsedInks( textModel, threadEnd, start, len, inkResourceData );
}




	/**
	Notifies the last Frame in the FrameList that its overset state may have
	changed. This call simply posts the appropriate inval rect for the location
	of the overset indicator and adds the Frame to the RecomposedFrames list.
	Note that the caller must ensure that at least one Frame in the FrameList
	is damaged or else the inval may never be played until the Frame becomes
	damaged and is recomposed.
	@param frameList
	*/
void
CZExpTextUtils::NotifyOversetChanged(
	const IFrameList* frameList)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->NotifyOversetChanged( frameList );
}




	/** Return whether or not the given object is a MultiColumn TextFrame
	@param obj can be any object
	*/
bool16
CZExpTextUtils::IsMultiColumnFrame(
	const IPMUnknown* obj) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsMultiColumnFrame( obj );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}



		
	/** Given an ITextFrameColumn, return the associated MultiColumn TextFrame.
	@param tfc Pointer to ITextFrameColumn
	*/
IMultiColumnTextFrame*
CZExpTextUtils::QueryMultiColumnFrame(
	ITextFrameColumn* tfc) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		IMultiColumnTextFrame* toReturn = orgImpl->QueryMultiColumnFrame( tfc );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( IMultiColumnTextFrame* )0;
}




	/** Const version of QueryMultiColumnFrame
	*/
const IMultiColumnTextFrame*
CZExpTextUtils::QueryMultiColumnFrame(
	const ITextFrameColumn* tfc) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		const IMultiColumnTextFrame* toReturn = orgImpl->QueryMultiColumnFrame( tfc );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( const IMultiColumnTextFrame* )0;
}




	/** UID version of QueryMultiColumnFrame
	*/
UID
CZExpTextUtils::QueryMultiColumnFrameUID(
	const ITextFrameColumn* tfc) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UID toReturn = orgImpl->QueryMultiColumnFrameUID( tfc );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UID )0;
}




	/** If the spline contains text, return the associated multi-column object.
	If spline doesn't contain text and if the spline has TOP, return the multi-column object associated with kTOPFrameItemBoss.
	return nil, for other conditions.
	*/
IMultiColumnTextFrame*
CZExpTextUtils::QueryMCFOrTOPFromSpline(
	const IGraphicFrameData* gfd) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		IMultiColumnTextFrame* toReturn = orgImpl->QueryMCFOrTOPFromSpline( gfd );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( IMultiColumnTextFrame* )0;
}



UID
CZExpTextUtils::GetMCFOrTOPFromSpline(
	const IGraphicFrameData* gfd) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UID toReturn = orgImpl->GetMCFOrTOPFromSpline( gfd );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UID )0;
}




	/** Given an IMultiColumnTextFrames, return the spline associated with it
	@param mcf Pointer to IMultiColumnTextFrame
	@return Pointer to IHierarchy
	*/
IHierarchy*
CZExpTextUtils::QuerySplineFromTextFrame(
	const IMultiColumnTextFrame* mcf) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		IHierarchy* toReturn = orgImpl->QuerySplineFromTextFrame( mcf );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( IHierarchy* )0;
}




	/** Given an ITextFrameColumn, return the spline associated with the parent MultiColumnTextFrame.
	@param tfc Pointer to ITextFrameColumn
	@return Pointer to IHierarchy
	*/
IHierarchy*
CZExpTextUtils::QuerySplineFromTextFrame(
	const ITextFrameColumn* tfc) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		IHierarchy* toReturn = orgImpl->QuerySplineFromTextFrame( tfc );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( IHierarchy* )0;
}




	/**
	*/
ErrorCode
CZExpTextUtils::ProcessScaleParcelContent(
	const ITextParcelList* tpl,
	ParcelKey key,
	const PMReal& xScale,
	const PMReal& yScale,
	IScaleObject::ScaleContext& context) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ErrorCode toReturn = orgImpl->ProcessScaleParcelContent( tpl, key, xScale, yScale, context );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ErrorCode )0;
}




	/**
		Returns the uid of the page which this text frame is one, can return kInvalidUID
		@param pageItem -- pageitem to find page UID of
		@return UID
	 */
UID
CZExpTextUtils::GetTextFramesPageItemUID(
	IGeometry* pageItem) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UID toReturn = orgImpl->GetTextFramesPageItemUID( pageItem );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UID )0;
}




	/**
	   Do character, word, paragraph and line count

	   @param startIndex		--- start index for counting
	   @param lengthToCount		--- how far should count stop.
	   @param action			--- just update line or update all.
	   @param state				--- start or continue counting.
	   @param wordStarted		--- left over state(word is started or not) of last time.
	   @param characters		--- count of characters. (passed out parameter)
	   @param words				--- count of words. (passed out parameter)
	   @param paragraphs		--- count of paragraphs. (passed out parameter)
	   @param lines				--- count of lines. (passed out parameter)
	   @param oversetCharacters	--- count of characters in overset. (passed out parameter)
	   @param oversetWords		--- count of words in overset. (passed out parameter)
	   @param oversetParagraphs	--- count of paragraphs in overset. (passed out parameter)
	   @param oversetLines		--- count of lines in overset. (passed out parameter)
    */
	
void
CZExpTextUtils::GetWordCountInfo(
	const ITextModel* iTextModel,
	const uint32& startIndex,
	const int32& lengthToCount,
	const ITextUtils::WordCountUpdateAction& action,
	const ITextUtils::WordCountUpdateState& state,
	bool16& wordStarted,
	uint32& characters,
	uint32& words,
	uint32& paragraphs,
	uint32& lines,
	uint32& oversetCharacters,
	uint32& oversetWords,
	uint32& oversetParagraphs,
	uint32& oversetLines,
	bool16& bWordAcrossOverset,
	TextStoryThreadHelper& helper)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetWordCountInfo( iTextModel, startIndex, lengthToCount, action, state, wordStarted, characters, words, paragraphs, lines, oversetCharacters, oversetWords, oversetParagraphs, oversetLines, bWordAcrossOverset, helper );
}




	/**
	   Do roman character, all character, katakana, hiragana, kanji, paragraph and line count

	   @param startIndex			--- start index for counting
	   @param lengthToCount			--- how far should count stop.
	   @param action				--- just update line or update all.
	   @param state					--- start or continue counting.
	   @param wordStarted		    --- left over state(word is started or not) of last time.
	   @param fullWidthChars		--- count of full-width chars including full-width numbers, symbols and punctuations. (passed out parameter)
	   @param oversetFullWidthChars	--- count of full-width chars including full-width numbers, symbols and punctuations in overset. (passed out parameter)
	   @param halfWidthChars		--- count of half-width chars including half-width numbers, symbols and punctuations. (passed out parameter)
	   @param oversetHalfWidthChars	--- count of half-width chars including half-width numbers, symbols and punctuations in overset. (passed out parameter)
	   @param japaneseChars         --- count of japanese chars including full and half-width Hiragana and Katakana. (passed out parameter)
	   @param oversetJapaneseChars  --- count of japanese chars including full and half-width Hiragana and Katakana in overset. (passed out parameter)
	   @param kanjis				--- count of kanjis. (passed out parameter)
	   @param oversetKanjis			--- count of kanjis in overset. (passed out parameter)
	   @param words					--- count of words. (passed out parameter)
	   @param oversetWords		    --- count of words in overset. (passed out parameter)
	   @param lines					--- count of lines. (passed out parameter)
	   @param oversetLines			--- count of lines in overset. (passed out parameter)
	   @param paragraphs			--- count of paragraphs. (passed out parameter)
	   @param oversetParagraphs		--- count of paragraphs in overset. (passed out parameter)
	   @param allChars				--- count of all characters(passed out parameter)
	   @param oversetAllChars       --- count of all characters in overset. (passed out parameter)
    */
void
CZExpTextUtils::GetWordCountInfo(
	const ITextModel* iTextModel,
	const uint32& startIndex,
	const int32& lengthToCount,
	const ITextUtils::WordCountUpdateAction& action,
	const ITextUtils::WordCountUpdateState& state,
	bool16&  wordStarted,
	uint32&  fullWidthChars, uint32& oversetFullWidthChars,
	uint32&  halfWidthChars, uint32& oversetHalfWidthChars,
	uint32&  japaneseChars,  uint32& oversetJapaneseChars,
	uint32&  kanjis,		 uint32& oversetKanjis,
	uint32&  words,          uint32& oversetWords,
	uint32&  lines,          uint32& oversetLines,
	uint32&  paragraphs,     uint32& oversetParagraphs,
	uint32&  allChars,       uint32& oversetAllChars,
	bool16&  bWordAcrossOverset,
	TextStoryThreadHelper& helper)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetWordCountInfo( iTextModel, startIndex, lengthToCount, action, state, wordStarted, fullWidthChars, oversetFullWidthChars, halfWidthChars, oversetHalfWidthChars, japaneseChars, oversetJapaneseChars, kanjis, oversetKanjis, words, oversetWords, lines, oversetLines, paragraphs, oversetParagraphs, allChars, oversetAllChars, bWordAcrossOverset, helper );
}





	/**
		return the number of overset lines in a given text model
	 */
int32
CZExpTextUtils::CountOversetLines(
	const UIDRef &textModelUIDRef)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->CountOversetLines( textModelUIDRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/**
		returns whether the given ITextFocus is a footnote and optionally returns the UIDRef for that footnote
	   @param pFocus					--- The ITextFocus in question
	   @param pFootnoteUIDRef		---  If non-nil and there is a footnote at that model-position, the UIDRef of the footnote
		@return Whether the focus is a footnote.
	 */
bool16
CZExpTextUtils::IsFocusAFootnote(
	const ITextFocus *pFocus, UIDRef* pFootnoteUIDRef)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsFocusAFootnote( pFocus, pFootnoteUIDRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**
		returns whether the position in the model is a footnote and optionally returns the UIDRef for that footnote
	   @param model					--- The model in question
	   @param position				--- The position in the given model
	   @param pFootnoteUIDRef		---  If non-nil and there is a footnote at that model-position, the UIDRef of the footnote
		@return Whether that model-position is a footnote.
	 */
bool16
CZExpTextUtils::IsFootnote(
	const ITextModel* model, TextIndex position, UIDRef* pFootnoteUIDRef)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsFootnote( model, position, pFootnoteUIDRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**
		returns whether the range in the model contains a footnote
	   @param model					--- The model in question
	   @param range					--- The range in the given model
		@return Whether that model-range contains a footnote.
	 */
bool16
CZExpTextUtils::AnyFootnotes(
	const ITextModel* model, const RangeData& range)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->AnyFootnotes( model, range );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**
		Populates the two passed in attribute boss lists with all the character attributes and paragraph attributes,
		respectively, that can exist in a style and therefore the attributes that can be "cleared".
		@param workspace			--- Workspace to use as the basis for collecting all possible "style" attributes.
		@param characterAttributes	--- AttributeBossList to populate with the complete list of character attributes that can be cleared (in a style)
		@param paragraphAttributes	--- AttributeBossList to populate with the complete list of paragraph attributes that can be cleared (in a style)
	 */
void
CZExpTextUtils::GenerateListsOfAttributesThatCanBeCleared(
	IWorkspace * workspace, AttributeBossList * characterAttributes, AttributeBossList * paragraphAttributes)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GenerateListsOfAttributesThatCanBeCleared( workspace, characterAttributes, paragraphAttributes );
}




	/** Calculate the range in the main story thread that ultimately owns the given range.
		e.g. Works to backtrack from a footnote or note or inline to the owning text index.
		@param point1 [IN/OUT] storyRef, the story that ultimately "owns" the given range.
		@param point2 [IN/OUT] range, the "owning" range of the original given range.
	*/
void
CZExpTextUtils::GetPrimaryStoryAnchor(
	UIDRef& storyRef, RangeData& range) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetPrimaryStoryAnchor( storyRef, range );
}




	/** Filter illegal control characters from the string.
		@param data is the string to filter
		@param func is a function that returns kTrue if it is passed a character that should be stripped
		@return the filtered string or nil if no filtering was required
	*/
K2::shared_ptr<WideString>
CZExpTextUtils::FilterInsertString(
	const WideString* data, StripMatchingCharactersFunction func) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		K2::shared_ptr<WideString> toReturn = orgImpl->FilterInsertString( data, func );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( K2::shared_ptr<WideString> )0;
}




	/** Calculate the range in the main story thread that owns the given range.
		within the same TextModel. Unlike GetPrimaryStoryAnchor() above, this
		method does NOT attempt to back out of inlines into a different story.
		@param model TextModel to work on
		@param range [IN/OUT] the "owning" range of the original given range.
	*/
void
CZExpTextUtils::GetPrimaryThreadAnchor(
	const ITextModel* model, RangeData& range) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetPrimaryThreadAnchor( model, range );
}




	/** Link two text frames together as follows:
		If prepend is kFalse, then the textFrameA will be linked to textFrameB. Thus, existing text will flow from textFrameA to textFrameB.
		If prepend is kTrue, then the textFrameB will be linked to textFrameA. Thus, existing text will flow from textFrameB to textFrameA.
		In both cases,  textFrameB will inherit textFrameA's attributes (e.g. vertical or horizontal frame).
		@param textFrameA - see desc above.
		@param textFrameB - see desc above.
		@param prepend - see desc above.
	*/
ErrorCode
CZExpTextUtils::LinkTextFrames(
	const IMultiColumnTextFrame* textFrameA, const IMultiColumnTextFrame *textFrameB,  bool16 prepend)
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ErrorCode toReturn = orgImpl->LinkTextFrames( textFrameA, textFrameB, prepend );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ErrorCode )0;
}




	/** Convert the textFrame to a member of IHierarchy (even if the textFrame is Text-on-a-Path)
	*/
IHierarchy*
CZExpTextUtils::QueryForHierarchyMember(
	IDataBase *db, const ITextFrameColumn *textFrame) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		IHierarchy* toReturn = orgImpl->QueryForHierarchyMember( db, textFrame );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( IHierarchy* )0;
}




	/** Return the UIDRef for the page the given textFrame is on
	@param target  The textFrame or OwnedItem whose page number is desired.
	*/
UIDRef
CZExpTextUtils::GetPageUIDRef(
	const UIDRef& textFrameRef) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UIDRef toReturn = orgImpl->GetPageUIDRef( textFrameRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UIDRef::gNull );
}




	/** Return the page number for the page the given textFrame is on
	*/
int32
CZExpTextUtils::GetPageNumber(
	const UIDRef& textFrameRef) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GetPageNumber( textFrameRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/** Return the number formatter for the page the given textFrame is on.  Formatter is also applicable for chapter numbers.
	*/
ClassID
CZExpTextUtils::GetPageNumberFormatter(
	const UIDRef& textFrameRef) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		ClassID toReturn = orgImpl->GetPageNumberFormatter( textFrameRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( ClassID )0;
}


	/** Return the formatted string for the current, previous, or next page the given textFrame is on.
	*/
void
CZExpTextUtils::GetFormattedPageNumberString(
	const UIDRef& textFrameRef, PMString* s, IDrawingStyle::PositionIndicator whichPage) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetFormattedPageNumberString( textFrameRef, s, whichPage );
}




	/** Return the formatted string for the section the given textFrame is on
	*/
void
CZExpTextUtils::GetFormattedSectionNameString(
	const UIDRef& textFrameRef, PMString* s) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetFormattedSectionNameString( textFrameRef, s );
}




	/** Return the chapter number
	@param target The relevant text frame or an owned item
	*/
int32
CZExpTextUtils::GetChapterNumber(
	IDataBase* db) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GetChapterNumber( db );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/** Return the chapter number as a string.
	@param target The relevant database
	@param s The returned formatted string
	@param v The optional value to format.  If v==kInvalidChapterNumber, the value is retrieved from GetChapterNumber.
	*/
void
CZExpTextUtils::GetFormattedChapterNumberString(
	IDataBase* db, PMString* s, int32 v) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetFormattedChapterNumberString( db, s, v );
}




	
	/** Return the page count based on the given scope
	@param target The relevant text frame or an owned item
	*/
int32
CZExpTextUtils::GetPageCount(
	const UIDRef& target, enum PageCountScopes scope) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GetPageCount( target, scope );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/** Return the UIDRef for the page number previous to the one the given textFrame is on
	*/
UIDRef
CZExpTextUtils::GetPreviousPageNumberFrameRef(
	const UIDRef& textFrameRef) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UIDRef toReturn = orgImpl->GetPreviousPageNumberFrameRef( textFrameRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UIDRef::gNull );
}




	/** Return the UIDRef for the page number after to the one the given textFrame is on
	*/
UIDRef
CZExpTextUtils::GetNextPageNumberFrameRef(
	const UIDRef& textFrameRef) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		UIDRef toReturn = orgImpl->GetNextPageNumberFrameRef( textFrameRef );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( UIDRef::gNull );
}




	/** Return whether or not the container for the given target is continued from elsewhere (e.g. a table is continued from the previous frame)
	*/
bool16
CZExpTextUtils::GetTargetIsInContinuedContainer(
	const UIDRef& target) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->GetTargetIsInContinuedContainer( target );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/** Return whether or not the container for the given target continues elsewhere (e.g. a table continues to another frame)
	*/
bool16
CZExpTextUtils::GetTargetContainerContinues(
	const UIDRef& target) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->GetTargetContainerContinues( target );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}




	/**
	*/
TextIndex
CZExpTextUtils::GetTextIndexForTrackedParagraph(
	const UIDRef& paraStrandDiskPageRef,
	UID trackerUID,
	uint32 trackerData) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		TextIndex toReturn = orgImpl->GetTextIndexForTrackedParagraph( paraStrandDiskPageRef, trackerUID, trackerData );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( TextIndex )0;
}




	/** Return the ICU Locale for the given target and position.
		If the target is a TextModel, then if the position is valid, it will return the LanguageID applied to that location.
		Otherwise, if target refers to a valid document, the default languageID for the document is returned.
		Otherwise, kLanguageNeutral is returned.
	@param target Preferably a document or a TextModel, but can be an empty UIDRef or any reference to the desired document.
	@param position The position in the model.  Optional.
	@return The ID of the language applied to the given location.
	*/
void
CZExpTextUtils::GetLocaleFromTextIndex(
	const UIDRef& modelRef, const TextIndex position, bool16 &noLang, void *locale) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->GetLocaleFromTextIndex( modelRef, position, noLang, locale );
}




	/** Return the last document in the current active book and whether it was already opened or not.
	*/
IDocument*
CZExpTextUtils::GetLastDocInBook(
	bool16& docWasAlreadyOpen, int32& startPageNum) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		IDocument* toReturn = orgImpl->GetLastDocInBook( docWasAlreadyOpen, startPageNum );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( IDocument* )0;
}




	/** Description: Fills in list of ITextFrameColumns known to the spread by
	calling the ISpreadMultiColumnFrames interface on the specified Spread.
	@param spreadRef UIDRef to spread to return TFCs from
	@param pList UIDList to fill
	@return Length of pList
	*/
int32
CZExpTextUtils::GetSpreadTextFrameColumns(
	const UIDRef& spreadRef,
	UIDList* pList) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GetSpreadTextFrameColumns( spreadRef, pList );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/** Description: Fills in list of IMultiColumnTextFrames known to the
	spread. by calling the ISpreadMultiColumnFrames interface on the specified
	Spread.
	@param spreadRef UIDRef to spread to return MCFs from
	@param pList UIDList to fill
	@return Length of pList
	*/
int32
CZExpTextUtils::GetSpreadMultiColumnTextFrames(
	const UIDRef& spreadRef,
	UIDList* pList) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		int32 toReturn = orgImpl->GetSpreadMultiColumnTextFrames( spreadRef, pList );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( int32 )0;
}




	/** Description: Edit the given target style with the given style attributes.
	@param targetDB [IN] target database
	@param targetStyle [IN] target style
	@param attributesToApply [IN] the attributes in the target database to apply
	@param uidList [IN] itemList to apply
	*/
void
CZExpTextUtils::EditTargetStyle(
	IDataBase* targetDB, const UID& targetStyle, AttributeBossList* attributesToApply, const UIDList& uidList) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->EditTargetStyle( targetDB, targetStyle, attributesToApply, uidList );
}




	/** Description: Convert the attributes from the source database to target database first and 
	then apply the converted attributes to target style.
	@param sourceDB [IN] source database
	@param sourceStyle [IN] source style
	@param targetDB [IN] target database
	@param targetStyle [IN] target style
	@param uidList [IN] itemList to apply
	*/
void
CZExpTextUtils::ApplySourceStyleAttrsToTargetStyle(
	IDataBase* sourceDB, const UID& sourceStyle, IDataBase* targetDB, const UID& targetStyle,
	const UIDList& uidList) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
		orgImpl->ApplySourceStyleAttrsToTargetStyle( sourceDB, sourceStyle, targetDB, targetStyle, uidList );
}




	/** Checks if the specified text is valid for insertion in the text model.
		Some control characters are not accepted by the text model.
		@see TextChar::IsIllegalControlChar().
		@param text [IN] text to be checked for validity.
		@return kTrue if the text is valid.
	*/
bool16
CZExpTextUtils::IsTextValidForInsertion(
	WideString const& text) const
{
	LogFunctionEnterExit;
	InterfacePtr<ITextUtils> orgImpl( this, mDelegateIID );
	if( orgImpl )
	{
		bool16 toReturn = orgImpl->IsTextValidForInsertion( text );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
		return toReturn;
	}
	return ( bool16 )0;
}



