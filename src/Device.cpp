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
  if (!handle) // urządzenie jest już otwarte
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
                                getFormat(data_format),
                                SND_PCM_ACCESS_RW_INTERLEAVED,
                                1,
                                48000,
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

snd_pcm_format_t Device::getFormat(DataFormat df)
{
  switch (data_format)
  {
    case DataFormat::U8:
      return SND_PCM_FORMAT_U8;
    case DataFormat::U16:
      return SND_PCM_FORMAT_U16;
    default :
      throw InvalidOperation("Wrong data format");
  }
}
void Device::setDevice(const std::string dev)
{
  device = dev;
}
