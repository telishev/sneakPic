#include <QFileInfo>
#include <QPainter>
#include <QStringList>

#include "console/console.h"

#include "console/gengetopt/cl_arguments_class.h"

#include "common/memory_deallocation.h"

#include "renderer/svg_renderer.h"

#include "svg/svg_document.h"


void render_to_image (svg_document *doc, QString file_name, int width, int height)
{
  QImage image_output (width, height, QImage::Format::Format_ARGB32);
  QPainter painter;

  painter.begin (&image_output);
  QRect rect = QRect (0, 0, width, height);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  painter.setTransform (QTransform ());
    doc->update_items ();

  svg_renderer renderer (nullptr);
  renderer.draw_item (doc->root (), painter, rect, QTransform ());
  painter.end ();
  image_output.save (file_name);
}

int start_console_processing (cl_arguments *args)
{
  QString input_file_name = args->get_target_files () [0]; // TODO:Process all
  QString png_output_file_name = args->get_render_png ();
  QString svg_output_file_name = args->get_save ();
  svg_document *doc = new svg_document;
  double width, height;
  // Our first task is process .svg file in any mode
  if (!doc->read_file (input_file_name))
    {
      printf ("Error: document can't be processed\n");
      return 1;
    }
  
  doc->get_doc_dimensions (width, height);

  if (!png_output_file_name.isEmpty ())
    render_to_image (doc, png_output_file_name, qRound (width), qRound (height));

  if (!svg_output_file_name.isEmpty ())
    doc->write_file (svg_output_file_name);

  FREE (doc);
  return 0; // Success
}
