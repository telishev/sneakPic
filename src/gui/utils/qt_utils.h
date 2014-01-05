#ifndef QT_UTILS_H
#define QT_UTILS_H

class QDoubleSpinBox;
class QGridLayout;
class QHBoxLayout;
class QLayout;
class QVBoxLayout;
class QWidget;

// Creates widget with 0 content offsets, useful as simple container with some layout
QWidget *create_container_widget (QWidget *parent = nullptr);
QGridLayout *create_common_grid_layout (QWidget *parent = nullptr);
QVBoxLayout *create_common_vbox_layout (QWidget *parent = nullptr);
QHBoxLayout *create_common_hbox_layout (QWidget *parent = nullptr);

QGridLayout *create_inner_grid_layout (QWidget *parent = nullptr);
QHBoxLayout *create_inner_hbox_layout (QWidget *parent = nullptr);
QHBoxLayout *create_inner_hbox_layout (QLayout *parent_layout);
QVBoxLayout *create_inner_vbox_layout (QWidget *parent = nullptr);
QVBoxLayout *create_inner_vbox_layout (QLayout *parent_layout);
void finish_with_spacer (QGridLayout *layout);
QDoubleSpinBox *create_double_spinbox (QWidget *parent, double maximum, int precision = 3, double single_step = 0.1, double minimum = 0.0);
#endif // QT_UTILS_H

