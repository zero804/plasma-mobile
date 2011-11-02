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
 *   GNU Library General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.mobilecomponents 0.1 as MobileComponents
import org.kde.qtextracomponents 0.1
import Qt.labs.gestures 1.0
import org.kde.plasma.slccomponents 0.1 as SlcComponents
import org.kde.metadatamodels 0.1 as MetadataModels


Image {
    id: imageViewer
    objectName: "imageViewer"
    source: viewerPackage.filePath("images", "fabrictexture.png")
    fillMode: Image.Tile
    state: "browsing"

    width: 360
    height: 360

    property bool firstRun: true

    MobileComponents.Package {
        id: viewerPackage
        name: "org.kde.active.imageviewer"
    }

    MobileComponents.ResourceInstance {
        id: resourceInstance
    }

    PlasmaCore.Theme {
        id: theme
    }

    function loadImage(path)
    {
        if (path.length == 0) {
            return
        }

        if (String(path).indexOf("/") === 0) {
            path = "file://"+path
        }

        //is in Nepomuk
        var index = metadataModel.find(path);
        if (index > -1) {
            fullList.model = filterModel
            quickBrowserBar.model = filterModel
            fullList.positionViewAtIndex(index, ListView.Center)
            fullList.currentIndex = index
            spareDelegate.visible = false
            fullList.visible = true
            imageViewer.state = "image"
            return
        }


        //is in dirModel
        fullList.model = dirModel
        quickBrowserBar.model = dirModel
        var i = dirModel.indexForUrl(path)
        fullList.positionViewAtIndex(i, ListView.Center)
        fullList.currentIndex = i
        spareDelegate.visible = false
        fullList.visible = true
        imageViewer.state = "image"
        return
    }

    Timer {
        id: firstRunTimer
        interval: 300
        repeat: false
        onTriggered: {
            loadImage(startupArguments[0])
            imageViewer.firstRun = false
        }
    }

    PlasmaCore.SortFilterModel {
        id: filterModel
        sourceModel: MetadataModels.MetadataModel {
            id: metadataModel
            resourceType: "nfo:Image"
            sortBy: [userTypes.sortFields[itemGroup.category]]
            sortOrder: Qt.AscendingOrder
        }
        filterRole: "label"
    }


    Toolbar {
        id: toolbar
    }

    QuickBrowserBar {
        id: quickBrowserBar
        model: filterModel
    }

    MobileComponents.IconGrid {
        id: resultsGrid
        anchors {
            fill: parent
            topMargin: toolbar.height
        }

        Component.onCompleted: resultsContainer.contentY = resultsContainer.height
        height: resultsContainer.height
        model: filterModel
        delegateWidth: 130
        delegateHeight: 120
        delegate: MobileComponents.ResourceDelegate {
            id: resourceDelegate
            className: model["className"]?model["className"]:"Image"
            width: 130
            height: 120
            infoLabelVisible: false
            property string label: model["label"]?model["label"]:model["display"]

            onPressAndHold: {
                resourceInstance.uri = model["url"]?model["url"]:model["resourceUri"]
                resourceInstance.title = model["label"]
            }

            onClicked: {
                if (mimeType == "inode/directory") {
                    dirModel.url = model["url"]
                    resultsGrid.model = dirModel
                } else {
                    loadImage(model["url"])
                }
            }
        }
    }

    Rectangle {
        id: viewer
        scale: startupArguments[0].length > 0?1:0

        function setCurrentIndex(index)
        {
            fullList.positionViewAtIndex(index, ListView.Center)
            fullList.currentIndex = index
        }

        color: "black"
        anchors {
            fill:  parent
        }
        Behavior on scale {
            NumberAnimation {
                duration: 250
                easing.type: Easing.InOutQuad
            }
        }
        FullScreenDelegate {
            id: spareDelegate
            anchors {
                fill:  parent
            }
            visible: false
        }
        ListView {
            id: fullList
            anchors.fill: parent
            model: filterModel
            highlightRangeMode: ListView.StrictlyEnforceRange
            orientation: ListView.Horizontal
            snapMode: ListView.SnapOneItem
            cacheBuffer: 40
            //highlightFollowsCurrentItem: true
            delegate: FullScreenDelegate {
                source: model["url"]
            }

            onCurrentIndexChanged: {
                resourceInstance.uri = currentItem.source
                resourceInstance.title = currentItem.label
                quickBrowserBar.setCurrentIndex(currentIndex)
            }
            visible: false
        }

    }

    SlcComponents.SlcMenu {
        id: contextMenu
    }

    states: [
        State {
            name: "browsing"
            PropertyChanges {
                target: toolbar
                y: 0
            }
            PropertyChanges {
                target: quickBrowserBar
                y: imageViewer.height+20
            }
            PropertyChanges {
                target: viewer
                scale: 0
            }
        },
        State {
            name: "image"
            PropertyChanges {
                target: toolbar
                y: -toolbar.height
            }
            PropertyChanges {
                target: quickBrowserBar
                y: imageViewer.height+20
            }
            PropertyChanges {
                target: viewer
                scale: 1
            }
        },
        State {
            name: "image+toolbar"
            PropertyChanges {
                target: toolbar
                y: 0
            }
            PropertyChanges {
                target: quickBrowserBar
                y: imageViewer.height-quickBrowserBar.height
            }
            PropertyChanges {
                target: viewer
                scale: 1
            }
        }
    ]
}
