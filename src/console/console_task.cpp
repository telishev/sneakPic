#include "console/console_task.h"
#include "console/console.h"

#include <QApplication>
#include <QStringList>

console_task::console_task (QObject *parent, cl_arguments *args_arg) : QObject (parent) 
{ 
  app = static_cast <QApplication *> (parent);
  args = args_arg;
}

void console_task::run ()
{
  app->exit (start_console_processing (args));
}