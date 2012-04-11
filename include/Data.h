/* 
 * File:   Data.h
 * Author: tomko
 *
 * Created on 9 kwiecień 2012, 15:50
 */

#ifndef DATA_H
#define	DATA_H

#include "extras.h"

namespace TALSA
{
  //pozwolenie Device na dostęp do wskaźnika danych.
  friend class TALSA::Device; 
  /**
   * Klasa odwzorująca dane przesyłane z i do urządzeń.
   */
  class Data
  {
  public:
    Data();
    /**
     * Na razie kopiowanie nie jest możliwe.
     */
    Data(const Data& orig) = delete; //not implemented yet
    virtual ~Data();
  private:
    /** Format danych.
     */
    DataFormat data_format;
    /** Dane.
     */
    void *data;
  };
}

#endif	/* DATA_H */

