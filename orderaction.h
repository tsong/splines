#ifndef QORDERACTION_H
#define QORDERACTION_H

#include <QAction>

//simple inheritance of QAction so we can emit triggered(uint k)
class OrderAction : public QAction
{
    Q_OBJECT
public:
     OrderAction(uint order, const QString &text, QObject *parent = 0);

signals:
    void triggered(uint k);

protected slots:
    void changeOrder();

protected:
    uint m_order;
};

#endif // QORDERACTION_H
