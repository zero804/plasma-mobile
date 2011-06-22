/***************************************************************************
 *   Copyright 2011 Marco Martin <mart@kde.org>                            *
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

#include "declarativepanel.h"

#include <QGraphicsView>
#include <QDeclarativeItem>
#include <QGraphicsObject>
#include <QGraphicsWidget>
#include <QTimer>
#include <QLayout>

#include <KWindowSystem>

#include <Plasma/Corona>


PanelProxy::PanelProxy(QObject *parent)
    : QObject(parent)
{
    m_panel = new QGraphicsView();
    m_panel->installEventFilter(this);
    m_panel->setAutoFillBackground(false);
    m_panel->viewport()->setAutoFillBackground(false);
    m_panel->setAttribute(Qt::WA_TranslucentBackground);
    m_panel->setWindowFlags(m_panel->windowFlags() | Qt::FramelessWindowHint);
    m_panel->setFrameShape(QFrame::NoFrame);
    KWindowSystem::setOnAllDesktops(m_panel->winId(), true);
    unsigned long state = NET::Sticky | NET::StaysOnTop | NET::KeepAbove;
    KWindowSystem::setState(m_panel->effectiveWinId(), state);
    KWindowSystem::setType(m_panel->effectiveWinId(), NET::Dock);
}

PanelProxy::~PanelProxy()
{
    delete m_panel;
}

QGraphicsObject *PanelProxy::mainItem() const
{
    return m_mainItem.data();
}

void PanelProxy::setMainItem(QGraphicsObject *mainItem)
{
    if (m_mainItem.data() != mainItem) {
        if (m_mainItem) {
            m_mainItem.data()->setParent(mainItem->parent());
            m_mainItem.data()->removeEventFilter(this);
        }
        m_mainItem = mainItem;
        if (mainItem) {
            mainItem->setParentItem(0);
            mainItem->setParent(this);
        }

        mainItem->installEventFilter(this);

        m_panel->resize(QSize(mainItem->boundingRect().width(), mainItem->boundingRect().height()));
        m_panel->setSceneRect(QRectF(QPointF(mainItem->x(), mainItem->y()), QSize(mainItem->boundingRect().width(), mainItem->boundingRect().height())));
        //if this is called in Compenent.onCompleted we have to wait a loop the item is added to a scene
        QTimer::singleShot(0, this, SLOT(syncMainItem()));
        emit mainItemChanged();
    }
}

void PanelProxy::syncMainItem()
{
    if (!m_mainItem) {
        return;
    }

    
}

bool PanelProxy::isVisible() const
{
    return m_panel->isVisible();
}

void PanelProxy::setVisible(const bool visible)
{
    if (m_panel->isVisible() != visible) {
        m_panel->setVisible(visible);
        if (visible) {
            m_panel->setVisible(visible);
            m_panel->raise();
        }
        emit visibleChanged();
    }
}

int PanelProxy::x() const
{
    return m_panel->pos().x();
}

void PanelProxy::setX(int x)
{
    m_panel->move(x, m_panel->pos().y());
}

int PanelProxy::y() const
{
    return m_panel->pos().y();
}

void PanelProxy::setY(int y)
{
    m_panel->move(m_panel->pos().x(), y);
}

bool PanelProxy::eventFilter(QObject *watched, QEvent *event)
{
    //Panel
    if (watched == m_panel && event->type() == QEvent::Move) {
        QMoveEvent *me = static_cast<QMoveEvent *>(event);
        if (me->oldPos().x() != me->pos().x()) {
            emit xChanged();
        }
        if (me->oldPos().y() != me->pos().y()) {
            emit yChanged();
        }

    //Main item
    } else if (watched == m_mainItem.data() &&
               (event->type() == QEvent::Resize || event->type() == QEvent::Move)) {
        m_panel->resize(QSize(m_mainItem.data()->boundingRect().width(), m_mainItem.data()->boundingRect().height()));
        m_panel->setSceneRect(QRectF(QPointF(m_mainItem.data()->x(), m_mainItem.data()->y()), QSize(m_mainItem.data()->boundingRect().width(), m_mainItem.data()->boundingRect().height())));
    }
    return false;
}


#include "declarativepanel.moc"

