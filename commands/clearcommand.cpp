#include "clearcommand.h"

ClearCommand::ClearCommand(BSpline &spline, QUndoCommand *parent)
    : QUndoCommand(parent), m_spline(spline)
{
    m_originalPoints = m_spline.getPoints();
    m_originalKnots = m_spline.getKnots();
}

ClearCommand::~ClearCommand() {

}

int ClearCommand::id() const {
    return CLEAR_COMMAND_ID;
}

bool ClearCommand::mergeWith(const QUndoCommand *other) {
    return other->id() == id();
}

void ClearCommand::redo() {
    m_spline.clearAllPoints();
}

void ClearCommand::undo() {
    m_spline.setPoints(m_originalPoints);
    m_spline.setKnots(m_originalKnots);
}
