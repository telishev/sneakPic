#ifndef CONSOLE_TASK_H
#define CONSOLE_TASK_H
/// Simple class which only used to run console processing


#include <QObject>

class QApplication;

class cl_arguments;

class console_task : public QObject
{
    Q_OBJECT
private:
  QApplication *app;
  cl_arguments *args;

public:
  console_task (QObject *parent, cl_arguments *args_arg);

public slots:
    void run();

signals:
    void finished();
};
#endif //!CONSOLE_TASK_H
