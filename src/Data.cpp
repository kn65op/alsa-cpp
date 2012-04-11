/* 
 * File:   Data.cpp
 * Author: tomko
 * 
 * Created on 9 kwiecień 2012, 15:50
 */

#include "../include/Data.h"

using namespace TALSA;

Data::Data()
{
}

Data::~Data()
{
}

void Data::setSize(int size)
{
  this->size = size;
  
}

int Data::getSize() const
{
  return size;
}


DataFormat Data::getDataFormat() const
{
  return data_format;
}

void Data::setDataFormat(DataFormat data_format)
{
  this->data_format = data_format;
}
