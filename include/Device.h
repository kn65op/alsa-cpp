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

namespace ALSA
{

  /**
   * Klasa wyjątku w przypadku złego argumentu przekazanego w funkcji.
   */
  class WrongArgument
  {
  public:
    //Brak możliwości tworzenia wyjątku bez podania wiadomości.
    WrongArgument() = delete;

    /**
     * Konstruktor z ustawieniem wiadomości.
     * @param mes std::string z wiadomością.
     */
    WrongArgument(std::string mes) : message(mes)
    {
    };
  private:
    std::string message;
  };

  /**
   * Klasa wyjątku w przypadku wywołania nieprawidłowej operacji lub błedu w trakcie operacji.
   */
  class InvalidOperation
  {
  public:
    //Brak możliwości tworzenia wyjątku bez podania wiadomości.
    InvalidOperation() = delete;

    /**
     * Konstruktor z ustawieniem wiadomości.
     * @param mes std::string z wiadomością.
     */
    InvalidOperation(std::string mes) : message(mes)
    {
    };
  private:
    std::string message;
  };

  /**
   * Klasa służąca do określenia kierunku przepływu informacji.
   */
  enum class AccessMode
  {
    READ,
    WRITE,
    INVALID,
  };

  /**
   * Klasa służąca do określenia formatu danych (rozmiar, itp)
   */
  enum class DataFormat
  {
    U8,
    U16,
  };

  /**
   * Klasa odwzorująca urządzenie (lub plik) umożliwiająca przesyłanie danych dźwiękowych.
   */
  class Device
  {
  public:
    Device();
    Device(const Device& orig);
    virtual ~Device();
    /**
     * Funkcja ustawiająca urządzenie.
     * <ul>Format wymagany przez ALSA (http://www.alsa-project.org/alsa-doc/alsa-lib/pcm.html)
     * <li>default - urządzenie domyślne</li>
     * <li>plughw:0,0, hw:0,0, pluh:hw - urządzenia fizyczne (?)</li>
     * <li>file:'/tmp/out.raw',raw - plik</li>
     * <li>file:'/tmp/out.wav',wav - plik</li>
     * </ul>
     * @param dev std::string odpowiadający urządzeniu w formacie wymaganym przez ALSA.
     */
    void setDevice(const std::string dev);
    /**
     * Funkcja ustawiająca urządzenie.
     * <ul>Format wymagany przez ALSA (http://www.alsa-project.org/alsa-doc/alsa-lib/pcm.html)
     * <li>default - urządzenie domyślne</li>
     * <li>plughw:0,0, hw:0,0, pluh:hw - urządzenia fizyczne (?)</li>
     * <li>file:'/tmp/out.raw',raw - plik</li>
     * <li>file:'/tmp/out.wav',wav - plik</li>
     * </ul>
     * @param dev Napis w stylu C (char *) odpowiadający urządzeniu w formacie wymaganym przez ALSA.
     */
    void setDevice(const char * dev);
    /**
     * Funkcja otwierająca urządzenie.
     * @param am ALSA::AccessMode określający kierunek przypływu informacji.
     */
    void open(AccessMode am) throw (WrongArgument, InvalidOperation);
    /**
     * Funkcja zamykająca urządzenie.
     */
    void close();
    /**
     * Funkcja ustawiająca format danych.
     * @param df ALSA::DataFormat określający format przysyłanych danych.
     */
    void setDateFormat(DataFormat df);
  private:
    snd_pcm_t *handle; //uchwyt
    char *device; //urządzenie
    AccessMode data_format; //służy do kontroli kierunku przepływu informacji
  };
}

#endif	/* DEVICE_H */

