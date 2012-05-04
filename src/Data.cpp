/* 
 * File:   Data.cpp
 * Author: tomko
 * 
 * Created on 9 kwiecień 2012, 15:50
 */

#include "../include/Data.h"

#include <fstream>
#include <cmath>
#include "../libmfcc/libmfcc.h"

//TODO usunąć
#include <iostream>

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
    data = new std::uint8_t [new_mem];
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
    avg += (double) data[i] / (double) mem_size;
  }
  std::cout << avg << "\n";
  for (int i = 0; i < mem_size; i++)
  {
    data[i] -= (int) avg;
  }
}

void Data::saveRawDataToFile(std::string filename)
{
  std::ofstream file(filename, std::ios::out);
  {
    for (int i = 0; i < mem_size; i++)
    {
      file << (int) (data[i] - TALSA::SIGNAL0) << "\n";
    }
  }
  file.close();
}

//TODO usunąć - funkcja testowa

void Data::test()
{
  std::ofstream file("test.dat", std::ios::out);
  double t = 0.020; //czas trwania okna to 20 ms
  int n = 0.02 * 8000.0;
  int nint = 5 / t;
  for (int j = 0; j < nint; ++j)
  {
    int energy = 0;
    for (int i = 1; i < n; ++i) //dla każdej próbki
    {
      energy += pow(data[i + j * n] - TALSA::SIGNAL0, 2);
      /*      if (data[i + j * n] * data[i + j * n - 1] < 0)
            {
              ++energy;
            }
       **/    }
    file << energy << "\n";
  }
  file.close();
}

/*/void Data::setFrameLength(double length, double overlap)
{
  window_length_in_s = length;
  window_overlap = overlap;
  window_length = length * sample_frequency;
  //  std::cout << window_length << "\n";
  window_start = window_length * (1 - overlap);
  //  std::cout << window_start << "\n";
}**/

void Data::setFrameLength(int length, int overlap)
{
  window_length = length;
  window_start = length - overlap;
}

int Data::getSampleFrequency() const
{
  return sample_frequency;
}

void Data::setSampleFrequency(int sf)
{
  sample_frequency = sf;
}

bool Data::isFrameWithSpeech(int n)
{
  int window_end = window_start * (n + 1);
  if (window_end > mem_size - 1)
  {
    window_end = mem_size - 1;
  }
  /*  int zero_crossing = 0;
    for (int i = window_start * n; i < window_end; ++i)
    {
      if (data[i + 1] * data[i] < 0)
      {
        ++zero_crossing;
      }
    }
   * **/
  //  if (zero_crossing > TALSA::MAX_NOISE_ZERO_CROSSING)
  double energy = 0;
  for (int i = window_start * n; i < window_end; ++i)
  {
    energy += pow(data[i] - TALSA::SIGNAL0, 2);
  }
  if (energy > TALSA::MIN_ENERGY)
  {
    return true;
  }
  return false;
}

int Data::getWindowsNumber() const
{
  int i = 1;
  int start = 0;
  while ((start = window_start * i++) + window_length < mem_size); //liczenie ilości ramek
  return i;
}

std::vector<double> Data::getMFCCFromFrame(int n)
{
  int start_fragment = n * window_start;
  int end_fragment = (n+1) * window_start;
  //skalowanie
  scale(start_fragment, end_fragment);
  //preemfaza
  preemphasis(start_fragment, end_fragment);
  //okienkowanie
  HammingWindow(start_fragment, end_fragment);
  //fft
  fft(start_fragment, end_fragment);
  //liczenie mfcc
  std::vector<double> tmp(12);
  for (int i =0; i < 12; i++)
  {
    tmp[i] = GetCoefficient(data_feature, sample_frequency, 12, 128, i);
  }
  return tmp;
}

