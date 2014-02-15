#include <QApplication>
#include <QClipboard>
#include <QImage>

#include "items/svg_item_image.h"

#include "svg/copy_paste_handler.h"
#include "svg/svg_document.h"

#include "editor/operations/add_item_operation.h"
#include "editor/items_selection.h"

#include "renderer/svg_painter.h"
#include "attributes/svg_attribute_xlink_href.h"
#include "attributes/svg_attributes_length_type.h"
#include "gui/gl_widget.h"
#include "internal_clipboard_format.h"

void copy_paste_handler::copy ()
{
  items_selection *selection = m_painter->selection ();
  if (selection->empty ())
    return;

  internal_clipboard_format format (m_painter->document ());

  format.fill_by_selection (selection);
  QClipboard *clipboard = QApplication::clipboard();
  QMimeData *data = new QMimeData;
  data->setData (format.mime_type (), format.pack ());
  clipboard->setMimeData (data);
}

void copy_paste_handler::paste ()
{
  const QClipboard *clipboard = QApplication::clipboard();
  const QMimeData *mimeData = clipboard->mimeData();
  internal_clipboard_format format (m_painter->document ());
  QPointF pos = local_cursor_pos ();

  if (mimeData->hasFormat (format.mime_type ()))
    {
      format.unpack (mimeData->data (format.mime_type ()));
      format.apply_to_doc (m_painter, pos);
    }
  else if (mimeData->hasImage())
    {
      QImage image = qvariant_cast<QImage> (mimeData->imageData());
      pos.setX (pos.x () - image.width () / 2);
      pos.setY (pos.y () - image.height () / 2);
      paste_image (image, pos);
    }
}

void copy_paste_handler::paste_image (const QImage &image, const QPointF &pos)
{
  auto *image_item = m_painter->document ()->create_new_svg_item<svg_item_image> ();

  image_item->get_attribute_for_change <svg_attribute_x> ()->set_value (pos.x ());
  image_item->get_attribute_for_change <svg_attribute_y> ()->set_value (pos.y ());
  image_item->get_attribute_for_change <svg_attribute_width> ()->set_value (image.width ());
  image_item->get_attribute_for_change <svg_attribute_height> ()->set_value (image.height ());
  image_item->get_attribute_for_change <svg_attribute_xlink_href> ()->iri ()->create_from_image (image);

  add_item_operation (m_painter).apply (image_item);
  m_painter->document ()->apply_changes ();
}

copy_paste_handler::copy_paste_handler (svg_painter *painter)
{
  m_painter = painter;
}

const QPointF &copy_paste_handler::local_cursor_pos ()
{
  return m_painter->get_local_pos (m_painter->glwidget ()->cursor_pos ());
}


