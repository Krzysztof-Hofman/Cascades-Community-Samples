// Default empty project template
#include "ContactPickerQml.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>

#include <qdebug.h>

using namespace bb::cascades;
using namespace bb::pim::contacts;
using namespace bb::pim::account;

ContactPickerQml::ContactPickerQml(bb::cascades::Application *app)
: QObject(app)
{
	 // Get accounts info from platform and save it in a field for exposing it to Qml.
	// It has to be dome before laoding Qml file as the data must exist before Qml file is created
	loadAccountsInfo();

	qmlRegisterType<bb::cascades::pickers::ContactPicker>("bb.cascades.pickers", 1, 0, "ContactPicker");
	qmlRegisterType<bb::cascades::Option>("bb.cascades.", 1, 0, "Option");

	qmlRegisterUncreatableType<bb::cascades::pickers::ContactSelectionMode>("bb.cascades.pickers", 1, 0,
							"ContactSelectionMode", "Cannot register ContactSelectionMode type in QML");

	// create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // expose this object to Qml
    qml->setContextProperty("cppManager", this);

    // Because AttributeKind class cannot b e exposed to QML its enum values is exposed via QDeclarativePropertyMap
    exposeAttributeKindsToQml(qml);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);

    mContactPicker = root->findChild<pickers::ContactPicker*>("contactPicker");

}

void ContactPickerQml::loadAccountsInfo() {
	AccountService accountService;
	QList<Account> accountList = accountService.accounts(/*Service::Contacts*/);
	QVariantMap accountInfo;

	// add account data reffered to every account on device.
	// It is for disabling account filtering in cohtacts
	mAccountsData.reserve(accountList.size()+1);
	accountInfo.insert("Name", QVariant::fromValue<QString>("Any"));
	accountInfo.insert("id", QVariant::fromValue<int>(-1));
	mAccountsData.push_back(accountInfo);

	QString accountName;
	foreach(const Account & account, accountList) {
		accountName = account.displayName();
		if(accountName.isEmpty()) {
			accountInfo.insert("Name", QVariant::fromValue<QString>(account.provider().name()));
		}
		else {
			accountInfo.insert("Name", QVariant::fromValue<QString>(account.displayName()));
		}
		accountInfo.insert("id", QVariant::fromValue<int>(account.id()));
		qDebug() << "Account name: " << account.displayName() << ", account probider: " << account.provider().name();
		mAccountsData.push_back(accountInfo);
	}
}

int ContactPickerQml::accountsCount() {
	return mAccountsData.size();
}

QVariantMap ContactPickerQml::accountInfo(int index) {
	return mAccountsData[index];
}

bb::cascades::Option* ContactPickerQml::accountInfoOption(int index) {
	Option* newOption = Option::create().
								text(mAccountsData[index].value("Name").toString()).
								value(mAccountsData[index].value("id").toInt());
	if(index == 0) {
		newOption->setSelected(true);
	}
	return newOption;
}

QVariantMap ContactPickerQml::contactInfo(int contactId) {
	QVariantMap contactData;
	Contact contact = ContactService().contactDetails(contactId);
	AccountService accountService;
	Account account = accountService.account(contact.accountId());

	contactData["name"] = contact.displayName();
	contactData["account"] = account.displayName();
	contactData["provider"] = account.provider().name();

	QString contactPhoto = contact.smallPhotoFilepath();

	contactData["photo"] = contactPhoto.isEmpty() ? "images/contact.png" : contactPhoto;

	return contactData;
}

QVariantMap ContactPickerQml::selectedContactAttributeInfo(const pickers::SelectedContactAttribute& attr) {
	QVariantMap info;
	info["attrKind"] = attributeKindToString(attr.attributeKind());
	info["attrSubKind"] = attributeSubKindToString(attr.attributeSubKind());
	info["value"] = attr.value();
	return info;
}

QVariantMap ContactPickerQml::contactInfo(const pickers::SelectedContactAttribute& attr) {
	return contactInfo(attr.contactId());
}

