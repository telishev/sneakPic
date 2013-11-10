#ifndef QT_UTILS_H
#define QT_UTILS_H

class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QWidget;

QWidget *create_inner_widget (QWidget *parent);
QGridLayout *create_common_grid_layout (QWidget *parent);
QVBoxLayout *create_common_vbox_layout (QWidget *parent);
QHBoxLayout *create_common_hbox_layout (QWidget *parent);

QGridLayout *create_inner_grid_layout (QWidget *parent);
QHBoxLayout *create_inner_hbox_layout (QWidget *parent);
QVBoxLayout *create_inner_vbox_layout (QWidget *parent);

void finish_with_spacer (QVBoxLayout *layout);
void finish_with_spacer (QGridLayout *layout);
void finish_with_spacer (QHBoxLayout *layout);
#endif // QT_UTILS_H

