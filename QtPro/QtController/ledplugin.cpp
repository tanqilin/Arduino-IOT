#include "led.h"
#include "ledplugin.h"

#include <QtPlugin>

LedPlugin::LedPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void LedPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool LedPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *LedPlugin::createWidget(QWidget *parent)
{
    return new Led(parent);
}

QString LedPlugin::name() const
{
    return QLatin1String("Led");
}

QString LedPlugin::group() const
{
    return QLatin1String("");
}

QIcon LedPlugin::icon() const
{
    return QIcon(QLatin1String(":/led-on.png"));
}

QString LedPlugin::toolTip() const
{
    return QLatin1String("");
}

QString LedPlugin::whatsThis() const
{
    return QLatin1String("Led模拟灯，用于显示传感器状态");
}

bool LedPlugin::isContainer() const
{
    return false;
}

QString LedPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Led\" name=\"led\">\n</widget>\n");
}

QString LedPlugin::includeFile() const
{
    return QLatin1String("led.h");
}

