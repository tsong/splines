#include "resetknotscommand.h"

ResetKnotsCommand::ResetKnotsCommand(BSpline &spline, QUndoCommand *parent)
    : QUndoCommand(parent), m_spline(spline)
{
    m_originalKnots = m_spline.getKnots();
}

ResetKnotsCommand::~ResetKnotsCommand() {

}

int ResetKnotsCommand::id() const {
    return RESET_BASIS_COMMAND_ID;
}

bool ResetKnotsCommand::mergeWith(const QUndoCommand *other) {
    return other->id() == id();
}

void ResetKnotsCommand::redo() {
    m_spline.resetKnots();
}

void ResetKnotsCommand::undo() {
    m_spline.setKnots(m_originalKnots);
}
