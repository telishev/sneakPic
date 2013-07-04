#include <QPainter>
#include <QStringList>

#include "common/memory_deallocation.h"

#include "renderer/svg_renderer.h"

#include "svg/svg_document.h"

int start_console_processing (QStringList arguments)
{
  // For now our task is just to make exporting arguments[1] as a png with the same name.
  if (arguments.count () != 2)
    return 1; // TODO: print usage

  svg_document *doc = new svg_document;
  QString input_file_name = arguments[1];
  QString output_file_name = input_file_name;
  output_file_name.replace (".svg", ".png");
  if (!doc->read_file (input_file_name))
    {
      printf ("Error: document can't be processed\n");
      return 1;
    }
  double width, height;
  doc->get_doc_dimensions (width, height);
  QImage image_output ((int) width, (int) height, QImage::Format::Format_ARGB32);
  QPainter painter;
  painter.begin (&image_output);
  QRect rect = QRect (0, 0, width, height);
  painter.fillRect (rect, Qt::white);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  painter.setTransform (QTransform ());
  doc->update_items ();
  svg_renderer::draw_items (doc->root (), painter, rect, QTransform ());
  painter.end ();
  image_output.save (output_file_name);
  FREE (doc);
  return 0; // Success
}
