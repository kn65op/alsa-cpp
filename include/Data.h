/* 
 * File:   Data.h
 * Author: tomko
 *
 * Created on 9 kwiecień 2012, 15:50
 */

#ifndef DATA_H
#define	DATA_H

#include "extras.h"

#include <string>
#include <vector>

namespace TALSA
{

  /**
   * Klasa odwzorująca dane przesyłane z i do urządzeń. Konieczne jest ustalenie formatu przez działaniem.
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
     * @param data_format Format danych.
     */
    void setDataFormat(DataFormat data_format);
    /**
     * Funkcja zwracająca format danych.
     * @return Format danych.
     */
    DataFormat getDataFormat() const;
    /**
     * Operator bezpośredniego dostępu do pamięci.
     * @return Wskaźnik typu void, wskazujący na początek pamięci.
     */
    void * operator*();
    /**
     * Funkcja usuwająca wartość średnią z sygnału.
     */
    void removeConstantComponent();
    /**
     * usunięcie wartości średniej - ok
     * skalowanie / normalizacja
     * tłumienie odbić - nie ma
     * tłumienie zakłóceń i szumu - nie ma	
     * preemfaza (wzmacnianie wyższych częstotliwości
     * podział na słowa
     */

    /**
     * Funkcja zapisująca czyste wartości do pliku.
     * @param filename Nazwa pliku, do którego chcemy zapisać.
     */
    void saveRawDataToFile(std::string filename);
    /**
     * Funkcja ustawiająca podział na ramki.
     * @param length Długość ramki w sekundach.
     * @param overlap Procentowe określenie zachodzenia ramek na siebie.
     */
    //    void setFrameLength(double length, double overlap);
    /**
     * Funkcja ustawiająca podział na ramki.
     * @param length Długość ramki jako liczba próbek.
     * @param overlap Nachodzenie ramek jako liczba próbek.
     */
    void setFrameLength(int length, int overlap);
    /**
     * Funkcja określająca czy dana ramka zawiera w sobie mowę. Określanie jest na podstawie energii.
     * @param n Numer ramki.
     * @return true, jeśli w danej ramce stwierdzono mowę, false w przeciwnym wypadku.
     */
    bool isFrameWithSpeech(int n);
    /**
     * Funkcja zwracająca 12 współczynników MFCC otrzymane z danej ramki.
     * @param n Numer ramki.
     * @return std::vector  12 liczb double, będących współczynnikami.
     */
    std::vector<double> getMFCCFromFrame(int n);
    /**
     * Ustawienie częstotliwości próbkowania.
     * @param sf Częstotliwość próbkowania w Hz.
     */
    void setSampleFrequency(int sf);
    /**
     * Odczytanie częstotliwości próbkowania.
     * @return Częstotliwość próbkowania w Hz.
     */
    int getSampleFrequency() const;
    /**
     * Funkcja zwracająca liczbę możliwych ramek.
     * @return Liczba możliwych ramek.
     */
    int getWindowsNumber() const;

    void test();
  private:
    /** Format danych.
     */
    DataFormat data_format;
    /** Dane.
     */
    std::uint8_t *data;
    /** Ilość próbek możliwa do zapisania w danym sygnale.
     */
    int size;
    /** Ilość zajętej pamięci
     */
    int mem_size;
    //    /** Funkcja zwracająca liczbę próbek, które dana porcja danych może zwierać
    //     */
    //    int size();
    /** Funkcja alokująca pamięć
     */
    void createData();
    /** Długość ramki w s.
     */
    double window_length_in_s;
    /** zachodzenie ramek
     */
    double window_overlap;
    /** punkt startu kolejnej ramki
     */
    int window_start;
    /** długość ramki w próbkach
     */
    int window_length;
    /** częstotliwość próbkowania
     */
    int sample_frequency;
    /** wskaźnik na dane używane przy ekstracji cech 
     */
    double *data_feature;
    /**
     * Skalowanie fragmentu dźwięku.
     * @param a początek fragmentu.
     * @param b koniec fragmentu.
     */
    void scale(int a, int b);
    /**
     * Preemfaza fragmentu dźwięku.
     * @param a początek fragmentu.
     * @param b koniec fragmentu.
     */
    void preemphasis(int a, int b);
    /**
     * Okienkowanie fragmentu dźwięku.
     * @param a początek fragmentu.
     * @param b koniec fragmentu.
     */
    void HammingWindow(int a, int b);
    /**
     * Liczenie transformaty Fouriera fragmentu.
     * @param a początek fragmentu.
     * @param b koniec fragmentu.
     */
    void fft(int a, int b);
  };
}



#endif	/* DATA_H */

