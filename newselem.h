#ifndef NEWSELEM_H
#define NEWSELEM_H

#include <QWidget>

namespace Ui {
class NewsElem;
}

class NewsElem : public QWidget
{
    Q_OBJECT

public:
    explicit NewsElem(QWidget *parent = nullptr);
    ~NewsElem();

private:
    Ui::NewsElem *ui;
};

#endif // NEWSELEM_H
