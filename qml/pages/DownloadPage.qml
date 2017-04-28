import QtQuick 2.2
import Sailfish.Silica 1.0
import QtQuick.XmlListModel 2.0

Page {
    allowedOrientations: defaultAllowedOrientations

    XmlListModel {
        id: xmlModel
        source: "https://raw.githubusercontent.com/d0b3rm4n/harbour-sidudict/master/data/dictionaries/dictionaries.xml"
        query: "/dictionaries/dictionary"

        XmlRole { name: "id"; query: "id/string()" }
        XmlRole { name: "name"; query: "name/string()" }
        XmlRole { name: "url"; query: "url/string()" }
        XmlRole { name: "entries"; query: "entries/string()" }
        XmlRole { name: "size"; query: "size/string()" }
        XmlRole { name: "date"; query: "date/string()" }
        XmlRole { name: "description"; query: "description/string()" }
    }
    SilicaListView {
        id: listView
        model: xmlModel
        anchors.fill: parent
        header: PageHeader {
            title: "Download"
        }
        PullDownMenu {
            MenuItem {
                text: "Reload"
                onClicked: {
                    console.log("reload");
                    xmlModel.reload();

                }
            }
        }
        delegate: DictDelegate{}
        VerticalScrollDecorator {}
    }
    BusyIndicator {
        id: dlBusyIndicator
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: (xmlModel.status === XmlListModel.Loading)?1:0
        visible: (xmlModel.status === XmlListModel.Ready)?0:1
    }
    Label {
        id: infoText
        visible: (xmlModel.status === XmlListModel.Error || xmlModel.status === XmlListModel.Null)?1:0
        anchors.top: dlBusyIndicator.bottom
        anchors.bottomMargin: Theme.paddingLarge
        anchors.leftMargin: Theme.paddingMedium
        anchors.rightMargin: Theme.paddingMedium
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        color: Theme.highlightColor
        font.family: Theme.fontFamilyHeading
        font.pixelSize: Theme.fontSizeMedium
        text: "You need an Internet connection, to download the dictionary catalogue and the dictionaries! You can reload the catalouge in the Pull-Down-Menu once the Internet connection is established.\n" + xmlModel.errorString()
    }
}
