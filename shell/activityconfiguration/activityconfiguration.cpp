/***************************************************************************
 *   Copyright 2010 Alexis Menard <menard@kde.org>                         *
 *   Copyright 2010 Artur Duque de Souza <asouza@kde.org>                  *
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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

//own
#include "activityconfiguration.h"
#include "backgroundlistmodel.h"

//Qt
#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeItem>
#include <QTimer>
#include <QGraphicsView>
#include <QApplication>

//KDE
#include <KDebug>
#include <KStandardDirs>

//Plasma
#include <Plasma/Containment>
#include <Plasma/Corona>
#include <Plasma/Context>
#include <Plasma/Package>

#ifndef NO_ACTIVITIES
#include <Activities/Controller>
#endif

ActivityConfiguration::ActivityConfiguration(QGraphicsWidget *parent)
    : Plasma::DeclarativeWidget(parent),
      m_containment(0),
      m_mainWidget(0),
      m_model(0),
      m_wallpaperIndex(-1)
{
    setQmlPath(KStandardDirs::locate("data", "plasma-device/activityconfiguration/view.qml"));
#ifndef NO_ACTIVITIES
    m_activityController = new Activities::Controller(this);
#endif

    if (engine()) {
        QDeclarativeContext *ctxt = engine()->rootContext();
        m_mainWidget = qobject_cast<QDeclarativeItem *>(rootObject());
        if (ctxt) {
            ctxt->setContextProperty("configInterface", this);
        }

        if (m_mainWidget) {
            connect(m_mainWidget, SIGNAL(closeRequested()),
                    this, SLOT(deleteLater()));
        }
    }
}

ActivityConfiguration::~ActivityConfiguration()
{
}

void ActivityConfiguration::ensureContainmentExistence()
{
    if (m_containment) {
        return;
    }
#ifndef NO_ACTIVITIES
    const QString id = m_activityController->addActivity(m_activityName);
    m_activityController->setCurrentActivity(id);
#endif
    Plasma::Corona *corona = qobject_cast<Plasma::Corona *>(scene());
    QEventLoop loop;
    //FIXME: find a better way
    QTimer::singleShot(100, &loop, SLOT(quit()));
    loop.exec();
    if (corona) {
        m_containment = corona->containmentForScreen(0);
    }
}

void ActivityConfiguration::setContainment(Plasma::Containment *cont)
{
    m_containment = cont;

    delete m_model;

    Plasma::Wallpaper *wp = 0;
    if (m_containment && m_containment->wallpaper()) {
        wp = m_containment->wallpaper();
    } else {
        wp = Plasma::Wallpaper::load("image");
        wp->setParent(this);

        //FIXME: this has to be done in C++ until we have QtComponents
        //doesn't really belong here, this is for the "first run"
        QGraphicsWidget *activityNameEdit = m_mainWidget->findChild<QGraphicsWidget*>("activityNameEdit");
        if (activityNameEdit) {
            activityNameEdit->setFocus(Qt::MouseFocusReason);
            QEvent openEvent(QEvent::RequestSoftwareInputPanel);
            if (qApp) {
                if (QGraphicsView *view = qobject_cast<QGraphicsView*>(qApp->focusWidget())) {
                    if (view->scene() && view->scene() == scene()) {
                        QApplication::sendEvent(view, &openEvent);
                    }
                }
            }
        }
    }

    if (m_containment) {
        m_activityName = m_containment->activity();
        emit activityNameChanged();
    }

    m_model = new BackgroundListModel(wp, this);
    connect(m_model, SIGNAL(countChanged()), this, SLOT(modelCountChanged()));
    m_model->setResizeMethod(Plasma::Wallpaper::CenteredResize);
    m_model->setWallpaperSize(QSize(1024, 600));
    m_model->reload();

    emit modelChanged();
}

void ActivityConfiguration::modelCountChanged()
{
    if (!m_containment || m_model->count() < 1) {
        return;
    }

    // since we're using the Image plugin, we'll cheat a bit and peek at the configuration
    // to see what wallpaper we're using
    KConfigGroup wpConfig = m_containment->config();
    wpConfig = KConfigGroup(&wpConfig, "Wallpaper");
    wpConfig = KConfigGroup(&wpConfig, "image");
    QModelIndex index = m_model->indexOf(wpConfig.readEntry("wallpaper", QString()));
    if (index.isValid()) {
        m_wallpaperIndex = index.row();
        emit wallpaperIndexChanged();
    }
}

Plasma::Containment *ActivityConfiguration::containment() const
{
    return m_containment;
}

void ActivityConfiguration::setActivityName(const QString &name)
{
    if (name == m_activityName) {
        return;
    }

    m_activityName = name;

    ensureContainmentExistence();
    if (!m_containment) {
        //should never happen
        return;
    }

    m_containment->setActivity(name);
    emit activityNameChanged();
}

QString ActivityConfiguration::activityName() const
{
    return m_activityName;
}

QString ActivityConfiguration::activityId() const
{
    if (!m_containment) {
        return QString();
    }

    return m_containment->context()->currentActivityId();
}

bool ActivityConfiguration::isActivityNameConfigurable() const
{
#ifndef NO_ACTIVITIES
    return true;
#else
    return false;
#endif
}

QObject *ActivityConfiguration::wallpaperModel()
{
    return m_model;
}

int ActivityConfiguration::wallpaperIndex()
{
    return m_wallpaperIndex;
}

void ActivityConfiguration::setWallpaperIndex(const int index)
{
    ensureContainmentExistence();
    if (!m_containment) {
        //should never happen
        return;
    }

    if (m_wallpaperIndex == index || index < 0) {
        return;
    }

    m_wallpaperIndex = index;
    Plasma::Package *b = m_model->package(index);
    if (!b) {
        return;
    }

    QString wallpaper;
    if (b->structure()->contentsPrefixPaths().isEmpty()) {
        // it's not a full package, but a single paper
        wallpaper = b->filePath("preferred");
    } else {
        wallpaper = b->path();
    }

    kDebug()<<"Setting new wallpaper path:"<<wallpaper;
    if (m_containment->wallpaper()) {
        m_containment->wallpaper()->setUrls(KUrl::List() << wallpaper);
    }

    emit wallpaperIndexChanged();
}

QSize ActivityConfiguration::screenshotSize()
{
    return m_model->screenshotSize();
}

void ActivityConfiguration::setScreenshotSize(const QSize &size)
{
    m_model->setScreenshotSize(size);
}

#include "activityconfiguration.moc"
