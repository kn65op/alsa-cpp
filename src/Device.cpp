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
