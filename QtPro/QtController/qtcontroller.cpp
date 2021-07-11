#include "colorplugin.h"
#include "ledplugin.h"
#include "videoplugin.h"
#include "debugplugin.h"
#include "buttonplugin.h"
#include "listplugin.h"
#include "dircontrolplugin.h"
#include "numberplugin.h"
#include "infoplugin.h"
#include "qtcontroller.h"

QtController::QtController(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new ColorPlugin(this));
    m_widgets.append(new LedPlugin(this));
    m_widgets.append(new VideoPlugin(this));
    m_widgets.append(new DebugPlugin(this));
    m_widgets.append(new ButtonPlugin(this));
    m_widgets.append(new ListPlugin(this));
    m_widgets.append(new DIRControlPlugin(this));
    m_widgets.append(new NumberPlugin(this));
    m_widgets.append(new InfoPlugin(this));

}

QList<QDesignerCustomWidgetInterface*> QtController::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qtcontrollerplugin, QtController)
#endif // QT_VERSION < 0x050000
