#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include <QDialog>

class options_dialog_ui;
class settings_t;

class options_dialog : public QDialog
{
  unique_ptr <options_dialog_ui> ui;
  settings_t *m_settings;

public:
  options_dialog (settings_t *settings_arg);
  ~options_dialog ();
  void init();

private slots:
  void apply_values ();
};

#endif // OPTIONS_DIALOG_H

