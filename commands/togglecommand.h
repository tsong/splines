#ifndef TOGGLECOMMAND_H
#define TOGGLECOMMAND_H

#include <QUndoCommand>

#define TOGGLE_COMMAND_ID 0xFF07

using namespace std;

class ToggleCommand : public QUndoCommand {

public:
    ToggleCommand(bool &variable, QUndoCommand *parent = 0);
    ~ToggleCommand();

    int id() const;
    bool mergeWith(const QUndoCommand *other);
    void redo();
    void undo();

protected:
    bool &m_variable;
    bool m_originalValue;

};

#endif // TOGGLECOMMAND_H
