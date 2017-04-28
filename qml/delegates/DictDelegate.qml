import QtQuick 2.2
import Sailfish.Silica 1.0

ListItem {
    Item {
        anchors.margins: Theme.paddingMedium
        anchors.fill: parent
        Label {
            text: name
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    onClicked: {
        // console.log("Details clicked");
        pageStack.push(Qt.resolvedUrl("DictDownloadDetails.qml"),
                       {
                           dictionaryName: name,
                           dictionaryEntries: entries,
                           dictionaryUrl: url,
                           dictionarySize: size,
                           dictionaryDate: date,
                           dictionaryDescription: description
                       })
    }
    menu: ContextMenu {
        MenuItem {
            text: "Download"
            onClicked: {
                starDictLib.downloadDict(url);
                console.log("Download context menu clicked: " + url);
            }
        }
    }
}
