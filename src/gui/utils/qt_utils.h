#ifndef QT_UTILS_H
#define QT_UTILS_H

class QDoubleSpinBox;
class QGridLayout;
class QHBoxLayout;
class QLayout;
class QVBoxLayout;
class QWidget;
class QDialogButtonBox;
class QDialog;

// Creates widget with 0 content offsets, useful as simple container with some layout
QWidget *create_container_widget (QWidget *parent = nullptr);
QGridLayout *create_common_grid_widget (QWidget *parent = nullptr);
QVBoxLayout *create_common_vbox_widget (QWidget *parent = nullptr);
QHBoxLayout *create_common_hbox_widget (QWidget *parent = nullptr);

QGridLayout *create_intermediate_grid_layout (QWidget *parent = nullptr);
QHBoxLayout *create_intermediate_hbox_layout (QWidget *parent = nullptr);
QHBoxLayout *create_intermediate_hbox_layout (QLayout *parent_layout);
QVBoxLayout *create_intermediate_vbox_layout (QWidget *parent = nullptr);
QVBoxLayout *create_intermediate_vbox_layout (QLayout *parent_layout);
void finish_with_spacer (QGridLayout *layout);
QDoubleSpinBox *create_double_spinbox (QWidget *parent, double maximum, int precision = 3, double single_step = 0.1, double minimum = 0.0);
QDialogButtonBox *create_standard_button_box (QDialog *dialog);
QVBoxLayout *create_common_vbox_layout (QWidget *widget);
QHBoxLayout *create_common_hbox_layout (QWidget *widget);
#endif // QT_UTILS_H

