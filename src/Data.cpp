/* 
 * File:   Data.cpp
 * Author: tomko
 * 
 * Created on 9 kwiecień 2012, 15:50
 */

#include "../include/Data.h"

#include <fstream>
#include <cmath>
#include <algorithm>
#include "../libmfcc/libmfcc.h"

//TODO usunąć
#include <iostream>
#include <fstream>
#include <bits/stl_iterator_base_funcs.h>
#include <stdexcept>

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
  prepared = -1;
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
  fft_good = length / 2 + 1;
  min_energy = log10(length * 3.2);
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

bool Data::isSpeechInside(int start, int end)
{
  double energy = 0;
  for (int i = start; i != end; ++i)
  {
    energy += pow(data[i] - TALSA::SIGNAL0, 2);
  }
  std::cout << energy << "\n";
  return energy > (end - start) * 4;
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
  if (getFrameEnergy(n) > min_energy)
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

void Data::prepareWindow(int n)
{
  if (prepared == n) //jeśli to okno już przygotowano
  {
    return;
  }
  prepared = n;
  int start_fragment = n * window_start;
  int end_fragment = start_fragment + window_length;
  int length = end_fragment - start_fragment;
  if (!fft)
  {
    initFFT(length);
  }
  //kopiowanie okna
  copyWindow(start_fragment, end_fragment);
  //preemfaza
  preemphasis(length);
  //skalowanie
  scale(length);
  //okienkowanie
  HammingWindow(length);
  //fft
  calcFFT(length);
}

std::vector<double> Data::getMFCCFromFrame(int n)
{
  prepareWindow(n);
  //liczenie mfcc
  std::vector<double> tmp(12);
  /*/std::ofstream file("data_after_fft.dat", std::ios::out | std::ios::app);
  {
    for (int i = 0; i < length; ++i)
    {
      file << data_after_fft[i] << " ";
    }
    file << "\n";
  }*/
  for (int i = 0; i < 12; ++i)
  {
    tmp[i] = GetCoefficient(data_after_fft, sample_frequency, 12, fft_good, i);
  }
  return tmp;
}

void Data::copyWindow(int a, int b)
{
  for (int i = a; i < b; ++i)
  {
    data_before_fft[i - a] = data[i] - TALSA::SIGNAL0;
  }
}

void Data::scale(int length)
{
  double max = 0;
  double tmp;
  for (int i = 0; i < length; ++i) //szukanie maksimum
  {
    if ((tmp = fabs(data_before_fft[i])) > max)
    {
      max = tmp;
    }
  }
  for (int i = 0; i < length; ++i) //skalowanie
  {
    data_before_fft[i] = 100 * data_before_fft[i] / max;
  }
}

void Data::preemphasis(int length)
{
  double tmp = data_before_fft[0];
  double tmp2 = data_before_fft[0];
  for (int i = 1; i < length; ++i)
  {
    tmp2 = data_before_fft[i];
    data_before_fft[i] = data_before_fft[i] - tmp * 0.95;
    tmp = tmp2;
  }
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
  plan = fftw_plan_r2r_1d(window_length, data_before_fft, data_after_fft, FFTW_R2HC, FFTW_ESTIMATE);
}

void Data::calcFFT(int length)
{
  fftw_execute(plan);
  //liczenie amplitudy
  data_after_fft[0] = fabs(data_after_fft[0]);
  for (int i = 1; i < length / 2; ++i) //zamiana na moduł
  {
    data_after_fft[i] = sqrt(pow(data_after_fft[i], 2) + pow(data_after_fft[length - i], 2));
  }
  spectalMoment0Val = -1;
  normalizedMoment1Val = -1;
}

double Data::getFrequencyFromSpectrum(int i) const
{
  return i * getSampleFrequency() / window_length;
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

std::vector<double> Data::get3Formants(int frame)
{
  prepareWindow(frame);
  std::vector<double> ret;
  //przepisanie danych do wektora
  std::vector<double> data;
  for (int i = 0; i < fft_good; i++)
  {
    data.push_back(data_after_fft[i]);
  }
  //pomocnicze zmienne
  std::vector<double>::iterator it;
  int n;
  int iend;
  int buffor = 5;
  //skala osi x
  double fk = sample_frequency / window_length;
  //test
  for (int i = 0; i < 6; ++i)
  {
    data[i] = 0;
  }
  //test
  //szukanie trzech maksimów 
  for (int i = 0; i < 3; i++)
  {
    it = std::max_element(data.begin(), data.end());
    n = std::distance(data.begin(), it);
    ret.push_back(n * fk / 8000);
    iend = n + buffor > fft_good ? fft_good : n + buffor;
    for (int j = n - buffor > 0 ? n - buffor : 0; j < iend; ++j)
    {
      data[j] = 0;
    }
  }
  std::sort(ret.begin(), ret.end());
  return ret;
}

void Data::findPhonemeBorders()
{
  int signal_max = 127;
  //szukanie pierwszej i ostatniej ramki z mową
  int first, last;
  int max = getWindowsNumber();
  first = 0;
  while (first < max && !isFrameWithSpeech(first++)); //pierwsza ramka
  last = max;
  while (last > 0 && !isFrameWithSpeech(last--));
  first -= (first > 10 ? 10 : first);
  last += (max - last > 10 ? 10 : max - last);
  std::cout << first * window_start << " " << last * window_start << "\n";
  int first_sample = first * window_start;
  //progi
  std::vector<double> thresholds;
  int max_it = 6;
  double scale = 0.01 * signal_max;
  scale /= max_it;
  for (int i = 1; i < max_it; ++i) thresholds.push_back(i * scale);
  scale = 0.1 * signal_max - 0.01 * signal_max;
  double bias = 0.01 * signal_max;
  max_it = 25;
  scale /= max_it;
  for (int i = 1; i < max_it; ++i) thresholds.push_back(i * scale + bias);
  scale = 0.9 * signal_max - 0.1 * signal_max;
  bias = 0.1 * signal_max;
  max_it = 15;
  scale /= max_it;
  for (int i = 1; i < max_it; ++i) thresholds.push_back(i * scale + bias);
  scale = 0.99 * signal_max - 0.9 * signal_max;
  bias = 0.9 * signal_max;
  max_it = 25;
  scale /= max_it;
  for (int i = 1; i < max_it; ++i) thresholds.push_back(i * scale + bias);
  scale = signal_max * 0.01;
  bias = 0.99 * signal_max;
  max_it = 6;
  scale /= max_it;
  for (int i = 1; i < max_it; ++i) thresholds.push_back(i * scale + bias);

  std::ofstream thresfile("thres.dat", std::ios::out);
  for (auto t : thresholds)
  {
    thresfile << t << "\n";
  }
  thresfile.close();

  //sprawdzenie czy jest mowa
  if (first >= last)
  {
    return;
  }
  //liczenie LCR
  std::vector<int> lcr((last - first) * window_start + window_length);
  int i = first * window_start;
  for (int & l : lcr)
  {
    for (auto t : thresholds)
    {
      if ((fabs(data[i] - TALSA::SIGNAL0) - t) * (fabs(data[i - 1] - TALSA::SIGNAL0) - t) < 0)
      {
        ++l;
      }
    }
    ++i;
  }
  std::ofstream fileclr("LCR.dat", std::ios::out);
  for (int i = first_sample; i > 0; --i)
  {
    fileclr << "0 ";
  }
  for (auto l : lcr)
  {
    fileclr << l << " ";
  }
  fileclr.close();

  //liczenie ALCR
  std::vector<double> alcr(lcr.size() - 200);
  std::vector<int>::iterator begin, end;
  begin = lcr.begin();
  end = lcr.begin() + 200;
  int sum;
  for (double & a : alcr)
  {

    std::for_each(begin++, end++, [&a](int x)
    {
                  a += x;
    });
    a /= 200;
    if (a < 0.1)
    {
      a = 0;
    }
  }

  std::ofstream file("ALCR.dat", std::ios::out);

  for (int i = first_sample + 100; i > 0; --i)
  {
    file << "0 ";
  }
  for (auto a : alcr)
  {
    file << a << " ";
  }

  std::vector<int> segments;
  double half_of_local_min = 0.01;
  int half_of_local_min_in_samples = sample_frequency * half_of_local_min;
  double min_segment_duration = 0.012;
  int min_segment_duraion_in_samples = sample_frequency * min_segment_duration;
  int mins_back;
  //liczenie granic pomiędzy fonemamia
  int size = alcr.size() - half_of_local_min_in_samples;
  for (int i = half_of_local_min_in_samples; i < size; ++i)
  {
    //przypadek, gdy mamy 0
    if (!alcr[i])
    {
      segments.push_back(i);
      while (++i < size && !alcr.at(i));
      segments.push_back(i);
      mins_back = 0;
    }
    else //inna lcizba
    {
      if (alcr[i] <= alcr[i - 1] && alcr[i] <= alcr[i + 1]) //minimum lokalne
      {
        bool ok = true;
        for (int k = 2; k < half_of_local_min_in_samples; ++k)
        {
          if (alcr[i] > alcr[i - k] || alcr[i] > alcr[i + k])//nie jest to minimum w otoczeniu 
          {
            ok = false;
            break;
          }
        }
        if (ok) // jest dobre minimum
        {
          segments.push_back(i);
          i += min_segment_duraion_in_samples - half_of_local_min_in_samples;
        }
      }
      /*   if (alcr[i] <= alcr[i - 1]) // mniejsze od poprzedniego, zwiększamy licznik
         {
           //szukamy minimum
           if (++mins_back == half_of_local_min_in_samples) // mamy minimum wstecz, sprawdzamy wprzód, szukanie najbliższego minimum lokalnego, które ma otoczenie 
           {
             mins_back = 0;
             bool ok = false;
             while (!ok)
             {
               ok = true;
               int local_max_it = half_of_local_min_in_samples + i;
               for (int k = i + 1; k < local_max_it; ++k) //iteracja w przód
               {
                 if (alcr[i] > alcr[k]) //znaleziono mniejszą liczbę
                 {
                   i = k;
                   ok = false;
                   break; //zatrzymanie pętli
                 }
               }
             }
             segments.push_back(i);
             i += min_segment_duraion_in_samples - half_of_local_min_in_samples;
           }
         }
         else //większe od poprzedniego, zerowanie licznika
         {
           mins_back = 0;
         }**/
    }
  }

  //przejście na numery próbek
  for (auto & a : segments)
  {
    a += 100 + half_of_local_min + first_sample;
  }

  std::ofstream segfile("segments.dat", std::ios::out);
  for (auto a : segments)
  {
    segfile << a << "\n";
  }
  segfile.close();

  std::cout << "analiza segmentów\n";
  //analiza segmentów
  //inicjalizacja fft
  double data_after_fft[1024];
  double data_before_fft[512];
  double autocor[1024];
  fftw_plan plan = fftw_plan_r2r_1d(1024, autocor, data_after_fft, FFTW_R2HC, FFTW_ESTIMATE);
  // /analzia 
  std::ofstream wektor("wektor.dat", std::ios::out);
  std::ofstream wektor_desc("wektor_desc.dat", std::ios::out);
  std::ofstream before("before.dat", std::ios::out);
  std::ofstream autokor("autokor.dat", std::ios::out);
  std::ofstream after("after.dat", std::ios::out);
  int seg_size = segments.size();
  int thres_out = 10000;
  for (int i = 1; i < seg_size; ++i)
  {
    if (!isSpeechInside(segments[i - 1], segments[i]))
    {
      std::cout << "Cont\n";
      continue;
    }
    wektor_desc << segments[i - 1] << " " << segments[i] << "\n";
    //przepisanie wartości
    int j;
    int it_data_fft = 0;
    for (j = segments[i - 1]; j < segments[i] && it_data_fft < 512; ++j)
    {
      data_before_fft[it_data_fft++] = (data[j] - TALSA::SIGNAL0);
    }
    for (; it_data_fft < 512; ++it_data_fft)
    {
      data_before_fft[it_data_fft] = 0;
    }
    //TODO wypisanie wejścia
    for (int i = 0; i < 512; i++)
    {
      before << data_before_fft[i] << " ";
    }
    before << "\n";
    //autokorelacja
    for (int i = 0; i < 512; ++i)
    {
      autocor[i] = 0;
      autocor[1023 - i] = 0;
      for (int j = 0; j < i + 1; ++j)
      {
        autocor[1023 - i] = autocor[i] += data_before_fft[j] * data_before_fft[511 - i + j];
      }
    }
    //TODO wypisanie korelacji
    for (int i = 0; i < 1024; ++i)
    {
      autokor << autocor[i] << " ";
    }
    autokor << "\n";
    fftw_execute(plan);
    //liczenie amplitudy
    data_after_fft[0] = fabs(data_after_fft[0]);
    for (int i = 1; i < 512; ++i) //zamiana na moduł
    {
      data_after_fft[i] = sqrt(pow(data_after_fft[i], 2) + pow(data_after_fft[1024 - i], 2));
    }
    //TODO wypisanie po fft
    for (int i = 0; i < 513; ++i)
    {
      after << data_after_fft[i] << " ";
    }
    after << "\n";
    for (int i = 0; i < 513; ++i) //zapis do pliku
    {
      wektor << (data_after_fft[i] > thres_out) << " "; //zamiana na wektor binarny
    }
    wektor << "\n";
  }
  wektor.close();
  wektor_desc.close();
}
