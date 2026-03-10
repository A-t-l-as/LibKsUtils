# LibKsUtils
The library needed to compile my programs written in C++ for modifying the KnightShift game.

# What's interesting here?:
- `BinFile`                 -- Class handling binary files (reading from memory buffer and writing),
- `TextFile`                -- Class handling text files (reading from memory buffer and writing),
- `MyGuid`                  -- My implementation of a cross-platform GUID for different operating systems,
- `OrderedSet`              -- An implementation of a data structure called a set, but which preserves the order of elements,
- `TwoTypeArray`            -- An array whose elements can be of two types (e.g., int and float),
- `CompilatorBase`          -- Base class for a compiler that can be used to create a specific compiler for a given file format.
- `SimpleCompilator`        -- A basic classic compiler that converts standard .cpp file data to BinFile object memory.
- `Console`                 -- A singleton that allows you to conveniently write to the console and retrieve data from the user (I modeled it after the Console class from C#).
- `OrderFileBase`           -- Base class for a file containing a list of files to compile,
- `Platform.hpp`            -- I created a multi-platform macro that selects the appropriate built-in byteswap function depending on the operating system.
- `StringUitls.hpp`         -- Some auxiliary functions that operate on strings.
- `MySystem::SafeLocalTime` -- A safe cross-platform function that selects the appropriate localtime function for the appropriate operating system.
- Check the rest yourself...

---

Biblioteka, która jest potrzebna do kompilacji moich programów napisanych w języku C++ do modyfikacji gry KnightShift.

# Co tu jest ciekawego?:
- `BinFile`                 -- Klasa obsługująca pliki binarne (odczyt z bufora w pamieci i zapis),
- `TextFile`                -- Klasa obsługująca pliki tekstowe (odczyt z bufora w pamieci i zapis),
- `MyGuid`                  -- Moja implementacja wieloplatformowego GUID na różne systemy operacyjne,
- `OrderedSet`              -- Czyli implementacja struktury danych zwanej zbiorem ale zachowującej kolejność elementów,
- `TwoTypeArray`            -- Tablica, której elementy mogą być w dwóch typach (np. int i float),
- `CompilatorBase`          -- Klasa bazowa dla kompilatora, której można używać w celu stworzenia specyficznego kompilatora dla danego formatu pliku.
- `SimpleCompilator`        -- Podstawowy klasyczny kompilator, który konwertuje dane standardowego pliku .cpp do pamięci obiektu BinFile.
- `Console`                 -- Singleton, który umożliwia wygodne pisanie w konsoli oraz pobieranie danych od użytkownika (wzorowałem się na klasie Console z C#).
- `OrderFileBase`           -- Klasa bazowa dla pliku zawierającego listę plików do kompilacji,
- `Platform.hpp`            -- Zrobiłem wieloplatformowe makro, które w zależności od systemu operacyjnego wybiera odpowiednią wbudowaną funkcję byteswap,
- `StringUitls.hpp`         -- Trochę funkcji pomocniczych operujących na stringach,
- `MySystem::SafeLocalTime` -- Bezpieczna wieloplatformowa funkcja, która wybiera odpowiednią funkcję localtime dla odpowiedniego systemu operacyjnego.
- Resztę sprawdź samemu...