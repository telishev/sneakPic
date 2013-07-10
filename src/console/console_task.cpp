#include "console/console_task.h"
#include "console/console.h"
#include "console/gengetopt/cl_arguments_class.h"

#include "common/memory_deallocation.h"

#include <QApplication>
#include <QStringList>

#ifdef _WINDOWS
#include <consoleapi.h>
#endif // _WINDOWS

console_task::console_task (QObject *parent, cl_arguments *args_arg) : QObject (parent) 
{ 
  app = static_cast <QApplication *> (parent);
  args = args_arg;
}

void console_task::cleanup ()
{
    // Actually if gengetopt exited this cleanup won't be made
    // For now let's ignore it
    FREE (main_window_inst);
    FREE (args);
}

void console_task::run ()
{
  app->exit (start_console_processing (args));
}