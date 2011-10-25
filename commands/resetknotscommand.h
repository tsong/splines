#ifndef RESETBASISCOMMAND_H
#define RESETBASISCOMMAND_H

#include <QUndoCommand>
#include "bspline.h"

#define RESET_BASIS_COMMAND_ID 0xFF09

using namespace std;

class ResetKnotsCommand : public QUndoCommand {

public:
    ResetKnotsCommand(BSpline &spline, QUndoCommand *parent = 0);
    ~ResetKnotsCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    BSpline &m_spline;
    vector<float> m_originalKnots;
};

#endif // RESETBASISCOMMAND_H
