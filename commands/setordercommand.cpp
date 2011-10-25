#include "setordercommand.h"

SetOrderCommand::SetOrderCommand(uint order, BSpline &spline, QUndoCommand *parent)
    : QUndoCommand(parent), m_spline(spline), m_order(order)
{
    m_originalOrder = m_spline.getOrder();
    m_originalKnots = m_spline.getKnots();
}

SetOrderCommand::~SetOrderCommand() {}

int SetOrderCommand::id() const {
    return SET_ORDER_COMMAND_ID;
}

bool SetOrderCommand::mergeWith(const QUndoCommand *) {
    return false;
}

void SetOrderCommand::redo() {
    m_spline.setOrder(m_order);
}

void SetOrderCommand::undo() {
    m_spline.setOrder(m_originalOrder);
    m_spline.setKnots(m_originalKnots);
}

