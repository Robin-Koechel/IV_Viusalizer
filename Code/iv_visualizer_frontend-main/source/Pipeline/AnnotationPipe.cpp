#include "AnnotationPipe.h"
#include "../FrameData/Data/PipelineData.h"

#include <QPen>

AnnotationPipe::AnnotationPipe(PipelineObject* a_next_pipe): PipelineObject(a_next_pipe) {

}

// Destructor
AnnotationPipe::~AnnotationPipe(){}

// this method draws all annotations to the image
void AnnotationPipe:: run(PipelineData& a_data){
    foreach (BoundingBox* a_bounding_box, a_data.get_data_frame().get_bounding_boxes()) {
       
        a_data.draw_bounding_box(*a_bounding_box);
    }

    // hand the data to the next pipe
    if(&m_next_pipe!=NULL){
        this->m_next_pipe->run(a_data);
    }
}
