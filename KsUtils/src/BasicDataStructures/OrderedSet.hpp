#ifndef ORDERED_SET_HPP
#define ORDERED_SET_HPP

#include <vector>
#include <map>
#include <stdexcept>

template<typename T>
class OrderedSet
{
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;


    std::pair<iterator, bool> insert(const T& value)
    {
        auto it = index_.find(value);
        if (it != index_.end()) {
            return {data_.begin() + it->second, false};
        }
        index_[value] = data_.size();
        data_.push_back(value);
        return {data_.end() - 1, true};
    }


    bool erase(const T& value)
    {
        auto it = index_.find(value);
        if (it == index_.end()) return false;

        size_t pos = it->second;
        index_.erase(it);
        data_.erase(data_.begin() + pos);

        // Aktualizacja indeksów po usuniętym elemencie
        for (size_t i = pos; i < data_.size(); ++i)
            index_[data_[i]] = i;

        return true;
    }

    bool contains(const T& value) const
    {
        return index_.count(value) > 0;
    }

    size_t count(const T& value) const
    {
        return index_.count(value);
    }

    size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
    void clear() { data_.clear(); index_.clear(); }


    const T& at(size_t pos) const
    {
        if (pos >= data_.size()) throw std::out_of_range("OrderedSet::at");
        return data_[pos];
    }


    iterator begin() { return data_.begin(); }
    iterator end()   { return data_.end(); }
    const_iterator begin()  const { return data_.begin(); }
    const_iterator end()    const { return data_.end(); }
    const_iterator cbegin() const { return data_.cbegin(); }
    const_iterator cend()   const { return data_.cend(); }

private:
    std::vector<T> data_;
    std::map<T, size_t> index_;
};

#endif // !ORDERED_SET_HPP
