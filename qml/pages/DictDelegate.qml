import QtQuick 2.2
import Sailfish.Silica 1.0
import "../pages"

ListItem {
    Item {
        anchors.margins: Theme.paddingMedium
        anchors.fill: parent
        Label {
            id: nameLabel
            text: name
//            anchors.ma: 0
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            height: 1
            color: 'white'
            anchors {
                left: parent.left
                right: parent.right
                top: parent.bottom
            }
            opacity: 0.2
            anchors.topMargin: Theme.paddingMedium
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
                var dict_to_sql_name = sql_parser.downloadDict(url);
                sql_parser.start_parse(dict_to_sql_name);
                console.log("Download context menu clicked: " + url);
            }
        }
    }
}
