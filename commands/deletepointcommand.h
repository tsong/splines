#ifndef DELETEPOINTCOMMAND_H
#define DELETEPOINTCOMMAND_H

#include <QUndoCommand>

#include <QUndoCommand>
#include "utils/vector.h"
#include "bspline.h"

#define DELETE_POINT_COMMAND_ID 0xFF01

using namespace std;

class DeletePointCommand : public QUndoCommand {

public:
    DeletePointCommand(uint position, BSpline &spline, QUndoCommand *parent = 0);
    ~DeletePointCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    BSpline &m_spline;
    uint m_position;
    Vector2f m_point;
    vector<float> m_originalKnots;
};

#endif // DELETEPOINTCOMMAND_H
