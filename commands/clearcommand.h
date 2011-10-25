#ifndef CLEARCOMMAND_H
#define CLEARCOMMAND_H

#include <QUndoCommand>
#include "utils/vector.h"
#include "bspline.h"

#define CLEAR_COMMAND_ID 0xFF04

using namespace std;

class ClearCommand : public QUndoCommand {

public:
    ClearCommand(BSpline &spline, QUndoCommand *parent = 0);
    ~ClearCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    vector<float> m_originalKnots;
    vector<Vector2f> m_originalPoints;
    BSpline &m_spline;
};

#endif // CLEARCOMMAND_H
