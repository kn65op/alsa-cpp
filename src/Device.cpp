/* 
 * File:   Device.cpp
 * Author: tomko
 * 
 * Created on 7 kwiecień 2012, 16:33
 */

#include "../include/Device.h"

using namespace TALSA;

Device::Device()
{
  clear();
  data_format = DataFormat::U8;
}

Device::~Device()
{
  close();
}

void Device::close()
{
  if (handle) //zapobieganie dwukrotnemu zamnięciu.
  {
    snd_pcm_close(handle);
    clear();
  }
}

void Device::open(AccessMode am) throw (WrongArgument, InvalidOperation)
{
  if (handle) // urządzenie jest już otwarte
  {
    throw InvalidOperation("Device is already opened.");
  }
  access_mode = am; //zapisanie trybu otwarcia
  snd_pcm_stream_t mode;
  switch (am) //wybór trybu otwarcia
  {
    case AccessMode::READ:
      mode = SND_PCM_STREAM_CAPTURE;
      break;
    case AccessMode::WRITE:
      mode = SND_PCM_STREAM_PLAYBACK;
      break;
    default:
      throw WrongArgument("At this moment you can only read or write.");
      break;
  }
  int err = snd_pcm_open(&handle, device.c_str(), mode, 0); //otwarcie
  if (err < 0)
  {
    throw InvalidOperation(snd_strerror(err));
  }
  if ((err = snd_pcm_set_params(handle,
                                DataFormatHelper::getFormat(data_format),
                                SND_PCM_ACCESS_RW_INTERLEAVED,
                                1,
                                20000,
                                1,
                                500000)) < 0)
  {
    throw InvalidOperation(snd_strerror(err));
  }
}

void Device::setDataFormat(DataFormat df)
{
  data_format = df;
}

void Device::clear()
{
  handle = 0;
  access_mode = AccessMode::INVALID;
}

void Device::setDevice(const std::string dev)
{
  device = dev;
}

void Device::write(Data& d) throw (WrongArgument, InvalidOperation)
{
  if (access_mode != AccessMode::WRITE)
  {
    throw InvalidOperation("Device hasn't been opened for writing");
  }
  checkData(d);
  int tmp = snd_pcm_writei(handle, d.data, d.getSize());
  if (tmp < 0)
  {
    snd_pcm_recover(handle, tmp, 0); //w przypadku błędu przywracanie stanu do poprawnego
  }
  if (tmp < 0)
  {
    throw InvalidOperation("Error while writing to device");
  }
  else if (tmp < d.getSize())
  {
    throw InvalidOperation("Data write is too short");
  }
}

void Device::read(Data& d) throw (WrongArgument, InvalidOperation)
{
  if (access_mode != AccessMode::READ)
  {
    throw InvalidOperation("Device hasn't been opened for reading");
  }
  checkData(d);
  int tmp = snd_pcm_readi(handle, d.data, d.getSize());
  if (tmp < 0)
  {
    snd_pcm_recover(handle, tmp, 0); //w przypadku błędu przywracanie stanu do poprawnego
  }
  if (tmp < 0)
  {
    throw InvalidOperation("Error while reading to device");
  }
  else if (tmp < d.getSize())
  {
    throw InvalidOperation("Data read is too short");
  }
}

void Device::checkData(Data & d) throw (WrongArgument)
{
  if (data_format != d.data_format)
  {
    throw WrongArgument("Data format must agree!");
  }
  if (!d.getSize())
  {
    throw WrongArgument("Data size must be positive!");
  }
  if (!d.data)
  {
    throw WrongArgument("Data must be allocated!");
  }
}

DataFormat Device::getDataFormat() const
{
  return data_format;
}
