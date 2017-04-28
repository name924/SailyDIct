import QtQuick 2.2
import Sailfish.Silica 1.0
import "../model"
import "../service"

Page {
    id: searchPage
    property string searchString
    property bool keepSearchFieldFocus: true
    property int curIndex

    allowedOrientations: defaultAllowedOrientations

//    Keys.onReturnPressed: {
//        if (starDictLib.isFirstListItemTranslatable()) {
//            var entry = starDictLib.firstListItemEntry();
//            var translation = starDictLib.getTranslation(entry, starDictLib.firstListItemDict());
//            pageStack.push(Qt.resolvedUrl("showEntry.qml"),{pageTitleEntry: entry, dictTranslatedEntry: translation});
//        }
//    }

    onSearchStringChanged: {
//        entryListModel.clear()
        updateModel(searchString)
//        starDictLib.updateList(searchString)
    }

    Component.onCompleted: {
        updateModel(searchString)
//        starDictLib.updateList(searchString)
    }

//    Loader {
//        anchors.fill: parent
//        sourceComponent: listViewComponent
//    }

//    Component {
//        Rectangle {
//            bor
//        }
//    }

    Column {
        id: headerContainer

        width: searchPage.width

        SearchField {
            id: searchField
//            background: color: "blue"

            width: parent.width
            inputMethodHints: Qt.ImhNoAutoUppercase
            placeholderText: "Search for..."

            Binding {
                target: searchPage
                property: "searchString"
                value: searchField.text
            }
        }
    }

//    Component {
//        id: listViewComponent
    SilicaListView {
        id: listView
        model: ListModel{
            id: entryListModel
            function addDictEntry(entry, dict) {
                append({
                           entry: entry,
                           dict: dict
                       });
            }
        }

        anchors.fill: parent
        currentIndex: -1 // otherwise currentItem will steal focus

        Binding {
            target: searchPage
            property: "curIndex"
            value: listView.currentIndex
        }

        header:  Item {
            id: header
            width: headerContainer.width
            height: headerContainer.height
            Component.onCompleted: headerContainer.parent = header
        }

        PullDownMenu {
                MenuItem {
                    text: "About"
                    onClicked: {
//                        console.log("Clicked option About")
                        pageStack.push(Qt.resolvedUrl("about.qml"))
                    }
                }
                MenuItem {
                    text: "Help"
                    onClicked: {
//                        console.log("Clicked option Help")
                        pageStack.push(Qt.resolvedUrl("help.qml"))
                    }
                }
            MenuItem {
                text: "Download"
                onClicked: {
//                        console.log("Clicked option Download")
                    pageStack.push(Qt.resolvedUrl("DownloadPage.qml"))
                }
            }
            MenuItem {
                text: "Settings"
                 onClicked: {
                     console.log("Clicked option Settings")
                     pageStack.push(Qt.resolvedUrl("settings.qml"))
                 }
            }
        }

        delegate: ListItem{
            contentHeight: Theme.itemSizeMedium // two line delegate
            anchors.margins: Theme.paddingMedium

            ListModel{
                id: translationListModel
                function addTranslation(translation) {
                    append({
                                translation: translation
                            });
                }
            }
            onClicked: {
                pageStack.push(Qt.resolvedUrl("showEntry.qml"),{entry: entry, dict: dict})
            }


            Label {
                id: enrtyLabel
                text: entry
                anchors {left: parent.left; right: parent.right}
                anchors.margins: Theme.paddingMedium
                color: Theme.primaryColor
            }
            Label {
                id: dictLabel
                anchors.top: enrtyLabel.bottom
                text: dict
                anchors {left: parent.left; right: parent.right}
                anchors.margins: Theme.paddingMedium
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
            }
            Rectangle {
                height: curIndex === entryListModel.count - 1 ? 0 : 1
                color: 'white'
                anchors {
                    left: parent.left
                    right: parent.right
                    top: dictLabel.bottom
                }
                opacity: 0.2
                anchors.margins: Theme.paddingMedium
            }
        }

        VerticalScrollDecorator {}

        Component.onCompleted: {
            if (keepSearchFieldFocus) {
                searchField.forceActiveFocus()
            }
            keepSearchFieldFocus = false
        }
    }
    //}

    function updateModel(searchString) {
        entryListModel.clear();
        if (searchString === ""){
            return
        }

        dao.getSuggestions(searchString, function(entries) {
            if(entries.length !== 0) {
                for (var i = 0; i < entries.length; i++) {
                    var entry = entries.item(i);

                    entryListModel.addDictEntry(entry.word, entry.dict);
                }
            }
        });
    }
}
