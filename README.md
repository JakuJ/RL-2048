# Gra 2048 w kontekście uczenia ze wzmocnieniem

Repozytorium stworzone na potrzeby projektu w ramach przedmiotu "Warsztaty z technik uczenia maszynowego" prowadzonego w sem. letnim 2020 na Wydziale Matematyki i Nauk Informacyjnych PW.

Autorami projektu są:
* [Jakub Drak Sbahi](https://github.com/jakub-drak-sbahi)
* [Jakub Janaszkiewicz](https://github.com/JakuJ)
* [Maja Krzemińska](https://github.com/krzeminskamaja)
* [Mikołaj Molenda](https://github.com/bulkazszynka)
* [Anna Szmurło](https://github.com/SzmurloAnia)

Projekt skupia się na eksploracji możliwości modeli uczenia ze wzmocnieniem w kontekście gry 2048, w oparciu o sieci N-tuple i algorytm TD(0). To repozytorium nie zawiera raportu na temat przeprowadzonych eksperymentów.

# Budowanie

Wymagane narzędzia:

* CMake w wersji >= 3.9
* Kompilator C++ wspierający OpenMP (np. standardowe dystrybucje `clang` lub `gcc`)

Projekt testowany był z wykorzystaniem kompilatora `clang` w wersji 10.0.

W celu uproszczenia budowania i uruchamiania programu przygotowano skrypt `build.sh`. Jako argumenty przyjmuje odpowiednio liczbę gier (iteracji treningu) oraz początkową stałą uczenia. Skrypt buduje (lub przebudowuje w zależności od zmian) program w folderze `build_dir`, a następnie uruchamia go dla podanych parametrów.

Ręczna kompilacja może być wykonana przy użyciu narzędzia CMake. Zalecamy budowanie w osobnym katalogu:

```shell
mkdir build_dir
cd build_dir
cmake ..
make
```

# Użycie

Przygotowany program przyjmuje cztery argumenty:

```shell
>> ./RL2048
Usage: ./RL2048 epochs learning_rate log_filename save_model_prefix
```

* Liczbę iteracji treningu do wykonania (gier)
* Początkową stałą uczenia
* Ścieżkę do pliku, w którym zapisane zostaną dane nt. przebiegu procesu trenowania
* Przedrostek ścieżki pod którą serializowane są wagi modelu po treningu

To, czy wagi modelu po ukończeniu treningu są zapisywane kontroluje makro `#SAVE_MODEL` zdefiniowane w pliku `src/main.cpp`. Sama serializacja jest funkcją eksperymentalną, domyślnie modele nie są zapisywane i wartość ostatniego parametru programu może być dowolna.

Skrypt `build.sh` używa domyślnych wartości dla ostatnich dwóch parametrów.

# Wyniki

Przygotowany program przeprowadza proces trenowania modelu, którego architekturę można modyfikować poprzez edycję konstruktora klasy [Ensemble](src/Ensemble.cpp). W trakcie działania, wyniki poszczególnych gier reprezentowane są przez kolorowy interfejs konsolowy. Wyniki osiągnięte w kolejnych grach zapisywane są do pliku `log.csv`. Do prostej wizualizacji przebiegu uczenia służy skrypt `plot.py`, którego argumentem jest plik z danymi.

Przykładowe wykonanie:

```shell
python3 plot.py log.csv
```
