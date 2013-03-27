/*
 * Copyright (C) 2011 Marco Martin <mart@kde.org>
 * Copyright (C) 2011 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef RECOMMENDATIONJOB_H
#define RECOMMENDATIONJOB_H

// plasma
#include <Plasma/ServiceJob>

#include "RecommendationItem.h"

namespace Contour {

class RecommendationJob : public Plasma::ServiceJob {
    Q_OBJECT

public:
    RecommendationJob(const QString & operation,
                      const QString & engine,
                      const QString & id,
                      QMap < QString, QVariant > & parameters,
                      QObject * parent = 0);
    ~RecommendationJob();

protected:
    void start();

private:
    QString m_engine, m_id;

};

}

#endif
