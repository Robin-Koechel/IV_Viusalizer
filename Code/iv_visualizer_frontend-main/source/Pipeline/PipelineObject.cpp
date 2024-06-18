#include "PipelineObject.h"

PipelineObject::PipelineObject(PipelineObject* a_next_pipe): m_next_pipe(a_next_pipe){}


PipelineObject::~PipelineObject(){
    if(&m_next_pipe!=NULL){
        delete this->m_next_pipe;
    }
}

