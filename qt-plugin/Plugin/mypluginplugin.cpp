#include "myplugin.h"
#include "mypluginplugin.h"

#include <QtPlugin>

myPluginPlugin::myPluginPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void myPluginPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool myPluginPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *myPluginPlugin::createWidget(QWidget *parent)
{
    return new myPlugin(parent);
}

QString myPluginPlugin::name() const
{
    return QLatin1String("myPlugin");
}

QString myPluginPlugin::group() const
{
    return QLatin1String("");
}

QIcon myPluginPlugin::icon() const
{
    return QIcon(QLatin1String(":/Desert.jpg"));
}

QString myPluginPlugin::toolTip() const
{
    return QLatin1String("");
}

QString myPluginPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool myPluginPlugin::isContainer() const
{
    return false;
}

QString myPluginPlugin::domXml() const
{
    return QLatin1String("<widget class=\"myPlugin\" name=\"myPlugin\">\n</widget>\n");
}

QString myPluginPlugin::includeFile() const
{
    return QLatin1String("myplugin.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mypluginplugin, myPluginPlugin)
#endif // QT_VERSION < 0x050000
