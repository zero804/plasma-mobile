/*
 *   Copycontext 2015 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
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

import QtQuick 2.1
import QtQuick.Controls 1.3
import "private"
import org.kde.plasma.mobilecomponents 0.2

/**
 * A window that provides some basic features needed for all apps
 *
 * It's usually used as a root QML component for the application.
 * It's based around the PageRow component, the application will be
 * about pages adding and removal.
 */
ApplicationWindow {
    id: root

    /**
     * The first page that will be loaded when the application starts
     */
    property alias initialPage: __pageStack.initialPage

    /**
     * The stack used to allocate the pages and to manage the transitions
     * between them.
     * It's using a PageRow, while having the same API as PageStack,
     * it positions the pages as adjacent columns, with as many columns
     * as can fit in the screen. An handheld device would usually have a single
     * fullscreen column, a tablet device would have many tiled columns.
     */
    property alias pageStack: __pageStack

    /**
     * Show a little notification in the application window
     * This is to be used for little, non invasive and not workflow
     * interrupting messages. It's not intended to replace the 
     * system-wide notification system.
     * @param message The message to show to the user
     * @param timeout optional: timeout in milliseconds or predefined values "short" and "long"
     * @param actionText optional: the text for an action button if the notification is intended to have an associated action
     * @param callBack optional: a callback JavaScript function() that will be executed when the user clicks the button
     * 
     */
    function showPassiveNotification(message, timeout, actionText, callBack) {
        if (!__actionButton.__passiveNotification) {
            var component = Qt.createComponent(Qt.resolvedUrl("private/PassiveNotification.qml"));
            __actionButton.__passiveNotification = component.createObject(contentItem.parent);
        }

        __actionButton.__passiveNotification.showNotification(message, timeout, actionText, callBack);
    }

    /**
     * Hide the passive notification, if any is shown
     */
    function hidePassiveNotification() {
        if(__actionButton.__passiveNotification) {
           __actionButton.__passiveNotification.hideNotification();
        }
    }

    PageRow {
        id: __pageStack
        anchors {
            fill: parent
            bottomMargin: Qt.platform.os == "android" ? 0 : Qt.inputMethod.keyboardRectangle.height
        }
        focus: true
        Keys.onReleased: {
            if (event.key == Qt.Key_Back ||
            (event.key === Qt.Key_Left && (event.modifiers & Qt.AltModifier))) {
                event.accepted = true;

                if (root.contextDrawer && root.contextDrawer.opened) {
                    root.contextDrawer.close();
                } else if (root.globalDrawer && root.globalDrawer.opened) {
                    root.globalDrawer.close();
                } else if (__pageStack.depth >= 1) {
                    var backEvent = {accepted: false}
                    __pageStack.lastVisiblePage.backRequested(backEvent);
                    if (!backEvent.accepted) {
                        if (__pageStack.depth > 1) {
                            __pageStack.pop();
                        } else {
                            Qt.quit();
                        }
                    }
                } else {
                    Qt.quit();
                }
            }
        }
    }

    property AbstractDrawer globalDrawer
    property AbstractDrawer contextDrawer

    onGlobalDrawerChanged: {
        globalDrawer.parent = contentItem.parent;
    }
    onContextDrawerChanged: {
        contextDrawer.parent = contentItem.parent;
    }

    width: Units.gridUnit * 25
    height: Units.gridUnit * 30

    property alias actionButton: __actionButton
    ActionButton {
        id: __actionButton
        //put it there just to make it not accessible bu users
        property Item __passiveNotification
        z: 9999
        anchors.bottom: parent.bottom
        x: parent.width/2 - width/2
        iconSource: action && action.iconName ? action.iconName : ""
    }
}
