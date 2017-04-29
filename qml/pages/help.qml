
import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    allowedOrientations: defaultAllowedOrientations

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingSmall

            PageHeader { title: "Help" }
            Label {
                wrapMode: Text.WordWrap
                anchors {left: parent.left; right: parent.right}
                anchors.margins: Theme.paddingMedium
                textFormat: Text.StyledText
                linkColor: Theme.highlightColor
                onLinkActivated: {
//                    console.log("clicked: " + link);
                    Qt.openUrlExternally(link);
                }
                function getText() {
                    var msg = "SailyDict does not contain pre-installed dictionaries"
                    + " (the package would just grow too much, and not all users want all"
                    + " dictionaries installed)."
                    + "<br>"
                    + "<br>You can download dictionaries for offline use from the"
                    + " Download view. Either you long press on the name to open the context menu"
                    + " or you tap on the name to see first some details about the dictionary."
                    + " There you can use the pull-down menu."
                    + "<br>"

                    return msg
                }

                text: getText()
            }
        }
    }
}
