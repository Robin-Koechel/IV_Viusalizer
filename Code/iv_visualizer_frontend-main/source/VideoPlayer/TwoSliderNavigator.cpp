#include "TwoSliderNavigator.h"
#include "../../build/ui_twoslidernavigator.h"

TwoSliderNavigator::TwoSliderNavigator(uint64_t a_time_to_navigate, uint64_t a_time_span_of_fine_slider, QWidget *a_parent) :
    VideoNavigator(a_time_to_navigate, a_parent), m_ui(new Ui::TwoSliderNavigator),
    m_time_span_of_fine_slider(a_time_span_of_fine_slider)
{
    m_ui->setupUi(this);
    m_paint_slider = new PainterSlider();
    m_ui->roughSlider->setVisible(false);
    m_ui->roughSliderHLayout->addWidget(m_paint_slider);
    connect(m_paint_slider, &PainterSlider::sliderReleased, this, &TwoSliderNavigator::on_roughSlider_sliderReleased);
    connect(m_paint_slider, &PainterSlider::sliderPressed, this, &TwoSliderNavigator::on_roughSlider_sliderPressed);
    reset_slider();
    update_time_to_navigate();
    m_start_position_of_fine_slider = 0;
}

TwoSliderNavigator::TwoSliderNavigator(uint64_t a_time_span_of_fine_slider, QWidget *a_parent) :
    VideoNavigator(a_parent), // Achtung: Hier könnte ein Argument fehlen oder die Basisklassen-Initialisierung ist unvollständig.
    m_ui(new Ui::TwoSliderNavigator),
    m_time_span_of_fine_slider(a_time_span_of_fine_slider)
{
    m_ui->setupUi(this);
    m_paint_slider = new PainterSlider();
    m_ui->roughSlider->setVisible(false);
    m_ui->roughSliderHLayout->addWidget(m_paint_slider);
    connect(m_paint_slider, &PainterSlider::sliderReleased, this, &TwoSliderNavigator::on_roughSlider_sliderReleased);
    connect(m_paint_slider, &PainterSlider::sliderPressed, this, &TwoSliderNavigator::on_roughSlider_sliderPressed);

    reset_slider();
}

TwoSliderNavigator::~TwoSliderNavigator()
{
    delete m_ui;
}

void TwoSliderNavigator::position_changed()
{
    emit sig_pos_changed(m_paint_slider->value());
}

void TwoSliderNavigator::change_current_position(uint64_t a_new_position)
{
    if(m_slider_is_moving){
        return;
    }
    m_paint_slider->setValue(static_cast<int>(a_new_position));
    if((a_new_position - m_start_position_of_fine_slider) <= m_time_span_of_fine_slider){
        m_ui->fineSlider->setValue(static_cast<int>(a_new_position - m_start_position_of_fine_slider));
    }
    else{
        m_ui->fineSlider->setValue(0);
        change_start_position_of_fine_slider(a_new_position);
    }
}

void TwoSliderNavigator::set_time_to_navigate(uint64_t a_new_time_to_navigate)
{
   if(this->m_time_to_navigate > a_new_time_to_navigate){
        this->m_time_to_navigate = a_new_time_to_navigate;
        reset_slider();
    }
    else{
        this->m_time_to_navigate = a_new_time_to_navigate;
    }
    update_time_to_navigate();

}

void TwoSliderNavigator::update_time_to_navigate()
{
    if(m_time_span_of_fine_slider <= m_time_to_navigate){
        m_ui->fineSlider->setRange(0,m_time_span_of_fine_slider);
    }
    else{
        m_ui->fineSlider->setRange(0,m_time_to_navigate);
    }
    m_paint_slider->setRange(0, m_time_to_navigate);
}

void TwoSliderNavigator::reset_slider()
{
    m_paint_slider->setValue(0);
    m_ui->fineSlider->setValue(0);
}

void TwoSliderNavigator::change_start_position_of_fine_slider(uint64_t a_new_position)
{
    m_start_position_of_fine_slider = a_new_position;
    update_time_to_navigate(); 
}

uint64_t TwoSliderNavigator::get_current_position()
{
    return static_cast<uint64_t>(m_paint_slider->value());
}

void TwoSliderNavigator::on_fineSlider_sliderReleased()
{
    m_slider_is_moving = false;
    m_paint_slider->setValue(m_ui->fineSlider->value() + this->m_start_position_of_fine_slider);
    position_changed();
}

void TwoSliderNavigator::on_roughSlider_sliderReleased()
{
    m_slider_is_moving = false;
    change_start_position_of_fine_slider(static_cast<uint64_t>(m_paint_slider->value()));
    position_changed();
}

void TwoSliderNavigator::on_fineSlider_sliderPressed()
{
    m_slider_is_moving = true;
}

void TwoSliderNavigator::on_roughSlider_sliderPressed()
{
    m_slider_is_moving = true;
}
void TwoSliderNavigator::set_marks(const QList<uint64_t> &a_positions){
    m_paint_slider->set_marks(a_positions);
}
