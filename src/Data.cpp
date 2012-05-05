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
#include <fstream>

using namespace TALSA;

Data::Data() : size(0)
{
  data = 0;
  data_before_fft = 0;
  data_after_fft = 0;
  fft = false;
  fft_good = 65;
  spectalMoment0Val = -1;
  normalizedMoment1Val = -1;
}

Data::~Data()
{
  if (data)
  {
    delete [] data;
    data = 0;
  }
  if (data_before_fft)
  {
    delete [] data_before_fft;
    data_before_fft = 0;
  }
  if (data_after_fft)
  {
    //delete [] data_after_fft;
    data_after_fft = 0;
  }
  if (fft)
  {
    fftw_destroy_plan(plan);
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
  /*double t = 0.016; //czas trwania okna to 20 ms
  int n = 0.016 * 8000.0;
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
//       **/ // }
  for (int i = 0; i < getWindowsNumber(); ++i)
  {
    file << getFrameEnergy(i) << "\n";
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

double Data::getFrameEnergy(int n)
{
  int window_end = window_start * (n + 1);
  if (window_end > mem_size - 1)
  {
    window_end = mem_size - 1;
  }
  double energy = 0;
  for (int i = window_start * n; i < window_end; ++i)
  {
    energy += pow(data[i] - TALSA::SIGNAL0, 2);
  }
  return log10(energy);
}

bool Data::isFrameWithSpeech(int n)
{
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
  if (getFrameEnergy(n) > TALSA::MIN_ENERGY)
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
  int end_fragment = start_fragment + window_length;
  int length = end_fragment - start_fragment;
  if (!fft)
  {
    initFFT(length);
  }
  //skalowanie
  scale(start_fragment, end_fragment);
  //preemfaza
  preemphasis(length);
  //okienkowanie
  HammingWindow(length);
  //fft
  calcFFT(length);
  //liczenie mfcc
  std::vector<double> tmp(12);
  for (int i = 0; i < 12; ++i)
  {
    tmp[i] = GetCoefficient(data_after_fft, sample_frequency, 12, 65, i);
  }
  return tmp;
}

void Data::scale(int a, int b)
{
  double max = 0;
  double tmp;
  int size = b - a;
  for (int i = a; i < b; ++i)
  {
    data_before_fft[i - a] = data[i] - TALSA::SIGNAL0;
    if ((tmp = fabs(data_before_fft[i - a])) > max)
    {
      max = tmp;
    }
  }
  for (int i = 0; i < size; ++i)
  {
    data_before_fft[i] = 100 * data_before_fft[i] / max;
  }
}

void Data::preemphasis(int length)
{
  //TODO dopisać
}

void Data::HammingWindow(int length)
{
  for (int i = 0; i < length; ++i)
  {
    data_before_fft[i] = (0.54 - 0.46 * cos((2 * M_PI * i) / (length - 1))) * data_before_fft[i];
  }
}

void Data::initFFT(int length)
{
  fft = true;
  data_after_fft = new double [length];
  data_before_fft = new double [length];
  plan = fftw_plan_r2r_1d(128, data_before_fft, data_after_fft, FFTW_R2HC, FFTW_ESTIMATE);
}

void Data::calcFFT(int length)
{
  fftw_execute(plan);
  //liczenie mfcc
  for (int i = 1; i < length / 2; ++i) //zamiana na moduł
  {
    data_after_fft[i] = sqrt(pow(data_after_fft[i], 2) + pow(data_after_fft[length - i], 2));
  }
  spectalMoment0Val = -1;
  normalizedMoment1Val = -1;
}

double Data::getFrequencyFromSpectrum(int i) const
{
  return i * getSampleFrequency() / 128.0;
}

double Data::spectralMoment0() 
{
  if (spectalMoment0Val == -1)
  {
    spectalMoment0Val = 0;
    for (int i = 0; i < fft_good; ++i)
    {
      spectalMoment0Val += data_after_fft[i];
    }
  }
  return spectalMoment0Val;
}

double Data::normalizedMoment1() 
{
  if (normalizedMoment1Val == -1)
  {
    normalizedMoment1Val = 0;
    for (int i = 0; i < fft_good; ++i)
    {
      normalizedMoment1Val += data_after_fft[i] * getFrequencyFromSpectrum(i);
    }
    normalizedMoment1Val /= spectralMoment0();
  }
  return normalizedMoment1Val;
}

double Data::normalizedCentralMoment2() 
{
  double tmp = 0;
  for (int i = 0; i < fft_good; ++i)
  {
    tmp += data_after_fft[i] * pow(getFrequencyFromSpectrum(i) - normalizedMoment1(), 2);
  }
  return tmp / spectralMoment0();
}

double Data::normalizedCentralMoment3() 
{
  double tmp = 0;
  for (int i = 0; i < fft_good; ++i)
  {
    tmp += data_after_fft[i] * pow(getFrequencyFromSpectrum(i) - normalizedMoment1(), 3);
  }
  return tmp / spectralMoment0();
}
