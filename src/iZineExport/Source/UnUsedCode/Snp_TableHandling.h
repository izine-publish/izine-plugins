void				AddAllTablesInStory(
							IXMLOutStream *				ioStream,
							const UIDRef &				inTextModelRef) const;
	
//----------------------------------------------------------------------------------------
// AddAllTablesInStory
//----------------------------------------------------------------------------------------
void				
CZExpXMLWriteFacade::AddAllTablesInStory(
						IXMLOutStream *				ioStream,
						const UIDRef &				inTextModelRef) const
{
	ASSERT( ioStream );
	InterfacePtr<const ITextModel> textModel( inTextModelRef, UseDefaultIID() );
	if( !textModel )
		return;

	InterfacePtr<ITableModelList> tableModelList(textModel, UseDefaultIID());
	ASSERT(tableModelList);
	if(tableModelList == nil) 
		return;

	for(int32 i = 0; i < tableModelList->GetModelCount(); i++) 
	{
		InterfacePtr<ITableModel> tableModel(tableModelList->QueryNthModel(i));
		ASSERT(tableModel);
		if(tableModel == nil)
			continue;
		
		InterfacePtr<ITableTextContainer> tableTextContainer(tableModel, UseDefaultIID());
		ASSERT(tableTextContainer);
		if(!tableTextContainer)
			break;
		
		//Index where the table lies in text
		TextIndex  whereTextIndex = tableTextContainer->GetAnchorTextIndex();

		RowRange headerRowRange = tableModel->GetHeaderRows();
		RowRange bodyRows = tableModel->GetBodyRows();
		RowRange footerRows = tableModel->GetFooterRows();
		RowRange totalRows = tableModel->GetTotalRows();
		ColRange totalCols = tableModel->GetTotalCols();
		
		InterfacePtr<ITableGeometry> tableGeometry(tableModel, ITableGeometry::kDefaultIID);

		ITableModel::const_iterator iterTable(tableModel->begin());
		ITableModel::const_iterator end(tableModel->end());
		
		while (iterTable != end)
		{
			GridAddress gridAddress = *iterTable;
			const GridID gridID = tableModel->GetGridID(gridAddress);
			WideString text;
			do
			{
				if (gridID == kInvalidGridID) 
					break;

				InterfacePtr<ITextStoryThreadDict> textStoryThreadDict(tableModel, UseDefaultIID());
				ASSERT(textStoryThreadDict);
				if (textStoryThreadDict == nil) 
					break;

				InterfacePtr<ITextStoryThread> textStoryThread(textStoryThreadDict->QueryThread(gridID));
				ASSERT(textStoryThread);
				if (textStoryThread == nil)
					break;
				
				InterfacePtr<ITextModel> textModel(textStoryThread->QueryTextModel());
				if (textModel == nil)
					break;
				
				int32 span=(-1);
				TextIndex at = textStoryThread->GetTextStart(&span);
				TextIterator beginTextChunk(textModel, at);
				TextIterator endTextChunk(textModel, at + span);
				
				std::copy(beginTextChunk, endTextChunk, std::back_inserter(text));
			
			}while (false);

			// goto to the next grid address
			++iterTable;
		}
	}
}
