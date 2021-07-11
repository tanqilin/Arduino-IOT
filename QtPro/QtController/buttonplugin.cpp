#include "button.h"
#include "buttonplugin.h"

#include <QtPlugin>

ButtonPlugin::ButtonPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void ButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ButtonPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ButtonPlugin::createWidget(QWidget *parent)
{
    return new Button(parent);
}

QString ButtonPlugin::name() const
{
    return QLatin1String("Button");
}

QString ButtonPlugin::group() const
{
    return QLatin1String("");
}

QIcon ButtonPlugin::icon() const
{
    return QIcon(QLatin1String(":/button.png"));
}

QString ButtonPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ButtonPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ButtonPlugin::isContainer() const
{
    return false;
}

QString ButtonPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Button\" name=\"button\">\n</widget>\n");
}

QString ButtonPlugin::includeFile() const
{
    return QLatin1String("button.h");
}

