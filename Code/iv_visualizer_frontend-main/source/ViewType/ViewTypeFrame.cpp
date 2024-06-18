#include "ViewTypeFrame.h"
#include "../../build/ui_viewtypeframe.h"

ViewTypeFrame::ViewTypeFrame(QWidget *a_parent)
    : QFrame(a_parent)
    , m_ui(new Ui::ViewTypeFrame)
{
    m_ui->setupUi(this);
}

ViewTypeFrame::~ViewTypeFrame()
{
    delete m_ui;
}
