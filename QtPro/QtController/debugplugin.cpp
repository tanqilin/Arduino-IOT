#include "debug.h"
#include "debugplugin.h"

#include <QtPlugin>

DebugPlugin::DebugPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void DebugPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool DebugPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *DebugPlugin::createWidget(QWidget *parent)
{
    return new Debug(parent);
}

QString DebugPlugin::name() const
{
    return QLatin1String("Debug");
}

QString DebugPlugin::group() const
{
    return QLatin1String("");
}

QIcon DebugPlugin::icon() const
{
    return QIcon(QLatin1String(":/debug.png"));
}

QString DebugPlugin::toolTip() const
{
    return QLatin1String("");
}

QString DebugPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool DebugPlugin::isContainer() const
{
    return false;
}

QString DebugPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Debug\" name=\"debug\">\n</widget>\n");
}

QString DebugPlugin::includeFile() const
{
    return QLatin1String("debug.h");
}

