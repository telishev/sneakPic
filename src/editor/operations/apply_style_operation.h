#ifndef APPLY_STYLE_OPERATION_H
#define APPLY_STYLE_OPERATION_H

class abstract_svg_item;
class settings_t;

class apply_style_operation
{
  const settings_t *m_settings;
public:
  apply_style_operation (const settings_t *settings);
  ~apply_style_operation ();

  void apply (abstract_svg_item *svg_item);

};

#endif // APPLY_STYLE_OPERATION_H
