# CatOS

CatOS to minimalistyczny system operacyjny, rozwijany głównie w językach Makefile, C oraz Assembly. Projekt ten ma na celu edukację oraz eksperymentowanie z podstawami budowy systemów operacyjnych od zera.

## Spis treści

- [Opis](#opis)
- [Funkcjonalności](#funkcjonalności)
- [Wymagania](#wymagania)
- [Budowanie](#budowanie)
- [Uruchamianie](#uruchamianie)
- [Struktura katalogów](#struktura-katalogów)
- [Licencja](#licencja)
- [Autor](#autor)

## Opis

CatOS to projekt systemu operacyjnego pisanego głównie dla celów edukacyjnych. W repozytorium znajdziesz kod źródłowy bootloadera, kernela, pliki konfiguracyjne oraz skrypty budujące.

## Funkcjonalności

- Prosty bootloader
- Podstawowy kernel w C i assemblerze
- Zarządzanie pamięcią (w fazie rozwoju)
- Obsługa wejścia/wyjścia z klawiatury i ekranu
- Możliwość uruchamiania w emulatorze QEMU

## Wymagania

- Kompilator `gcc` oraz `nasm`
- `make`
- Opcjonalnie: emulator QEMU

## Budowanie

Aby zbudować projekt, użyj polecenia:

```sh
make
```

Wszystkie artefakty budowania pojawią się w katalogu `build/`.

## Uruchamianie

Aby uruchomić system w emulatorze QEMU:

```sh
make run
```

Lub uruchom wygenerowany obraz ISO w wybranym emulatorze.

## Struktura katalogów

- `src/` – kod źródłowy kernela i bootloadera
- `build/` – pliki wynikowe po kompilacji
- `Makefile` – plik budujący projekt

## Licencja

Projekt dostępny na licencji MIT.

## Autor

Projekt rozwijany przez [Kitler174](https://github.com/Kitler174).
