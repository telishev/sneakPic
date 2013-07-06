#include "console/gengetopt/cl_arguments_class.h"

#include "common/memory_deallocation.h"

#include "console/gengetopt/cl_arguments.h"

#include <QStringList>

cl_arguments::cl_arguments ()
{
  args_info = new gengetopt_args_info;
}

cl_arguments::~cl_arguments ()
{
  FREE (args_info);
}

int cl_arguments::init (int argc, char **argv)
{
  return cmdline_parser (argc, argv, args_info);
}

QString cl_arguments::get_render_png ()
{
  return QString (args_info->render_png_arg);
}

QString cl_arguments::get_save ()
{
  return QString (args_info->save_arg);
}

QStringList cl_arguments::get_target_files ()
{
  QStringList list;
  for (unsigned int i = 0; i < args_info->inputs_num; i++)
    list << args_info->inputs[i];
  return list;
}
