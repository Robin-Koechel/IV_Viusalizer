#include "VideoPipe.h"
#include "../FrameData/Data/PipelineData.h"
#include <QGraphicsPixmapItem>

// Constructor
// Constructor
VideoPipe::VideoPipe(PipelineObject* m_next_pipe)
    : PipelineObject(m_next_pipe)
{
}

// Destructor
VideoPipe::~VideoPipe()
{
    // Destructor implementation for VideoPipe
}

// draws the frame image onto a QGraphicsScene
void VideoPipe::run(PipelineData& a_data)
{
    Image image = a_data.get_data_frame().get_image();
    a_data.draw_image(image);

    // hand the data to the next pipe
    if(&m_next_pipe!=NULL){
        m_next_pipe->run(a_data);
    }
}
