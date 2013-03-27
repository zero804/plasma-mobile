/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -p connman-manager introspection/connman-manager.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef CONNMAN_MANAGER_H
#define CONNMAN_MANAGER_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface net.connman.Manager
 */
class NetConnmanManagerInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "net.connman.Manager"; }

public:
    NetConnmanManagerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~NetConnmanManagerInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QDBusObjectPath> ConnectService(const QVariantMap &network)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(network);
        return asyncCallWithArgumentList(QLatin1String("ConnectService"), argumentList);
    }

    inline QDBusPendingReply<> DisableTechnology(const QString &type)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(type);
        return asyncCallWithArgumentList(QLatin1String("DisableTechnology"), argumentList);
    }

    inline QDBusPendingReply<> EnableTechnology(const QString &type)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(type);
        return asyncCallWithArgumentList(QLatin1String("EnableTechnology"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetProperties()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetProperties"), argumentList);
    }

    inline QDBusPendingReply<> RequestScan(const QString &type)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(type);
        return asyncCallWithArgumentList(QLatin1String("RequestScan"), argumentList);
    }

    inline QDBusPendingReply<> SetProperty(const QString &name, const QDBusVariant &value)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(name) << qVariantFromValue(value);
        return asyncCallWithArgumentList(QLatin1String("SetProperty"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void PropertyChanged(const QString &name, const QDBusVariant &value);
};

#endif