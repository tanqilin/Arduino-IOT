#include "color.h"
#include "colorplugin.h"

#include <QtPlugin>

ColorPlugin::ColorPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void ColorPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ColorPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ColorPlugin::createWidget(QWidget *parent)
{
    return new Color(parent);
}

QString ColorPlugin::name() const
{
    return QLatin1String("Color");
}

QString ColorPlugin::group() const
{
    return QLatin1String("");
}

QIcon ColorPlugin::icon() const
{
    return QIcon(QLatin1String(":/colors.png"));
}

QString ColorPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ColorPlugin::whatsThis() const
{
    return QLatin1String("颜色选择器");
}

bool ColorPlugin::isContainer() const
{
    return false;
}

QString ColorPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Color\" name=\"color\">\n</widget>\n");
}

QString ColorPlugin::includeFile() const
{
    return QLatin1String("color.h");
}

