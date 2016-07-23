#define MIME_SHORT_DESCR "Person"
#define MIME_LONG_DESCR "Personal and Work Contact Information"

#define APP_TITLE_TEXT APP_NAME" "APP_VERSION" - New Person"

// définitions des labels des champs MIME
// (utilisés dans FileTypes / Tracker / QueryApp / People)

// général
#define VIEW_COMPAT_NAME		"Complete name"
#define VIEW_FIRST_NAME			"First name"
#define VIEW_LAST_NAME			"Last name"
#define VIEW_NICKNAME			"Nickname"
#define VIEW_TITLE				"Title"
#define VIEW_GROUPNAME			"Group"
#define VIEW_GROUP_NONE			"none"


// home stuff
#define VIEW_HOME_ADDRESS		"Street"
#define VIEW_HOME_ADDRESS2		"Street(2)"
#define VIEW_HOME_CITY			"City"
#define VIEW_HOME_STATE			"State"
#define VIEW_HOME_ZIP_CODE		"Zip code"
#define VIEW_HOME_COUNTRY		"Country"
#define VIEW_HOME_PHONE			"Phone"
#define VIEW_HOME_FAX			"FAX"
#define VIEW_HOME_CELL_PHONE	"Cell Phone"
// no home pager
#define VIEW_HOME_EMAIL			"E-mail"
#define VIEW_HOME_URL			"URL"


// work stuff
#define VIEW_WORK_COMPANY_NAME	"Company"
#define VIEW_WORK_POSITION		"Position"

#define VIEW_WORK_ADDRESS		"Street"
#define VIEW_WORK_ADDRESS2		"Street(2)"
#define VIEW_WORK_CITY			"City"
#define VIEW_WORK_STATE			"State"
#define VIEW_WORK_ZIP_CODE		"Zip Code"
#define VIEW_WORK_COUNTRY		"Country"
#define VIEW_WORK_PHONE			"Phone"
#define VIEW_WORK_FAX			"FAX"
#define VIEW_WORK_CELL_PHONE	"Cell Phone"
#define VIEW_WORK_PAGER			"Pager"
#define VIEW_WORK_EMAIL			"E-mail"
#define VIEW_WORK_URL			"URL"

// internet stuff
#define VIEW_INET_EMAIL3		"E-mail 3"
#define VIEW_INET_EMAIL4		"E-mail 4"
#define VIEW_INET_EMAIL5		"E-mail 5"
#define VIEW_INET_URL3			"Other URL"
#define VIEW_INET_ICQ			"ICQ UIN"
#define VIEW_INET_AIM			"AIM ident"
#define VIEW_INET_JABBER		"Jabber name"
#define VIEW_INET_YAHOO			"Yahoo Messenger"
#define VIEW_INET_MSN			"MSN passport"

#define VIEW_NOTES				"Notes"


// définitions des labels des menus
// People only

#define VIEW_GENERAL_TAB		"General"
#define VIEW_HOME_TAB			"Personal"
#define VIEW_WORK_TAB			"Professional"
#define VIEW_INET_TAB			"Internet"
#define VIEW_NOTES_TAB			"Notes"
#define	VIEW_PREFS_TAB			"Preferences"

#define VIEW_SEARCH_LABEL			"search for"
#define VIEW_SEARCH_DEFAULT_TEXT	"type a bit"B_UTF8_ELLIPSIS
#define	VIEW_SEARCH_NO_RESULT		"nothing appropriate for"
#define	VIEW_SEARCH_NO_MORE_RESULT	"no more in this direction for"

#define VIEW_UPDATE_THIS_FIELD		"CHANGE-ME"

// trees
#define VIEW_PEOPLE_TREE_GROUPS		"Groups"
#define VIEW_PEOPLE_TREE_ORPHANS	"Orphans"

#define MENU_FILE				"File"
#define MENU_FILE_NEW_PERSON	"New Person"
#define MENU_FILE_CLOSE			"Close"
#define MENU_FILE_SAVE			"Save"
#define MENU_FILE_SAVE_AS		"Save as"
#define MENU_FILE_REVERT		"Revert"
#define MENU_FILE_ABOUT			"About"
#define MENU_FILE_QUIT			"Quit"

#define MENU_EDIT					"Edit"
#define MENU_EDIT_UNDO				"Undo"
#define MENU_EDIT_CUT				"Cut"
#define MENU_EDIT_COPY				"Copy"
#define MENU_EDIT_PASTE				"Paste"
#define MENU_EDIT_SELECT_ALL		"Select all"
#define MENU_EDIT_SEARCH			"Search"
#define MENU_EDIT_SEARCH_NEXT		"Next"
#define	MENU_EDIT_SEARCH_PREVIOUS	"Previous"
#define MENU_EDIT_COLLAPSE_ALL		"Collapse all"
#define MENU_EDIT_EXPAND_ALL		"Extand all"


// definitions des messages d'erreur et des boites diverses...

#define LARGEST_GENERAL_TEXT		VIEW_FIRST_NAME
#define LARGEST_HOME_TEXT			VIEW_HOME_CELL_PHONE
#define LARGEST_WORK_TEXT			VIEW_WORK_CELL_PHONE
#define LARGEST_INET_TEXT			VIEW_INET_YAHOO

#define UNKNOWN_PERSON					"Unknown person"

#define ERROR_CREATION_TEXT				"I couldn't create %s."
#define ERROR_CREATION_BUTTON_TEXT		"Sorry!"
#define SAVE_CHANGES_TEXT				"Person is modified, what to do ?"
#define SAVE_CHANGES_CANCEL_BUTTON_TEXT	"Let me see"
#define SAVE_CHANGES_QUIT_BUTTON_TEXT	"Just quit"
#define SAVE_CHANGES_SAVE_BUTTON_TEXT	"Save & quit"

#define ABOUT_BY_TEXT "Original People by Robert Polic for Be, Inc.\n\nExtensions, new features and english / french translations by Olivier Cortes\n<olive@deep-ocean.net>.\n\nOther translations by :\n  - Jason Burges\n  - David Powell\n  - Osvaldo Gonzalez\n  - Daniel Warmuth\n  - Jan Polzer\n  - Robert Paciorek\n  - An-Dee\n  - Frank Paul Silye.\n\nPlease support the BeOS driver fund initiative on BeUnited\n<http://www.beunited.org/>."
#define ABOUT_BUTTON_TEXT "Gotcha!"
