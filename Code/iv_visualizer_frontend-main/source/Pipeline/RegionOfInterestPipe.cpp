#include "RegionOfInterestPipe.h"
#include "../FrameData/Data/RegionOfInterest.h"


RegionOfInterestPipe::~RegionOfInterestPipe(){}

RegionOfInterestPipe::RegionOfInterestPipe(PipelineObject* m_next_pipe):PipelineObject(m_next_pipe){

}

void RegionOfInterestPipe::run(PipelineData& a_data){
    if(!a_data.get_data_frame().get_regions_of_interest().empty()){
        a_data.get_scene().init_region_of_interest(a_data.get_data_frame().get_regions_of_interest().at(0)->is_inside(), QRectF(0, 0, 
            a_data.get_data_frame().get_image().width(), a_data.get_data_frame().get_image().height()));
    }
    for(RegionOfInterest *a_region: a_data.get_data_frame().get_regions_of_interest()){
        a_data.draw_region_of_interest(*a_region);
    }

    // Call the next pipe if it exists
    if (m_next_pipe != nullptr) {
        m_next_pipe->run(a_data);
    }
}
