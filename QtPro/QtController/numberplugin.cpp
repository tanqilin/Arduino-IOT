#include "number.h"
#include "numberplugin.h"

#include <QtPlugin>

NumberPlugin::NumberPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void NumberPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool NumberPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *NumberPlugin::createWidget(QWidget *parent)
{
    return new Number(parent);
}

QString NumberPlugin::name() const
{
    return QLatin1String("Number");
}

QString NumberPlugin::group() const
{
    return QLatin1String("");
}

QIcon NumberPlugin::icon() const
{
    return QIcon(QLatin1String(":/number.png"));
}

QString NumberPlugin::toolTip() const
{
    return QLatin1String("");
}

QString NumberPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool NumberPlugin::isContainer() const
{
    return false;
}

QString NumberPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Number\" name=\"number\">\n</widget>\n");
}

QString NumberPlugin::includeFile() const
{
    return QLatin1String("number.h");
}

