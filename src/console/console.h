#ifndef CONSOLE_H
#define CONSOLE_H
class QStringList;

class cl_arguments;

class QString;
class svg_document;

class console_renderer
{
public:
  int start_console_processing (cl_arguments *args);
};

#endif // !CONSOLE_H
