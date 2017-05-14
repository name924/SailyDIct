/***************************************************************************

    about.qml - Sidudict, a StarDict clone based on QStarDict
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

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingSmall

            PageHeader { title: "About" }

            PullDownMenu {
                MenuItem {
                    text: "View Sidudict license"
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("LicensePage.qml"))
                    }
                }
            }
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
                    var msg = "<b>SailyDict,</b> a dictionary program based on QStarDict."
                    + "<br>Copyright © 2017 Kenneth Nwafor \& Vyacheslav Goncharenko "
                    + "&lt;k.nwafor@innopolis.ru, g.vyacheslav@innopolis.ru&gt;"
                    + "<br>"
                    + "<br>SailyDict is a fork of Sidudict which is an open source dictionary program based on QStarDict."
                    + "<br>"
                    + "<br>SailyDict began as an attempt to solve the problem in Sidudict where the dictionary is indexed each time the application is started, thereby increasing startup latency."
                    + "<br>"
                    + "<br>We reduced the latency by using an embedded SQLite database."
                    + "<br>"
                    + "<br>You can find the SailyDict source here (pull requests are very welcome!):"
                    + "<br><a href='https://github.com/name924/SailyDIct'>"
                    + "https://github.com/name924/SailyDIct</a>"
                    + "<br>"
                    + "<br>Downloadable dictionaries are based on data from the Wiktionary dumps available from:"
                    + "<br><a href='http://dumps.wikimedia.org/enwiktionary/latest/enwiktionary-latest-pages-articles.xml.bz2'>"
                    + "http://dumps.wikimedia.org/enwiktionary/latest/enwiktionary-latest-pages-articles.xml.bz2</a>"
                    + "<br>All content in these dictionaries are under the same license as Wiktionary content."
                    + "<br>"

                    return msg
                }
                text: getText()
            }
        }
    }
}
