#include "orderaction.h"

OrderAction::OrderAction(uint order, const QString &text, QObject *parent)
    : QAction(text, parent), m_order(order)
{
    connect(this, SIGNAL(triggered()), this, SLOT(changeOrder()));
}

void OrderAction::changeOrder() {
    emit triggered(m_order);
}
