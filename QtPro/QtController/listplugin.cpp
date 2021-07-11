#include "list.h"
#include "listplugin.h"

#include <QtPlugin>

ListPlugin::ListPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void ListPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ListPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ListPlugin::createWidget(QWidget *parent)
{
    return new List(parent);
}

QString ListPlugin::name() const
{
    return QLatin1String("List");
}

QString ListPlugin::group() const
{
    return QLatin1String("");
}

QIcon ListPlugin::icon() const
{
    return QIcon(QLatin1String(":/list.png"));
}

QString ListPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ListPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ListPlugin::isContainer() const
{
    return false;
}

QString ListPlugin::domXml() const
{
    return QLatin1String("<widget class=\"List\" name=\"list\">\n</widget>\n");
}

QString ListPlugin::includeFile() const
{
    return QLatin1String("list.h");
}

