/*
    Copyright 2010 Marco Martin <notmart@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

import Qt 4.7
import org.kde.qtextracomponents 0.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.mobilecomponents 0.1 as MobileComponents


Item {
    id: main
    width: 120
    height: 120
    //property string urlText: url
    property string text
    property variant icon
    signal clicked
    property alias textColor: iconText.color

    QIconItem {
        anchors.horizontalCenter: parent.horizontalCenter
        width: 64
        height: 64
        id: iconWidgt
        icon: main.icon
    }
    Text {
        id: iconText
        y: 67
        width: parent.width -16
        wrapMode:Text.Wrap
        horizontalAlignment: Text.AlignHCenter
        clip: true
        anchors.horizontalCenter: parent.horizontalCenter
        text: main.text
        color: theme.textColor
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        onClicked : {
            //appsView.currentIndex = index
            main.clicked()
        }
    }
}
