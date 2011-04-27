/*
    Copyright 2011 Sebastian Kügler <sebas@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

// Nepomuk
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include <Nepomuk/Query/ResourceTerm>
#include <Nepomuk/Tag>

#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/FileQuery>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/Result>

#include <soprano/queryresultiterator.h>
#include <soprano/model.h>
#include <soprano/vocabulary.h>

#include <nepomuk/andterm.h>
#include <nepomuk/orterm.h>
#include <nepomuk/comparisonterm.h>
#include <nepomuk/literalterm.h>
#include <nepomuk/resourcetypeterm.h>

#include "bookmarksengine.h"
#include "../metadatabaseengine.h"
#include <stdio.h>


BookmarksEngine::BookmarksEngine(QObject* parent, const QVariantList& args)
    : MetadataBaseEngine(parent, args)
{
}

BookmarksEngine::~BookmarksEngine()
{
}

bool BookmarksEngine::sourceRequestEvent(const QString &name)
{
    // Let's try a literal query ...
    kDebug() << "async search for query:" << name;
    Nepomuk::Query::Query fileQuery;
    Nepomuk::Query::LiteralTerm nepomukTerm(name);
    fileQuery.setTerm(nepomukTerm);
    //fileQuery.addIncludeFolder(KUrl("/"), true);
    return query(fileQuery);
}


#include "bookmarksengine.moc"