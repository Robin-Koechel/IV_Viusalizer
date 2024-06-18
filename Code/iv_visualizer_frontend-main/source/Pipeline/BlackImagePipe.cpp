#include "BlackImagePipe.h"
#include "../FrameData/Data/PipelineData.h"
#include <QPen>
#include <QImage>
#include <QColor>

BlackImagePipe::BlackImagePipe(PipelineObject* a_next_pipe): PipelineObject(a_next_pipe) {
    QImage image(0, 0, QImage::Format_RGB888);
    image.fill(Qt::black);  
    m_black_image = new Image(image, 0);
}

// Destructor
BlackImagePipe::~BlackImagePipe(){
    delete m_black_image;
}

// this method draws all annotations to the image
void BlackImagePipe:: run(PipelineData& a_data){

    if(m_black_image->width() != a_data.get_data_frame().get_image().width() || m_black_image->height() != a_data.get_data_frame().get_image().height()){
        delete m_black_image;
        QImage image(a_data.get_data_frame().get_image().width(), a_data.get_data_frame().get_image().height(), QImage::Format_RGB888);
        image.fill(Qt::black);  
        m_black_image = new Image(image, 0);    
    
    }

    a_data.draw_image(*m_black_image);

    // hand the data to the next pipe
    if(&m_next_pipe!=NULL){
        this->m_next_pipe->run(a_data);
    }
}