void ContactPickerQml::exposeAttributeKindsToQml(bb::cascades::QmlDocument* qml) {
	QDeclarativePropertyMap* attrPropertyMap = new QDeclarativePropertyMap(this);
	attrPropertyMap->insert("Name", QVariant::fromValue<int>(AttributeKind::Name));
	attrPropertyMap->insert("Phone", QVariant::fromValue<int>(AttributeKind::Phone));
	attrPropertyMap->insert("Email", QVariant::fromValue<int>(AttributeKind::Email));
	attrPropertyMap->insert("Website", QVariant::fromValue<int>(AttributeKind::Website));
	attrPropertyMap->insert("InstantMessaging ", QVariant::fromValue<int>(AttributeKind::InstantMessaging ));

	qml->setContextProperty("AttributeKind", attrPropertyMap);
}

void ContactPickerQml::setContactFilterAttrKind(int attrKind) {
	if(attrKind == AttributeKind::Name) {
		mContactPicker->setKindFilters(QSet<AttributeKind::Type>() << AttributeKind::Name);
	}
	else if(attrKind == AttributeKind::Phone) {
		mContactPicker->setKindFilters(QSet<AttributeKind::Type>() << AttributeKind::Phone);
	}
	else if(attrKind == AttributeKind::Website) {
		mContactPicker->setKindFilters(QSet<AttributeKind::Type>() << AttributeKind::Website);
	}
	else if(attrKind == AttributeKind::InstantMessaging) {
		mContactPicker->setKindFilters(QSet<AttributeKind::Type>() << AttributeKind::InstantMessaging);
	}
}


QString ContactPickerQml::attributeKindToString(const AttributeKind::Type & kind) {
	switch(kind) {
	case AttributeKind::Phone:
		return "Phone";
	case AttributeKind::Email:
		return "Email";
	case AttributeKind::Website:
		return "Website";
	case AttributeKind::Name:
		return "Name";
	default:
		return "Unhandled attribute kind: ";
	}
}


QString ContactPickerQml::attributeSubKindToString(const AttributeSubKind::Type & subKind) {
	switch(subKind) {
	case AttributeSubKind::Other:
		return "Other";
	case AttributeSubKind::Home:
		return "Home";
	case AttributeSubKind::Work:
		return "Work";
	case AttributeSubKind::PhoneMobile:
		return "PhoneMobile";
	case AttributeSubKind::Blog:
		return "Blog";
	case AttributeSubKind::WebsitePortfolio:
		return "Protfolio";
	case AttributeSubKind::WebsitePersonal:
		return "Personal";
	case AttributeSubKind::WebsiteCompany:
		return "Company";
	case AttributeSubKind::ProfileFacebook:
		return "Portfolio Facebook";
	case AttributeSubKind::NameSurname:
		return "Surname";
	case AttributeSubKind::Title:
		return "Title";
	case AttributeSubKind::NameSuffix:
		return "Name suffix";
	case AttributeSubKind::NameMiddle:
		return "Middle name";
	case AttributeSubKind::NameNickname:
		return "Nickname";
	case AttributeSubKind::NameAlias:
		return "Alias";
	case AttributeSubKind::NameDisplayName:
		return "Display name";
	case AttributeSubKind::InstantMessagingBbmPin:
		return "BBM pin";
	case AttributeSubKind::InstantMessagingAim:
		return "Aim";
	case AttributeSubKind::InstantMessagingAliwangwang:
		return "Wangwang";
	case AttributeSubKind::InstantMessagingGoogleTalk:
		return "GoogleTalk";
	case AttributeSubKind::InstantMessagingSametime:
		return "Sametime";
	case AttributeSubKind::InstantMessagingIcq:
		return "Irq";
	case AttributeSubKind::InstantMessagingIrc:
		return "Irc";
	case AttributeSubKind::InstantMessagingJabber:
		return "Jabber";
	case AttributeSubKind::InstantMessagingMsLcs:
		return "MsLcs";
	case AttributeSubKind::InstantMessagingMsn:
		return "MSN Messenger";
	case AttributeSubKind::InstantMessagingQq:
		return "Qq";
	case AttributeSubKind::InstantMessagingSkype:
		return "Skypoe";
	case AttributeSubKind::InstantMessagingYahooMessenger:
		return "Yahoo messenger";
	case AttributeSubKind::InstantMessagingYahooMessengerJapan:
		return "Yahoo messenger Japan";
	default:
		return "Unhandled attribute sub kind£ ";
	}
}

