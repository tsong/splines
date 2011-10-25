#include "moveknotcommand.h"
#include <QDebug>

MoveKnotCommand::MoveKnotCommand(int id, uint position, float newKnot, BSpline &spline, QUndoCommand *parent)
    : QUndoCommand(parent), m_id(id), m_position(position), m_newKnot(newKnot), m_spline(spline)
{
    m_originalKnot = m_spline.getKnots()[position];
}

MoveKnotCommand::~MoveKnotCommand() {

}

int MoveKnotCommand::id() const {
    return m_id;
}

bool MoveKnotCommand::mergeWith(const QUndoCommand *other) {
    if (other->id() != id())
        return false;

    m_newKnot = static_cast<const MoveKnotCommand*>(other)->m_newKnot;
    return true;
}

void MoveKnotCommand::redo() {
    m_spline.moveKnot(m_position, m_newKnot);
}

void MoveKnotCommand::undo() {
    m_spline.moveKnot(m_position, m_originalKnot);
}
