#include <QFileInfo>
#include <QPainter>
#include <QStringList>

#include <memory>

#include "console/console.h"

#include "console/gengetopt/cl_arguments_class.h"

#include "common/memory_deallocation.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/svg_renderer.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"
#include "renderer/renderer_items_container.h"
#include "renderer/renderer_state.h"
#include "renderer/render_utils.h"

#include "svg/svg_document.h"
#include "svg/svg_utils.h"

#include "skia/skia_includes.h"




int console_renderer::start_console_processing (cl_arguments *args)
{
  QString input_file_name = args->get_target_files () [0]; // TODO:Process all
  QString png_output_file_name = args->get_render_png ();
  QString svg_output_file_name = args->get_save ();
  svg_document *doc = new svg_document ();
  double width, height;
  // Our first task is process .svg file in any mode
  if (!doc->read_file (input_file_name))
    {
      printf ("Error: document can't be processed\n");
      return 1;
    }

  svg_utils::get_doc_dimensions (doc, width, height);

  if (!png_output_file_name.isEmpty ())
    render_utils::render_to_image (*doc, png_output_file_name, qRound (width), qRound (height));

  if (!svg_output_file_name.isEmpty ())
    doc->write_file (svg_output_file_name);

  FREE (doc);
  return 0; // Success
}
