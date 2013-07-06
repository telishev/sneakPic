/// \file main.cpp

#include <QApplication>
#include <QTimer>

#include "main_window.h"

#include "common/memory_deallocation.h"

#include "console/console.h"
#include "console/console_task.h"
#include "console/gengetopt/cl_arguments_class.h"

#ifdef _WINDOWS
#include <consoleapi.h>
#endif // _WINDOWS

int main (int argc, char **argv)
{
  QApplication app (argc, argv);
  main_window *mainWin = 0;
  cl_arguments *args = new cl_arguments;
  bool console_inited;

  if (app.arguments ().size () == 1) // If QT eaten all arguments, then it means that program needs to run GUI
    {
      main_window *mainWin = new main_window;
      mainWin->show ();
      
    }
  else
    {
#ifdef _WINDOWS
      if (!AttachConsole (-1))
        console_inited = AllocConsole ();
      if (!console_inited)
        goto cleanup;
      freopen("CONOUT$", "wb", stdout);     
#endif // _WINDOWS
      if (args->init (argc, argv))
        goto cleanup;
      console_task *task = new console_task (&app, args);
      QTimer::singleShot (0, task, SLOT (run ()));
    }

  return app.exec ();
cleanup:
  if (console_inited)   // Actually if gengetopt exited this cleanup won't be mad
    {                   // For now let's ignore it
      fclose(stdout);
      FreeConsole();
    }
  FREE (args);
  FREE (mainWin);
}
