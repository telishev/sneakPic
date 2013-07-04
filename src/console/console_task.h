#ifndef CONSOLE_TASK_H
#define CONSOLE_TASK_H
/// Simple class which only used to run console processing


#include <QObject>

class QApplication;

class console_task : public QObject
{
    Q_OBJECT
private:
  QApplication *app;

public:
  console_task (QObject *parent = 0);

public slots:
    void run();

signals:
    void finished();
};
#endif //!CONSOLE_TASK_H
