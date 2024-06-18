#include "Writer.h"

Writer::Writer(QString a_url) : m_url(a_url)
{

}

QString Writer::get_url(){
    return this->m_url;
}