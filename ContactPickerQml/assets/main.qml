// Default empty project template
import bb.cascades 1.0
import bb.cascades.pickers 1.0
//import bb.pim.contacts 1.0

// creates one page with a label

Page {
    attachedObjects: [
        ContactPicker {
            id: contactPicker
            objectName: "contactPicker"
            // hadnle selected item in ContactPicker if SelectionMode is single item
            onContactSelected: {
                contactsListModel.clear();
                var newItem = cppManager.contactInfo(contactId);
                contactsListModel.insert(newItem);
            }
            // hadnle selected item in ContactPicker if SelectionMode is multiple items
            onContactsSelected: {
                contactsListModel.clear();
                // retrieve selected contacts information and populate it in ListView model
                for (var i = 0; i < contactIds.length; ++ i) {
                    var newItem = cppManager.contactInfo(contactIds[i]);
                    contactsListModel.insert(newItem);
                }
            }
            // hadnle selected item in ContactPicker if SelectionMode is attribute.
            onContactAttributeSelected: {
                contactsListModel.clear();
                // get contact information from SelectedContactAttribute and populate it in contact list
                var contactInfo = cppManager.contactInfo(attribute);
                contactsListModel.insert(contactInfo);
                // get  selected contact attribute data
                var contactAttr = cppManager.selectedContactAttributeInfo(attribute);
                selectedAttrKind.text = contactAttr["attrKind"];
                selectedAttrSubkind.text = contactAttr["attrSubKind"];
                selectedAttrValue.text = contactAttr["value"];
            }
        }
    ]
    actions: [
        ActionItem {
            title: "Show contacts"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                // hide contact attributes container if ContactPicker selection mode is
                // different from ContactSelectionMode.Attribute
                contactAttributesPane.visible = selectionMode.selectedIndex == 2;
                contactAttrributesDivider.visible = selectionMode.selectedIndex == 2;
                
                // set ContactPicker filter by contact attribute. This filter is respected only if
                // ContactSelectionMode.Attribute is set in selection mode list
                cppManager.setContactFilterAttrKind(selectionAttrKind.selectedValue);
                // set ContactPicker selection mode
                contactPicker.mode = selectionMode.selectedValue;
                // set contact filter by account if a concrete account is selected by an user
                if (selectionAccount.selectedIndex != 0) {
                    contactPicker.account = selectionAccount.selectedValue;
                }
                // show ContactPicker
                contactPicker.open();
            }// end if action item triggered singla handler 
        }// end of ActionItem for opening ContactPicker
    ]
    onCreationCompleted: {
        var count = cppManager.accountsCount();
        for (var i = 0; i < count; ++ i) {
            var newOption = cppManager.accountInfoOption(i);
            selectionAccount.add(newOption);
        }
    }
    titleBar: TitleBar {
        title: "ContactPicker parameters"
        visibility: ChromeVisibility.Visible
    }
    Container {
        layout: StackLayout {
        }
        topPadding: 10.0
        DropDown {
            id: selectionMode
            title: "Contact selection mode"
            horizontalAlignment: HorizontalAlignment.Center
            options: [
                Option {
                    text: "single"
                    value: ContactSelectionMode.Single
                    selected: true
                },
                Option {
                    text: "multiple"
                    value: ContactSelectionMode.Multiple
                },
                Option {
                    text: "attribute"
                    value: ContactSelectionMode.Attribute
                }
            ]
        } // end of contact selection mode DropDown list
        DropDown {
            id: selectionAttrKind
            title: "Contact filter by attribute"
            horizontalAlignment: HorizontalAlignment.Center
            options: [
                Option {
                    text: "Name"
                    value: AttributeKind.Name
                    selected: true
                },
                Option {
                    text: "Phone"
                    value: AttributeKind.Phone
                },
                Option {
                    text: "Email"
                    value: AttributeKind.Email
                },
                Option {
                    text: "Website "
                    value: AttributeKind.Website
                },
                Option {
                    text: "InstantMessaging"
                    value: AttributeKind.InstantMessaging
                }
            ]
        } // end of contact AttributeKind filter DropDown list
        DropDown {
            id: selectionAccount
            title: "Contact filter by account"
            horizontalAlignment: HorizontalAlignment.Center
        } // end of contact account filter DropDown list
        Divider {
            id: contactAttrributesDivider
            visible: false
        }
        Container {
            id: contactAttributesPane
            visible: false
            layout: StackLayout {
            }
            Header {
                id: contactAttributeHeader
                title: "Selected contact attributes:"
            }
            Container {
                layout: DockLayout { }
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Attribute kind"
                }
                Label {
                    id: selectedAttrKind
                    horizontalAlignment: HorizontalAlignment.Right
                    text: "no attribute selected"
                }
            }
            Container {
                layout: DockLayout { }
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Attribute sub kind"
                }
                Label {
                    id: selectedAttrSubkind
                    horizontalAlignment: HorizontalAlignment.Right
                    text: "no attribute selected"
                }
            }
            Container {
                layout: DockLayout { }
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Attribute value"
                }
                Label {
                    id: selectedAttrValue
                    horizontalAlignment: HorizontalAlignment.Right
                    text: "no attribute selected"
                 }
            }
        }// end of contact attributes container
        Divider {}
        Container {
            id: contactListPane
            layout: StackLayout{}
	        Header {
	            title: "Availabe Contacts in " + selectionAccount.selectedOption.text
	        }
	        ListView {
	            id: contactsList
	            dataModel: GroupDataModel {
	                id: contactsListModel
	                grouping: ItemGrouping.ByFirstChar
	                sortingKeys: ["name"]
	            }
	            listItemComponents: [
	                ListItemComponent {
	                    type: "item"
	                    StandardListItem {
	                        title: ListItemData.name
	                        description: ListItemData.account
	                        status: ListItemData.provider
	                        imageSource: ListItemData.photo
	                    }// end of ListView item definition
	                }// end of ListItemComponent
	            ]// end of ListView item definitions
	        }// end of contacts ListView
	    }//end of contact list container
    }// end of page content container
}// end of Page definition
