/* 
 * File:   Device.h
 * Author: tomko
 *
 * Created on 7 kwiecień 2012, 16:33
 */

#ifndef DEVICE_H
#define	DEVICE_H

#include "alsa/asoundlib.h"
#include <string>

#include "Data.h"
#include "extras.h"

namespace TALSA
{

  /**
   * Klasa odwzorująca urządzenie (lub plik) umożliwiająca przesyłanie danych dźwiękowych.
   */
  class Device
  {
  public:
    Device();
    /**
     * Kopiowanie nie jest na razie możliwe.
     */
    Device(const Device& orig) = delete; //not implemented yet
    virtual ~Device();
    /**
     * Funkcja ustawiająca urządzenie.<br/>
     * Format wymagany przez ALSA (http://www.alsa-project.org/alsa-doc/alsa-lib/pcm.html)
     * <ul>
     * <li>default - urządzenie domyślne</li>
     * <li>plughw:0,0, hw:0,0, pluh:hw - urządzenia fizyczne (?)</li>
     * <li>file:'/tmp/out.raw',raw - plik</li>
     * <li>file:'/tmp/out.wav',wav - plik</li>
     * </ul>
     * @param dev std::string odpowiadający urządzeniu w formacie wymaganym przez ALSA.
     */
    void setDevice(std::string dev);
    /**
     * Funkcja otwierająca urządzenie.
     * @param am TALSA::AccessMode określający kierunek przypływu informacji.
     */
    void open(AccessMode am) throw (WrongArgument, InvalidOperation);
    /**
     * Funkcja zamykająca urządzenie.
     */
    void close();
    /**
     * Funkcja ustawiająca format danych. Formatu nie można zmienić dla otwartego urządzenia.
     * @param df TALSA::DataFormat określający format przysyłanych danych.
     */
    void setDataFormat(DataFormat df);
    /**
     * Funkcja zapisująca dane z podanego obiektu do urządzenia. Parametry zapisu (np. długość), zależą od podanych danych.
     * @param d Obiekt, w którym znajdują się dane do zapisu.
     */
    void write(Data & d) throw (WrongArgument, InvalidOperation);
    /**
     * Funkcja czytająca z urządzenia dane do podanego obiektu. Parametry odczytu (np. długość), zależą od podanych danych. Aktualne dane zostaną nadpisane.
     * @param d Obiekt, który będzie przechowywał dane.
     */
    void read(Data & d) throw (WrongArgument, InvalidOperation);
  private:
    snd_pcm_t *handle; //uchwyt
    std::string device; //urządzenie
    AccessMode access_mode; //służy do kontroli kierunku przepływu informacji
    DataFormat data_format;


    
    /**
     * Funkcja pomocnicza, do sprawdzania warunków poprawności danych.
     * @param d Dane do sprawdzenia.
     */
    void checkData(Data & d) throw (WrongArgument);

    /**
     * Funkcja czyszcząca.
     */
    void clear();
  };
}

#endif	/* DEVICE_H */
