#ifndef SETORDERCOMMAND_H
#define SETORDERCOMMAND_H

#include <QUndoCommand>
#include "bspline.h"

#define SET_ORDER_COMMAND_ID 0xFF06

using namespace std;

class DisplayWidget;

class SetOrderCommand : public QUndoCommand {

public:
    SetOrderCommand(uint order, BSpline &spline, QUndoCommand *parent = 0);
    ~SetOrderCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    BSpline &m_spline;
    uint m_order;
    uint m_originalOrder;

};
#endif // SETORDERCOMMAND_H
