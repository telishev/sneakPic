#include "console/console_task.h"
#include "console/console.h"

#include <QApplication>
#include <QStringList>

console_task::console_task (QObject *parent) : QObject (parent) 
{ 
  app = static_cast <QApplication *> (parent);
}

void console_task::run ()
{
  app->exit (start_console_processing (app->arguments ()));
}