#include "video.h"
#include "videoplugin.h"

#include <QtPlugin>

VideoPlugin::VideoPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void VideoPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool VideoPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *VideoPlugin::createWidget(QWidget *parent)
{
    return new Video(parent);
}

QString VideoPlugin::name() const
{
    return QLatin1String("Video");
}

QString VideoPlugin::group() const
{
    return QLatin1String("");
}

QIcon VideoPlugin::icon() const
{
    return QIcon(QLatin1String(":/videoplay.png"));
}

QString VideoPlugin::toolTip() const
{
    return QLatin1String("");
}

QString VideoPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool VideoPlugin::isContainer() const
{
    return false;
}

QString VideoPlugin::domXml() const
{
    return QLatin1String("<widget class=\"Video\" name=\"video\">\n</widget>\n");
}

QString VideoPlugin::includeFile() const
{
    return QLatin1String("video.h");
}

