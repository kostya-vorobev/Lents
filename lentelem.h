#ifndef LENTELEM_H
#define LENTELEM_H

#include <QWidget>
#include "postclass.h"

namespace Ui {
class LentElem;
}

class LentElem : public QWidget
{
    Q_OBJECT

public:
    explicit LentElem(PostClass* post, QWidget *parent = nullptr);
    ~LentElem();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::LentElem *ui;
    PostClass* post;
    int postID;
};

#endif // LENTELEM_H
