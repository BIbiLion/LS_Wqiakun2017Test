﻿/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef EXTENSIONSYSTEM_PLUGINMANAGER_H
#define EXTENSIONSYSTEM_PLUGINMANAGER_H

#include "extensionsystem_global.h"
#include "aggregate.h"

#include <QObject>
#include <QReadWriteLock>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QTextStream;
class QSettings;
QT_END_NAMESPACE

namespace ExtensionSystem {
class PluginCollection;

namespace Internal {  // （Internal） 内部
class PluginManagerPrivate;
}

class IPlugin;
class PluginSpec;

typedef QList<PluginSpec *> PluginSpecSet;

class EXTENSIONSYSTEM_EXPORT PluginManager : public QObject
{
    Q_OBJECT

public:
    static PluginManager *instance();

    PluginManager();
    virtual ~PluginManager();

    // Object pool operations
    static void addObject(QObject *obj);
    static void removeObject(QObject *obj);
    static QList<QObject *> allObjects();
    template <typename T> static QList<T *> getObjects()
    {
        QReadLocker lock(&m_instance->m_lock);
        QList<T *> results;
        QList<QObject *> all = allObjects();
        QList<T *> result;
        foreach (QObject *obj, all) {
            result = ExtensionSystem::query_all<T>(obj);
            if (!result.isEmpty())
                results += result;
        }
        return results;
    }
    template <typename T> static T *getObject()
    {
        QReadLocker lock(&m_instance->m_lock);
        QList<QObject *> all = allObjects();
        T *result = 0;
        foreach (QObject *obj, all) {
            if ((result = ExtensionSystem::query<T>(obj)) != 0)
                break;
        }
        return result;
    }

    static QObject *getObjectByName(const QString &name);
    static QObject *getObjectByClassName(const QString &className);

    // Plugin operations
    static QList<PluginSpec *> loadQueue();
    static void loadPlugins();
    static QStringList pluginPaths();
    static void setPluginPaths(const QStringList &paths);
    static QList<PluginSpec *> plugins();
    static QHash<QString, PluginCollection *> pluginCollections();
    static void setFileExtension(const QString &extension);
    static QString fileExtension();
    static bool hasError();

    // Settings
    static void setSettings(QSettings *settings);
    static QSettings *settings();
    static void setGlobalSettings(QSettings *settings);
    static QSettings *globalSettings();
    static void writeSettings();

    // command line arguments
    static QStringList arguments();
    static bool parseOptions(const QStringList &args,
        const QMap<QString, bool> &appOptions,
        QMap<QString, QString> *foundAppOptions,
        QString *errorString);
    static void formatOptions(QTextStream &str, int optionIndentation, int descriptionIndentation);
    static void formatPluginOptions(QTextStream &str, int optionIndentation, int descriptionIndentation);
    static void formatPluginVersions(QTextStream &str);

    static QString serializedArguments();

    static void profilingReport(const char *what, const PluginSpec *spec = 0);

signals:
    void objectAdded(QObject *obj);
    void aboutToRemoveObject(QObject *obj);

    void pluginsChanged();

public slots:
    void shutdown();

private:
    Internal::PluginManagerPrivate *d;
    static PluginManager *m_instance;
    mutable QReadWriteLock m_lock;

    friend class Internal::PluginManagerPrivate;
};

} // namespace ExtensionSystem

#endif // EXTENSIONSYSTEM_PLUGINMANAGER_H
