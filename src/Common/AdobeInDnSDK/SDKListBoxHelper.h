//========================================================================================
//  
//  $File: //depot/indesign_4.0/gm/source/sdksamples/common/SDKListBoxHelper.h $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: rajkumar.sehrawat $
//  
//  $DateTime: 2005/03/08 13:31:35 $
//  
//  $Revision: 1288 $
//  
//  $Change: 323509 $
//  
//  Copyright 1997-2005 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#ifndef __SDKListBoxHelper_h__
#define __SDKListBoxHelper_h__

class IPMUnknown;
class IControlView;

/**
	Sample code that hides some of the detail of working with the list-box API.
	The list box could be on a panel or a dialog.

	
*/
class SDKListBoxHelper
{
public:
	/**
		Constructor.
		@param fOwner reference to boss object using this helper.
		@param pluginID plug-in where resources can be found
		@param listBoxID the list box widget ID
		@param owningWidgetID could be a panel or a dialog this listbox is on
	*/
	SDKListBoxHelper(IPMUnknown* fOwner, const PluginID& pluginID, const WidgetID& listBoxID, const WidgetID& owningWidgetID);
	virtual ~SDKListBoxHelper();

	/**
		Add an element with the specified name at a given location in the listbox.
		@param displayName string value
		@param updateWidgetId the text widget ID within the cell
		@param atIndex specify the location, by default, the end of the list-box.
	*/
	void AddElement( const PMString & displayName,   WidgetID updateWidgetId, int atIndex = -2 /* kEnd */);

	/**
		Method to remove a list-box element by position.
		@param indexRemove position of element to remove.
	*/
	void RemoveElementAt(int indexRemove);
	
	/**
		Method to remove the last element in the list-box.
	*/
	void RemoveLastElement();

	/**
		Query for the list-box on the panel or a dialog that is currently visible, assumes one visible at a time.
		@return reference to the current listbox, not add-ref'd.
	*/
	IControlView * FindCurrentListBox();

	/**
		Method to delete all the elements in the list-box.
	*/
	void EmptyCurrentListBox();

	/**
		Accessor for the number of elements in encapsulated list-box.
	*/
	int GetElementCount();
	
private:
	// impl methods
	bool16 verifyState() { return (fOwner!=nil) ? kTrue : kFalse; }
	/**
		helper method to a new list element widget.
	*/
	void AddListElementWidget(InterfacePtr<IControlView> & elView, const PMString & displayName, WidgetID updateWidgetId, int atIndex);

	/**
		Helper method to remove the specified index.
	*/
	void removeCellWidget(IControlView * listBox, int removeIndex);

	IPMUnknown * fOwner;
	PluginID fOwningPluginID;
	WidgetID fListboxID;
	WidgetID fOwningWidgetID;
};


#endif // __SDKListBoxHelper_h__




