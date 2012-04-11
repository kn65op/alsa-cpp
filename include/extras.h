/* 
 * File:   extras.h
 * Author: tomko
 *
 * Created on 9 kwiecień 2012, 15:51
 */

#ifndef EXTRAS_H
#define	EXTRAS_H

#include <string>

#include <alsa/asoundlib.h>

namespace TALSA
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

  /** enum służący do okrelania kierunku przepływu informacji.
   * Możliwe wartości: READ - tryb czytania; WRITE - tryb pisania.
   */
  enum class AccessMode
  {
    READ,
    WRITE,
    INVALID,
  };

  /** enum służący do określenia formatu danych (rozmiar, itp)
   * Możliwe wartości: U8 - 8-bitowo, bez znaku; U16 - 16-bitowo, bez znaku
   */
  enum class DataFormat
  {
    U8,
    U16,
    U24,
  };

  /**
   * Klasa z funkcjami pomocniczymi do obsługi DataFormat
   */
  class DataFormatHelper
  {
  public:
    /**
     * Brak możliwości tworzenia instancji.
     */
    DataFormatHelper() = delete;
    /**
     * Brak możliwości kopiowania.
     */
    DataFormatHelper(const DataFormatHelper &) = delete;

    /**
     * Zamiana formatu TALSA::DataFormat na snd_pcm_format_t
     * @param df TALSA::DataFormat dla którego chcemy uzyskać snd_pcm_format_t.
     * @return Wartość snd_pcm_format_t odpowiadający podanemu ALSA::DataFormat.
     */
    static snd_pcm_format_t getFormat(DataFormat df)
    {
      switch (df)
      {
        case DataFormat::U8:
          return SND_PCM_FORMAT_U8;
        case DataFormat::U16:
          return SND_PCM_FORMAT_U16;
        case DataFormat::U24:
          return SND_PCM_FORMAT_U24;
        default:
          throw WrongArgument("Wrong data format");
      }
    }

    /**
     * Funkcja zwracająca ilość bitów potrzebnych do zapisania danego typu danych zgodnie z dokumentacją ALSA.
     * @param df TALSA::DataFormat, którego rozmiar jest liczony.
     * @return Ilość bitów, które potrzebne są do zapisania danego formau.
     */
    static int getSizeOfFormat(DataFormat df)
    {
      switch (df)
      {
        case DataFormat::U8:
          return 1;
        case DataFormat::U16:
          return 2;
        case DataFormat::U24:
          return 4; //zgodnie z dokumentacją
        default:
          throw WrongArgument("Wrong data format");
      }
    }
  };

}


#endif	/* EXTRAS_H */

