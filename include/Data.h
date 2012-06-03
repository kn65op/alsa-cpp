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
#include <fftw3.h>

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
     * Funkcja określająca czy dany zakres zawiera w sobie mowę. Określanie na podstawie energii.
     * @param start Początek zakresu.
     * @param end Koniec zakresu.
     * @return true jeśli w danym zakresie wykryto mowę, false w przeciwnym wypadku.
     */
    bool isSpeechInside(int start, int end);
    /**
     * Funkcja licząca energię danej ramki jako logarytm 
     * @param n Numer ramki.
     * @return Energia ramki.
     */
    double getFrameEnergy(int n);
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
    /**
     * Funkcja licząca momemt widmowy rzędu 0 .
     * @return Moment widmowy rzędu 0.
     */
    double spectralMoment0();
    /**
     * Funkcja licząca moment unormowany rzędu pierwszego.
     * @return Moment unormowany rzędu 1.
     */
    double normalizedMoment1();
    /**
     * Funkcja licząca moment unormowany centralny rzędu drugiego.
     * @return Moment unormowany centralny rzędu 2.
     */
    double normalizedCentralMoment2();
    /**
     * Funkcja licząca unormowany moment centralny rzędu trzeciego.
     * @return Moment unormowany centralny rzędu 3.
     */
    double normalizedCentralMoment3();
    /**
     * Funkcja licząca 3 formanty.
     * @param frame Numer okna.
     * @return Wektor z trzema formantami.
     */
    std::vector<double> get3Formants(int frame);
    /**
     * Funkcja przygotowująca ramkę do analizy.
     * @param n Numer ramki.
     */
    void prepareWindow(int n);
    /**
     * Funkcja wyszukująca granice między fonemami. Przed wyszukiwaniem fonemów należy znaleźć granice mowy za pomocą funkcji findSpeechBorders();
     */
    void findPhonemeBorders();
    /**
     * Funkcja szukająca granic mowy.
     * @return true, jeśli znaleziono mowę, false w przeciwnym wypadku.
     */
    bool findSpeechBorders();
    /**
     * Funkcja analizująca wykryte segmenty. Wcześniej należy wyznaczyć granice pomiędzy fonemami za pomocą funkcji findPhonemeBorders();
     */
    void analyzeSegments();
    /**
     * Funkcja zwracająca parametry wyznaczone dla kolejnych fonemów. Należy wcześniej je obliczyć za pomocą funkcji analyzeSegments();
     * @return Macierz parametrów.
     */
    std::vector<std::vector<double>> getParameters() const
    {
      return parameter;
    }

    /**
     * Funkcja zwracająca progi do wyznaczania granic fonemów.
     * @return std::vector<double> z progami.
     */
    static std::vector<double> getALCRThresholds();
    

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
    /** wskaźnik na dane używane przy ekstracji cech  przed fft
     */
    double *data_before_fft;
    /** wskaźnik na dane używane przy ekstracji cech  po fft
     */
    double *data_after_fft;
    /** Czy jest plan fft
     */
    bool fft;
    /** plan
     */
    fftw_plan plan;
    /** */
    /** liczba ważnych elementów w fft
     */
    int fft_good;
    /** numer ostatnio przygotowanego okna
     */
    int prepared;
    /** wartość momentu widmowego rzędu 0
     */
    double spectalMoment0Val;
    /** wartość momentu znormalizowanego rzędu 1
     */
    double normalizedMoment1Val;
    /** Minimalna wielkość energii oznaczająca, że jest mowa
     */
    double min_energy;
    /**
     * Funkcja kopiująca okno do innej tablicy.
     * @param a Początek okna.
     * @param b Koniec okna.
     */
    void copyWindow(int a, int b);
    /**
     * Skalowanie fragmentu dźwięku.
     * @param a początek fragmentu.
     * @param b koniec fragmentu.
     */
    void scale(int length);
    /**
     * Preemfaza fragmentu dźwięku, znajdującego się w zmiennej data_feature.
     * @param length Długość fragmentu.
     */
    void preemphasis(int length);
    /**
     * Okienkowanie fragmentu dźwięku, znajdującego się w zmiennej data_feature.
     * @param length Długość fragmentu.
     */
    void HammingWindow(int length);
    /**
     * Liczenie transformaty Fouriera fragmentu dźwięku, znajdującego się w zmiennej data_feature.
     * @param length Długość fragmentu.
     */
    void calcFFT(int length);
    /**
     * Funkcja tworząca plan fftw.
     */
    void initFFT(int length);
    /**
     * Funkcja zwracająca częstotliwość na osi w widmie.
     * @param i Numer prążka.
     * @return Częstotliwość dla danego prążka.
     */
    double getFrequencyFromSpectrum(int i) const;
    /**
     * Funkcja zwracająca częstotliwość na osi w widmie.
     * @param i Numer prążka.
     * @param fft_len Długość fft.
     * @return Częstotliwość dla danego prążka.
     */
    double getFrequencyFromSpectrum(int i, int fft_len) const;
    /** Progi do wyznaczania ALCR
     */
    static std::vector<double> ALCRthresholds;
    /** czy progi zostały stworzone
     */
    static bool is_ALCRThresholds;
    /** Początek i koniec fragmentu mowy. */
    int speech_begin, speech_end;
    /** wektor granic pomiędzy segmentami mowy
     */
    std::vector<int>segments;
    /** parametry wyznaczone z segmentow
     */
    std::vector<std::vector<double>> parameter;
  };
}



#endif	/* DATA_H */

