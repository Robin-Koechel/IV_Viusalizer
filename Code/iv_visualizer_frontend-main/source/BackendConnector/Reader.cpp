#include "Reader.h"

Reader::Reader(QString a_url): m_url(a_url)
{
}

QString Reader::get_url(){
    return this->m_url;
}