/*
 *   Copyright (C) 2011 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "SolidNetworkNotifier.h"

#include <solid/control/networkmanager.h>
#include <solid/control/wirelessnetworkinterface.h>
#include <solid/control/wirelessaccesspoint.h>

#include <NetworkManagerQt/manager.h>
#include <NetworkManagerQt/wirelessdevice.h>


#include <QHash>

#include <KDebug>


REGISTER_NETWORK_NOTIFIER(SolidNetworkNotifier)

class SolidNetworkNotifier::Private {
public:
    QHash < QString, NetworkManager::WirelessDevice * > interfaces;

};


SolidNetworkNotifier::SolidNetworkNotifier(QObject * parent)
    : NetworkNotifier(parent), d(new Private())
{
}

void SolidNetworkNotifier::init()
{
    kDebug() << "Solid";

    NetworkManager::Device::List iflist = NetworkManager::networkInterfaces();

    foreach (NetworkManager::Device::Ptr iface, iflist) {
        // Checking whether it is a wifi
        if (iface->type() != NetworkManager::Device::Wifi) continue;

        NetworkManager::WirelessDevice * wlan = static_cast < NetworkManager::WirelessDevice * > (iface.data());

        d->interfaces[wlan->interfaceName()] = wlan;

        connect(wlan, SIGNAL(activeAccessPointChanged(QString)),
                this, SLOT(setActiveAccessPoint(QString)));

    }
}

SolidNetworkNotifier::~SolidNetworkNotifier()
{
    delete d;
}

