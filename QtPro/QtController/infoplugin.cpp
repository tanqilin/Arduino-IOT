#include "info.h"
#include "infoplugin.h"

#include <QtPlugin>

InfoPlugin::InfoPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void InfoPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool InfoPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *InfoPlugin::createWidget(QWidget *parent)
{
    return new Info(parent);
}

QString InfoPlugin::name() const
{
    return QLatin1String("Info");
}

QString InfoPlugin::group() const
{
    return QLatin1String("");
}

QIcon InfoPlugin::icon() const
{
    return QIcon(QLatin1String(":/info.png"));
}

QString InfoPlugin::toolTip() const
{
    return QLatin1String("");
}

QString InfoPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool InfoPlugin::isContainer() const
{
    return false;
}

QString InfoPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Info\" name=\"info\">\n</widget>\n");
}

QString InfoPlugin::includeFile() const
{
    return QLatin1String("info.h");
}

