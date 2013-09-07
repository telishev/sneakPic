/// \file main.cpp

#include <QApplication>
#include <QTimer>
#include <memory>

#include "main_window.h"

#include "common/memory_deallocation.h"
#include "common/common_utils.h"

#include "console/console.h"
#include "console/console_task.h"
#include "console/gengetopt/cl_arguments_class.h"



#ifdef _WINDOWS
#include <consoleapi.h>
#endif // _WINDOWS

#include "gtest/gtest.h"


int main (int argc, char **argv)
{
  ::testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
  QApplication app (argc, argv);
  std::unique_ptr<main_window> mainWin;
  std::unique_ptr<cl_arguments> args (new cl_arguments);

  if (app.arguments ().size () == 1) // If QT eaten all arguments, then it means that program needs to run GUI
    {
      mainWin.reset (new main_window);
      mainWin->show ();
    }
  else
    {
      if (args->init (argc, argv))
        return 0;

      console_task *task = new console_task (&app, args.get ());
      QTimer::singleShot (0, task, SLOT (run ()));
    }

  return app.exec ();
}
