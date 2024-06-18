#include "VideoNavigator.h"
#include "../../build/ui_videonavigator.h"
#include <QKeyEvent>

VideoNavigator::VideoNavigator(uint64_t a_time_to_navigate, QWidget *a_parent) :
    QFrame(a_parent), m_ui(new Ui::VideoNavigator),
    m_time_to_navigate(a_time_to_navigate)
{
    m_ui->setupUi(this);
}

VideoNavigator::VideoNavigator(QWidget *a_parent) :
    QFrame(a_parent), m_ui(new Ui::VideoNavigator),
    m_time_to_navigate(0) 
{
   m_ui->setupUi(this);
}



VideoNavigator::~VideoNavigator()
{
    delete m_ui;
}

