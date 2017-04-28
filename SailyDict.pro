# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = SailyDict

CONFIG += sailfishapp

SOURCES += src/SailyDict.cpp \
    src/downloadmanager.cpp \
    src/parser.cpp \
    src/sqlparser.cpp

OTHER_FILES += qml/SailyDict.qml \
    qml/cover/CoverPage.qml \
    rpm/SailyDict.changes.in \
    rpm/SailyDict.spec \
    rpm/SailyDict.yaml \
    translations/*.ts \
    SailyDict.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/SailyDict-de.ts

HEADERS += \
    src/downloadmanager.h \
    src/parser.h \
    src/sqlparser.h

INCLUDEPATH += /usr/include/c++/6.3.1/
LIBS += -lsqlite3
QT += dbus

DISTFILES += \
    qml/delegates/DictDelegate.qml \
    qml/model/EntryListModel.qml \
    qml/model/TestListModel.qml \
    qml/model/TranslationListModel.qml \
    qml/pages/about.qml \
    qml/pages/DictDelegate.qml \
    qml/pages/DictDownloadDetails.qml \
    qml/pages/DownloadPage.qml \
    qml/pages/help.qml \
    qml/pages/SearchPage.qml \
    qml/pages/settings.qml \
    qml/pages/showEntry.qml \
    qml/service/Dao.qml
