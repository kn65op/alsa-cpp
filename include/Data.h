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

  /**
   * Klasa odwzorująca dane przesyłane z i do urządzeń.
   */
  class Data
  {
    //pozwolenie Device na dostęp do wskaźnika danych.
    friend class Device;

  public:
    Data();
    /**
     * Na razie kopiowanie nie jest możliwe.
     */
    Data(const Data& orig) = delete; //not implemented yet
    virtual ~Data();

    /**
     * Funkcja zwracająca liczbę próbek, które dana porcja danych może zawierać,
     * @return Libcza próbek, które może zawierać porcja danych.
     */
    int getSize() const;
    /**
     * Funkcja ustawiająca liczbę próbek, które dana porcja danych może zawierać.
     * @param size Liczba próbek, które powinna zawierać porcja danych
     */
    void setSize(int size);
    /**
     * Funkcja ustawiająca format danych na podany.
     * @param Format danych.
     */
    void setDataFormat(DataFormat);
    /**
     * Funkcja zwracająca format danych.
     * @return Format danych.
     */
    DataFormat getDataFormat() const;

  private:
    /** Format danych.
     */
    DataFormat data_format;
    /** Dane.
     */
    void *data;
    /** Ilość próbek możliwa do zapisania w danym sygnale.
     */
    int size;
//    /** Funkcja zwracająca liczbę próbek, które dana porcja danych może zwierać
//     */
//    int size();
  };
}



#endif	/* DATA_H */

