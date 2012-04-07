/* 
 * File:   Device.h
 * Author: tomko
 *
 * Created on 7 kwiecień 2012, 16:33
 */

#ifndef DEVICE_H
#define	DEVICE_H

namespace ALSA
{

  class Device
  {
  public:
    Device();
    Device(const Device& orig);
    virtual ~Device();
  private:

  };
}

#endif	/* DEVICE_H */

