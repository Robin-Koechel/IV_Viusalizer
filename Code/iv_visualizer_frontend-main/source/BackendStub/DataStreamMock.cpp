#include "DataStreamMock.h"
#include "../FrameData/Data/BoundingBox.h"
#include "qdatetime.h"
#include <QPainter>
#include <QRandomGenerator>

DataStreamMock::DataStreamMock(TimeRange* a_time_range)
    : m_time_range(a_time_range), m_asked_data_frame(a_time_range->get_start_time()/33000000), m_config(nullptr) {
    QTime time = QTime::currentTime();
    
}

DataFrame* DataStreamMock::pop(bool a_create_data_frame){
    QMutexLocker locker(&m_mutex_config);
    m_asked_data_frame++;
    if(a_create_data_frame){
        this->create_data_frame();
        if (m_data.isEmpty()) {
            return nullptr; 
        }
        DataFrame* result = m_data.takeFirst(); 
        return result;

    }
    else{
        return nullptr;
    }


}

void DataStreamMock::create_data_frame(){
    QVector<BoundingBox*> bounds;
    QVector<RegionOfInterest*> regions;

    if(m_config != nullptr){
        foreach(const RegionOfInterest* roi, m_config->get_config_detector().get_rois()){
            RegionOfInterest* roiToAdd = new RegionOfInterest(roi->get_name(), roi->get_id(), roi->get_polygon(), roi->is_inside(), roi->get_opacity());
            regions.append(roiToAdd);
        }
    }

    for(int j = 0; j < 33; j++){
      std::vector<int> vec = {
            QRandomGenerator::global()->bounded(650),
            QRandomGenerator::global()->bounded(650),
            QRandomGenerator::global()->bounded(50) + 1,
            QRandomGenerator::global()->bounded(50) + 1
        };
        std::vector<int> vec2 = {
            QRandomGenerator::global()->bounded(650),
            QRandomGenerator::global()->bounded(650),
            QRandomGenerator::global()->bounded(50) + 1,
            QRandomGenerator::global()->bounded(50) + 1
        };
      std::vector<int> vec3 = {
            QRandomGenerator::global()->bounded(650),
            QRandomGenerator::global()->bounded(650),
            QRandomGenerator::global()->bounded(50) + 1,
            QRandomGenerator::global()->bounded(50) + 1
        };
        BoundingBox* b1 = new BoundingBox(vec,0.9,"horse",cv::Size2i(700, 700), BoundingBox::Format::TLWH, 0);
        BoundingBox* b2 = new BoundingBox(vec2,0.9,"car", cv::Size2i(700, 700), BoundingBox::Format::TLWH, 0);
        BoundingBox* b3 = new BoundingBox(vec3,0.9,"human", cv::Size2i(700, 700), BoundingBox::Format::TLWH, 0);

        bounds.append(b1);
        bounds.append(b2);
        bounds.append(b3);
    }

    
    add_data_frame((m_asked_data_frame) * 1000000*33, bounds, regions);
}

bool DataStreamMock::is_done(){
    return m_asked_data_frame * 1000000*33 >= m_time_range->get_end_time();
}

void DataStreamMock::add_data_frame(uint64_t a_timestamp, QVector<BoundingBox *> a_annotations, QVector<RegionOfInterest*> a_regions){
    Image image(700, 700, a_timestamp); 
    image.fill(Qt::blue);

    QPainter painter(&image);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 30));

    QString number = QString::number(m_asked_data_frame + 1);
    QRect rect = image.rect();
    painter.drawText(rect, Qt::AlignCenter, number);

    QString url= "test"; 
    DataFrame* frame = new DataFrame(url, image, a_regions, a_annotations);
    m_data.push_back(frame);
}

void DataStreamMock::stop_stream(){
}

void DataStreamMock::slot_set_config(Config* a_config){
    QMutexLocker locker(&m_mutex_config);
    if(a_config != nullptr){
        //delete this->con;
    }
    m_config = a_config;
}

DataStreamMock::~DataStreamMock(){
    delete m_time_range;
    delete m_config;
}
