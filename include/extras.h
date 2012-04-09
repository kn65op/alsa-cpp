/* 
 * File:   extras.h
 * Author: tomko
 *
 * Created on 9 kwiecień 2012, 15:51
 */

#ifndef EXTRAS_H
#define	EXTRAS_H

#include <string>

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
  };

}


#endif	/* EXTRAS_H */

