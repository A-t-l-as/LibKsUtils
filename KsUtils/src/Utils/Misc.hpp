#ifndef MISC_H
#define MISC_H

#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include <sstream>

namespace Misc
{
    static size_t CountOccurrences(const char* buffer, size_t size, const std::string& word)
	{
		std::string data(buffer, size);

		size_t count = 0;
		size_t pos = data.find(word, 0);

		while (pos != std::string::npos)
		{
			++count;
            pos = data.find(word, pos + word.length()); // Przesuwamy indeks za znalezione slowo
		}

		return count;
	}

    template<class T>
    inline bool CheckThatElementExistInVec(const T& answer, const std::vector<T>& elements)
    {
        for (const T& element : elements)
        {
            if(answer == element)
                return true;
        }

        return false;
    }


    inline void MoveByLinesInSS(std::stringstream& arg_stream, std::size_t count)
    {
        std::string null_str;

        for (std::size_t i = 0; i < count; ++i)
            std::getline(arg_stream, null_str);
    }

    static std::queue<std::string> FindLineStrings(const char* buffer, size_t bufferSize, const std::string& pattern)
    {
        std::queue<std::string> results;
        const size_t patternLength = pattern.length();

        const char* currentPos = buffer;
        const char* endPos = buffer + bufferSize;

        while (currentPos < endPos)
        {
            // Znajdz wystapienie "texture "
            const char* found = std::search(currentPos, endPos, pattern.begin(), pattern.end());

            if (found == endPos)
            {
                break; // Nie znaleziono wiecej wystapień
            }

            // Przesun wskaznik na poczatek tekstu po "texture "
            const char* textStart = found + patternLength;

            // Znajdz koniec linii (znak nowej linii lub koniec bufora)
            const char* lineEnd = std::find(textStart, endPos, '\n');

            // Zapisz wynik (od tekstu po "texture " do konca linii)
            results.emplace(textStart, lineEnd);

            // Przesun wskaznik na koniec biezacej linii do przeszukania
            currentPos = lineEnd;
        }

        return results;
    }


    inline void InitializeHashMapViaLabelsVector(std::unordered_map<std::string, int64_t>& labels_map,
                                                 const std::vector<std::string>& labels)
    {
        int64_t i = 0;
        for (const std::string& str : labels)
        {
            labels_map[str] = i;
            ++i;
        }
    }


    inline void Margin(std::stringstream& output, int64_t count)
    {
        for (int64_t i = 0; i < count; ++i)
            output << '\t';
    }


    template<typename T>
    inline void InsertToVector(std::vector<T>& vec, const T& value, std::size_t n)
    {
        for (std::size_t i = 0; i < n; ++i)
            vec.push_back(value);
    }

};


#endif // !MISC_H
