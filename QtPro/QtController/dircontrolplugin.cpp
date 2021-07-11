#include "dircontrol.h"
#include "dircontrolplugin.h"

#include <QtPlugin>

DIRControlPlugin::DIRControlPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void DIRControlPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool DIRControlPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *DIRControlPlugin::createWidget(QWidget *parent)
{
    return new DIRControl(parent);
}

QString DIRControlPlugin::name() const
{
    return QLatin1String("DIRControl");
}

QString DIRControlPlugin::group() const
{
    return QLatin1String("");
}

QIcon DIRControlPlugin::icon() const
{
    return QIcon(QLatin1String(":/direction_up_down.png"));
}

QString DIRControlPlugin::toolTip() const
{
    return QLatin1String("");
}

QString DIRControlPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool DIRControlPlugin::isContainer() const
{
    return false;
}

QString DIRControlPlugin::domXml() const
{
    return QLatin1String("<widget class=\"DIRControl\" name=\"dIRControl\">\n</widget>\n");
}

QString DIRControlPlugin::includeFile() const
{
    return QLatin1String("dircontrol.h");
}

