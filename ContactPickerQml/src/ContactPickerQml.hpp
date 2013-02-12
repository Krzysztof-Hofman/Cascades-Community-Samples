// Default empty project template
#ifndef ContactPickerQml_HPP_
#define ContactPickerQml_HPP_

#include <QObject>
#include <Qvariant>
#include <bb/cascades/pickers/ContactPicker>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Option>

#include <bb/cascades/pickers/ContactSelectionMode>

#include <bb/pim/account/AccountService>
#include <bb/pim/account/AccountService>
#include <bb/pim/account/Provider>

#include <bb/pim/contacts/Contact>
#include <bb/pim/contacts/ContactService>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ContactPickerQml : public QObject
{
    Q_OBJECT
public:
    ContactPickerQml(bb::cascades::Application *app);
    virtual ~ContactPickerQml() {}

    /**
     * Sets ContactPicker attribute filter if SelectionMode.Attribute is selected by the user.
     * Parameters:
     * 	attrKind - integer value representing AttributeKind::Type
     */
    Q_INVOKABLE void setContactFilterAttrKind(int attrKind);
    /** Returns number of Account objects handling Service::Contacts */
    Q_INVOKABLE int accountsCount();
    /**
     * Returns Account information (id, name, or its Provider name if Account name is empty).
     * Parameters:
     * 	index - Account index in array of Accounts
     * 	Returns:
     * 	Account information as QVariantMap to populate it.
     */
    Q_INVOKABLE QVariantMap accountInfo(int index);
    /**
     * Creates and returns bb::cascades::Option object which represents
     * Account information in DropDon control
     * Parameters:
     * 	index - Account index in array
     * Returns:
     * 	bb::cascades::Option object
     */
    Q_INVOKABLE bb::cascades::Option* accountInfoOption(int index);
    /**
     * Populates Contact information (photo, displayName, Account and Account Provider).
     * Parameters:
     * 	contactId - Contact identifier in system databse
     * 	Returns:
     * 	Contact information
     */
    Q_INVOKABLE QVariantMap contactInfo(int contactId);
    /**
     * Populates Contact information from SelectedContactAttribute received from ContactPicker
     * opened in SelectionMode::Attribute.
     * Parameters:
     * 	attr - SelectedContactAttribute object passed in ContactPicker contactAttributeSelected signal
     * 	Returns:
     * 	Contacdt information in the same format as in contactInfo(int contactId) method.
     */
    Q_INVOKABLE QVariantMap contactInfo(const bb::cascades::pickers::SelectedContactAttribute & attr);
    /**
     * Populates Contact's selected attribute in ContactPicker. The attribute includes AttributeKind::Type,
     * AttributeSubkind::Type and Attribute value.
     * Parameters:
     * 	attr - SelectedContactAttribute object passed in ContactPicker contactAttributeSelected signal
     * Returns:
     * 	Selected Contact attribute
     */
    Q_INVOKABLE QVariantMap selectedContactAttributeInfo(const bb::cascades::pickers::SelectedContactAttribute & attr);

private:
    /**
     * Scans device for Accounts handling Service::Contacts. Accounts founded are stored in a field for later use.
     *:This method is called before loading Qml file to provide Accounts information in Qml.
     */
    void loadAccountsInfo();
    /**
     * Exposes AttributeKind enum values to Qml.
     * Parameters:
     * 	qml - QmlDocumet to which AttrbuteKind values are to be exposed
     */
    void exposeAttributeKindsToQml(bb::cascades::QmlDocument* qml);
    /**
     * Converts AttributeKind::Type value to QString which can be populated in Qml.
     * Parameters:
     * 	kind - AttributeKind::Type to be converted
   */
    QString attributeKindToString(const bb::pim::contacts::AttributeKind::Type & kind);
    /**
	 * Converts AttributeSubKind::Type value to QString which can be populated in Qml.
	 * Parameters:
	 * 	kind - AttributeSubKind::Type to be converted
   */
    QString attributeSubKindToString(const bb::pim::contacts::AttributeSubKind::Type & subKind);

private:
    /** Pointer to ContactPicker instance created in Qml */
    bb::cascades::pickers::ContactPicker* mContactPicker;
    /** List of Accounts information for populating in Qml */
    QList<QVariantMap> mAccountsData;
};


#endif /* ContactPickerQml_HPP_ */
