#include "ViewTypeFrameWidget.h"
#include "../../build/ui_viewtypeframewidget.h"
#include "ViewType.h"

// Constructor
ViewTypeFrameWidget::ViewTypeFrameWidget(QWidget *a_parent)
    : QWidget(a_parent)
    , m_ui(new Ui::ViewTypeFrameWidget)
{
    m_ui->setupUi(this);
    m_ui->rbtnGroupViewTypes->addButton(m_ui->rbtnAnnotationAndVideo, 1);
    m_ui->rbtnGroupViewTypes->addButton(m_ui->rbtnAnnotationWithoutVideo, 2);
    m_ui->rbtnGroupViewTypes->addButton(m_ui->rbtnOnlyVideo, 3);
    m_ui->rbtnOnlyVideo->setChecked(true);

}

// Destructor
ViewTypeFrameWidget::~ViewTypeFrameWidget()
{
    delete m_ui;
}

// returns the currently selected view type
const ViewType &ViewTypeFrameWidget::get_selected_view_type(){
    switch(m_ui->rbtnGroupViewTypes->id(m_ui->rbtnGroupViewTypes->checkedButton())){
    case 1:
        return this->m_annotation_with_video;
        break;
    case 2:
        return this->m_annotation_without_video;
        break;
    default:
        return this->m_only_video;
        break;
    }
}

void ViewTypeFrameWidget::on_rbtnOnlyVideo_clicked()
{
    emit sig_view_type_changed(this->m_only_video);
}


void ViewTypeFrameWidget::on_rbtnAnnotationAndVideo_clicked()
{
    emit sig_view_type_changed(this->m_annotation_with_video);
}


void ViewTypeFrameWidget::on_rbtnAnnotationWithoutVideo_clicked()
{
    emit sig_view_type_changed(this->m_annotation_without_video);
}


