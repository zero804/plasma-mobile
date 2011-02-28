/*
 *   Copyright 2011 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import Qt 4.7
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.qtextracomponents 0.1 as QtExtra

Item {
    id: main
    signal shrinkRequested

    Component.onCompleted: {

        plasmoid.containmentType = "CustomContainment"

        plasmoid.appletAdded.connect(addApplet)

        for (var i = 0; i < plasmoid.applets.length; ++i) {
            var applet = plasmoid.applets[i]
            print(applet)
            addApplet(applet, 0);
        }
    }


    function addApplet(applet, pos)
    {
        var component = Qt.createComponent("PlasmoidContainer.qml");
        var plasmoidContainer = component.createObject(appletsRow, {"x": pos.x, "y": pos.y});
        plasmoidContainer.applet = applet
        applet.parent = plasmoidContainer
        applet.x=0
        applet.y=0
    }


    PlasmaCore.Theme {
        id: theme
    }

    PlasmaCore.Svg {
        id: iconsSvg
        imagePath: "widgets/action-overlays"
    }

    Item {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        Flickable {
            id: appletsFlickable
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            clip: true
            interactive:true
            contentWidth: mainRow.width
            contentHeight: mainRow.height

            width: Math.min(parent.width, appletsRow.width)

            Row {
                id: mainRow
                Row {
                    id: appletsRow
                    height: appletsFlickable.height
                    move: Transition {
                        NumberAnimation {
                            properties: "x"
                            easing.type: Easing.OutBounce
                            duration: 250
                        }
                    }
                }
                Item {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: main.width/2
                    ActionButton {
                        anchors.centerIn: parent
                        elementId: "add-normal"
                        action: plasmoid.action("add widgets")
                    }
                }
            }
        }
    }
}
