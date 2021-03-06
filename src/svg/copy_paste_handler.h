#ifndef COPY_PASTE_HANDLER_H
#define COPY_PASTE_HANDLER_H

class svg_document;
class svg_painter;

class copy_paste_handler
{
  svg_painter *m_painter;

public:
  copy_paste_handler (svg_painter *painter);

  void copy ();
  void paste (bool in_place);

private:
  void paste_image (const QImage &image, const QPointF &pos);
  QPointF local_cursor_pos();
};

#endif // COPY_PASTE_HANDLER_H
