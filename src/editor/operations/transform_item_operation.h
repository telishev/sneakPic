#ifndef TRANSFORM_ITEM_OPERATION_H
#define TRANSFORM_ITEM_OPERATION_H

class svg_document;
class QTransform;
class abstract_svg_item;

class transform_item_operation
{
  svg_document *m_document;
public:
  transform_item_operation (svg_document *document);
  ~transform_item_operation ();

  void apply_transform (const QTransform &transform, abstract_svg_item *item);
  QTransform get_new_transform (const QTransform &transform, abstract_svg_item *item);

};

#endif // TRANSFORM_ITEM_OPERATION_H
