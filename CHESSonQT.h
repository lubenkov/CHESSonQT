#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CHESSonQT.h"

class CHESSonQT : public QMainWindow
{
    Q_OBJECT

public:
    CHESSonQT(QWidget *parent = nullptr);
    ~CHESSonQT();

private:
    Ui::CHESSonQTClass ui;
};
