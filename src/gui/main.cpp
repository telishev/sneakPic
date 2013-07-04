/// \file main.cpp

#include <QApplication>
#include <QTimer>

#include "main_window.h"

#include "common/memory_deallocation.h"

#include "console/console.h"
#include "console/console_task.h"

int main (int argc, char **argv)
{
  QApplication app (argc, argv);
  main_window *mainWin = 0;

  if (argc == 1)
    {
      main_window *mainWin = new main_window;
      mainWin->show ();
    }
  else
    {
      console_task *task = new console_task (&app);
      QTimer::singleShot(0, task, SLOT(run()));
    }

  return app.exec ();
  FREE (mainWin);
}
