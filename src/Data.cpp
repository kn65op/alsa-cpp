/* 
 * File:   Data.cpp
 * Author: tomko
 * 
 * Created on 9 kwiecień 2012, 15:50
 */

#include <stdint.h>

//TODO usunąć
#include <iostream>

#include "../include/Data.h"

using namespace TALSA;

Data::Data() : size(0)
{
  data = 0;
}

Data::~Data()
{
  if (data)
  {
    delete [] data;
  }
}

void Data::setSize(int size)
{
  this->size = size;
  createData();
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
  createData();
}

void Data::createData()
{
  int new_mem;
  try
  {
    new_mem = size * DataFormatHelper::getSizeOfFormat(data_format);
  }
  catch (WrongArgument e) //jeśli nie podano foramtu, to poczekaj z alokacją pamięci
  {
    return;
  }
  if (new_mem != mem_size) //potrzeba innej ilości pamięci.
  {
    if (data)
    {
      delete [] data;
    }
    data = new std::int8_t [new_mem];
    mem_size = new_mem;
  }
}

void * Data::operator *()
{
  return data;
}

void Data::removeConstantComponent()
{
  double avg = 0;
  for (int i = 0; i < mem_size; i++)
  {
    avg += (double)data[i] / (double)mem_size;
  }
  std::cout << avg << "\n";
  for (int i=0; i<mem_size; i++)
  {
    data[i] -= (int)avg;
  }
}