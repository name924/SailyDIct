/***************************************************************************

    settings.qml - Sidudict, a StarDict clone based on QStarDict
    Copyright 2013 - 2014 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA 02110-1301, USA.                                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    allowedOrientations: defaultAllowedOrientations

    function fillModel(){
        dictListModel.clear()
        dao.getDicts(function(entries) {
            console.log(entries.length);
            if(entries.length !== 0) {
                for (var i = 0; i < entries.length; i++) {
                    var entryI = entries.item(i);

                    dictListModel.addDict(entryI);
                }
            }
        });
    }

    Component.onCompleted: {
        fillModel();
    }

    SilicaListView{
        id: listView
        model: ListModel{
            id: dictListModel
            function addDict(dict) {
                append({ dict: dict });
            }
        }
        anchors.fill: parent
        header: PageHeader {
            title: "Settings"
        }
        delegate: dictModelDelegate
        VerticalScrollDecorator {}
    }

    Component {
        id: dictModelDelegate
        ListItem{
            id: dictListItem
            onClicked: {
//                console.log("Clicked: " + name + " - " + index + " - " + dictSwitch.checked);
                dictSwitch.checked = !dictSwitch.checked
                dao.changeDictState(dict.dict);
                //starDictLib.setSelectDict(index, dictSwitch.checked);
            }
            Row {
                Switch {
                    id: dictSwitch
                    checked: dict.isActive === 0 ? false : true
                    automaticCheck: false
                    propagateComposedEvents: true
                    onClicked: {
//                        console.log("clicked dictSwitch");
                        dao.changeDictState(dict.dict);
                        checked = !checked
                    }
                }
                Label {
                    id: dictlabel
                    text: dict.dict
                    anchors.verticalCenter: parent.verticalCenter
                    color: Theme.primaryColor
                }
            }
            menu: ContextMenu {
//                MenuItem {
//                    text: "Details"
//                    onClicked: {
////                        console.log("Details clicked");
//                        pageStack.push(Qt.resolvedUrl("DictDetails.qml"),{dictionaryName: name})
//                    }
//                }
                MenuItem {
                    text: "Delete"
                    onClicked: {
                        //console.log("Delete clicked for dict: " + name);
                        dictListModel.remove(index);
                        dao.delDict(dict.dict);
                        //dictListModel.sync();
                    }
                }
            }
        }
    }
}
