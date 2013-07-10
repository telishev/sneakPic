#ifndef CONSOLE_TASK_H
#define CONSOLE_TASK_H
/// Simple class which only used to run console processing


#include <QObject>

class QApplication;

class cl_arguments;
class main_window;

class console_task : public QObject
{
    Q_OBJECT
private:
  QApplication *app;
  cl_arguments *args;
  main_window  *main_window_inst;

public:
  console_task (QObject *parent, cl_arguments *args_arg);

public slots:
    void cleanup ();
    void run();
};
#endif //!CONSOLE_TASK_H
