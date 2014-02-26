#include "gui/connection.h"
#include "gui/options_dialog.h"
#include "gui/settings.h"
#include "gui/utils/qt_utils.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>


class options_dialog_ui
{
  QDialog *m_dialog;
  QCheckBox *m_show_next_curve_preview_checkbox;

public:
  options_dialog_ui (QDialog *dialog)
  {
    m_dialog = dialog;
    auto layout = qt_utils::create_common_vbox_layout (dialog);
    layout->addWidget (m_show_next_curve_preview_checkbox = new QCheckBox (settings_t::user_string (option_id::NEXT_CURVE_PREVIEW)));
    layout->addWidget (qt_utils::create_standard_button_box (dialog));
  }

  QCheckBox *show_next_curve_preview_checkbox () { return m_show_next_curve_preview_checkbox; }
};

options_dialog::options_dialog (settings_t *settings_arg)
{
  m_settings = settings_arg;
  ui.reset (new options_dialog_ui (this));
  CONNECT (this, &options_dialog::accepted, this, &options_dialog::apply_values);
  setWindowFlags (Qt::Tool);
  init ();
}

void options_dialog::apply_values ()
{
  m_settings->set_value (option_id::NEXT_CURVE_PREVIEW, ui->show_next_curve_preview_checkbox ()->isChecked ());
  m_settings->settings_updated ();
}

options_dialog::~options_dialog ()
{
}

void options_dialog::init ()
{
  ui->show_next_curve_preview_checkbox ()->setChecked (m_settings->value (option_id::NEXT_CURVE_PREVIEW).toBool ());
}

