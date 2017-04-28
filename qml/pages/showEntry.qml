import QtQuick 2.2
import Sailfish.Silica 1.0
import "../service"

Page {
    id: page
    property string dict
    property string entry

    allowedOrientations: defaultAllowedOrientations

    function fillModel(){
        translationListModel.clear()
        dao.getTranslations(entry, dict, function(entries) {
            console.log(entry + " was clicked! It has " + entries.length + " translations" );
            if(entries.length !== 0) {
                firstTranslation = entries.item(0).info
                lastEntry = entry
                for (var i = 0; i < entries.length; i++) {
                    var entryI = entries.item(i);

                    translationListModel.addTranslation(entryI.info);
                }
            }
        });
    }

    Component.onCompleted: {
        fillModel();
    }

    SilicaListView {
            id: listView
            anchors.fill: parent
            header: PageHeader {
                title: entry
            }

            currentIndex: -1
            model: ListModel{
                id: translationListModel
                function addTranslation(translation) {
                    append({ info: translation });
                }
            }
            delegate: BackgroundItem {
                id: delegate
                enabled: false
                height: tranlsation.height
                TextArea {
                    id: tranlsation
//                    x: Theme.paddingLarge
                    anchors {left: parent.left; right: parent.right}
                    width: page.width
                    text: info
                    enabled: false
                    anchors.verticalCenter: parent.verticalCenter
                    color: Theme.primaryColor
                    //wrapMode: TextEdit.WordWrap
//                    verticalAlignment: TextEdit.AlignTop
                }
//                 onClicked: console.log("Clicked " + index)
            }
//            VerticalScrollDecorator {}
    }

//    SilicaFlickable {
//        anchors.fill: parent

//        PageHeader {
//            id: header
//            title: entry
//        }

//        SilicaListView {
//            id: listView
//            anchors.bottom: parent.bottom
//            anchors.top: header.bottom
//            currentIndex: -1
//            model: ListModel{
//                id: translationListModel
//                function addTranslation(translation) {
//                    append({ info: translation });
//                }
//            }

//            delegate: ListItem {
////                contentHeight: Theme.itemSizeMedium
//                anchors.margins: Theme.paddingMedium

//                TextArea {
//                    id: translation
//                    width: 100//300 - 2*Theme.paddingMedium
//                    text: info
//                    anchors {left: parent.left; right: parent.right}
////                    anchors.margins: Theme.paddingMedium
//                    wrapMode: Text.WordWrap
//                    color: Theme.primaryColor
//                }
//                Rectangle {
//                    height: 1
//                    color: 'white'
//                    anchors {
//                        left: parent.left
//                        right: parent.right
//                        top: translation.bottom
//                    }
//                    opacity: 0.2
//                    anchors.margins: Theme.paddingMedium
//                }
//            }
//        }
//    }
}

