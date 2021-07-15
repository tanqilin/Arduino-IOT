
#include "card.h"
#include "cardplugin.h"

#include <QtPlugin>

CardPlugin::CardPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void CardPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool CardPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *CardPlugin::createWidget(QWidget *parent)
{
    return new Card(parent);
}

QString CardPlugin::name() const
{
    return QLatin1String("Card");
}

QString CardPlugin::group() const
{
    return QLatin1String("");
}

QIcon CardPlugin::icon() const
{
    return QIcon(QLatin1String(":/card.png"));
}

QString CardPlugin::toolTip() const
{
    return QLatin1String("");
}

QString CardPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool CardPlugin::isContainer() const
{
    return false;
}

QString CardPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Card\" name=\"card\">\n</widget>\n");
}

QString CardPlugin::includeFile() const
{
    return QLatin1String("card.h");
}

