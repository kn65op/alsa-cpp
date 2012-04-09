/* 
 * File:   Device.cpp
 * Author: tomko
 * 
 * Created on 7 kwiecień 2012, 16:33
 */

#include "../include/Device.h"

using namespace ALSA;

Device::Device()
{
  handle = 0;
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
    handle = 0;
  }
}

void Device::open(AccessMode am) throw (WrongArgument, InvalidOperation)
{
  if (!handle) // urządzenie jest już otwarte
  {
    throw InvalidOperation("Device is already opened.");
  }
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
  int err = snd_pcm_open(&handle, device, mode, 0); //otwarcie
  if (err < 0)
  {
    throw InvalidOperation(snd_strerror(err));
  }
  if ((err = snd_pcm_set_params(handle,
                                SND_PCM_FORMAT_U8,
                                SND_PCM_ACCESS_RW_INTERLEAVED,
                                1,
                                48000,
                                1,
                                500000)) < 0)
  {
    throw InvalidOperation(snd_strerror(err));
  }
}