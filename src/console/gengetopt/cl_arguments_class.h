#ifndef CL_ARGUMENTS_CLASS_H
#define CL_ARGUMENTS_CLASS_H

class QString;
class QStringList;

struct gengetopt_args_info;

class cl_arguments
{
  gengetopt_args_info *args_info;
public:
  cl_arguments ();
  ~cl_arguments ();

  int init (int argc, char **argv);

  QString get_render_png ();
  QString get_save ();
  QStringList get_target_files ();
};
#endif // CL_ARGUMENTS_CLASS_H
