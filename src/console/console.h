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

private:
  void render_to_image (svg_document *doc, QString file_name, int width, int height);
};

#endif // !CONSOLE_H
