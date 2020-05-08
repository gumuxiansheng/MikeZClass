#ifndef SCREENSHAREFORM_H
#define SCREENSHAREFORM_H

#include <QWidget>

namespace Ui {
class ScreenShareForm;
}

class ScreenShareForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenShareForm(QWidget *parent = nullptr);
    ~ScreenShareForm();

private:
    Ui::ScreenShareForm *ui;
};

#endif // SCREENSHAREFORM_H
