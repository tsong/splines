#ifndef MOVEKNOTCOMMAND_H
#define MOVEKNOTCOMMAND_H

#include <QUndoCommand>
#include "utils/vector.h"
#include "bspline.h"

using namespace std;

class MoveKnotCommand : public QUndoCommand {

public:
    MoveKnotCommand(int id, uint position, float newKnot, BSpline &spline, QUndoCommand *parent = 0);
    ~MoveKnotCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    int m_id;
    uint m_position;
    float m_newKnot;
    float m_originalKnot;
    BSpline &m_spline;

};

#endif // MOVEKNOTCOMMAND_H
