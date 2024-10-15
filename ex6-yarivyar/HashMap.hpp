
#ifndef EX6_YARIVYAR_HASH_MAP_HPP
#define EX6_YARIVYAR_HASH_MAP_HPP


#include <vector>
#include <functional>

#define INITIAL_SIZE 16
#define UPPER_LOAD_FACTOR (3.0 / 4.0)
#define LOWER_LOAD_FACTOR (1.0 /4.0)
#define INCREASE_SIZE 1
#define DECREASE_SIZE -1

template<class KeyT, class ValueT>
class HashMap
{
public:
    typedef std::vector<std::pair<KeyT, ValueT>> dynamic_vector;
protected:
    int _size;
    int _capacity;
    dynamic_vector *_hash_map;
    ValueT default_value_type = ValueT();
public:
    // default c'tor, starting size of _capacity is 16
    HashMap() : _size(0), _capacity(INITIAL_SIZE)
    {
        _hash_map = new dynamic_vector[INITIAL_SIZE];
        clear();
    };

    // receives two vectors (keys, values) inserts all to _hash_map
    // if two keys are the same, the later value is saved
    HashMap(const std::vector<KeyT> &key_vector,
            const std::vector<ValueT> &value_vector)
            : _size(0), _capacity(INITIAL_SIZE)
    {
        if (key_vector.size() != value_vector.size())
        {
            throw std::invalid_argument(
                    "Tried to Initiate HASH_MAP using two vectors "
                    "of different sizes");
        }
        _hash_map = new dynamic_vector[INITIAL_SIZE];
        clear();
        for (size_t i = 0; i < key_vector.size(); i++)
        {
            if (!insert(key_vector[i],
                        value_vector[i])) // key is already inside
            {
                at(key_vector[i]) = value_vector[i];
            }
        }
    }

    // copy c'tor
    HashMap(const HashMap &other) : _size(other.size()),
                                    _capacity(other.capacity())
    {
        _hash_map = new dynamic_vector[other.capacity()];
        clear();
        _size = other.size();
        _capacity = other.capacity();
        for (int cell = 0; cell < other.capacity(); cell++)
        {
            _hash_map[cell] = other._hash_map[cell];
        }
    }

    // virtual because of polymorphism of Dictionary
    virtual ~HashMap()
    {
        clear();
        delete[] _hash_map;
    }

    int size() const
    { return _size; }

    int capacity() const
    { return _capacity; }

    bool empty() const
    { return _size == 0; }

    /*
     * inserts a key to its bucket using hash func h(x) = x mod capacity
     * returns false if key is already in _hash_map.
     *
     */
    bool insert(const KeyT key, const ValueT value)
    {
        if (!contains_key(key))
        {
            std::hash<KeyT> key_hash;
            int cell = modulo_capacity(key_hash(key));
            _hash_map[cell].push_back(std::make_pair(key, value));
            _size++;
            if (get_load_factor() > UPPER_LOAD_FACTOR)
            {
                resize_hash_map(INCREASE_SIZE);
            }
            return true;
        }
        return false;
    }

    /*
    /// check if a key is already inside _hash_map
    /// assuming KeyT has operator==
    /// \param key
    /// \return true if key was found inside _hash_map, false otherwise
     */
    bool contains_key(const KeyT key) const
    {
        std::hash<KeyT> key_hash;
        int cell = modulo_capacity(key_hash(key));
        for (const auto &it: _hash_map[cell])
        {
            if (it.first == key)
            {
                return true;
            }
        }
        return false;
    }

    // receives key and returns its value by ref (changeable)
    // throws invalid argument if key wasn't found
    ValueT &at(const KeyT key)
    {
        for (auto &it: _hash_map[bucket_index(key)])
        {
            if (it.first == key)
            {
                return it.second;
            }
        }
        throw std::invalid_argument(
                "Used key doesn't exist.");
    }

    // receives key and returns its value by const_ref (unchangeable)
    // throws invalid argument if key wasn't found
    const ValueT &at(const KeyT &key) const
    {
        for (const auto &it: _hash_map[bucket_index(key)])
        {
            if (it.first == key)
            {
                return it.second;
            }
        }
        throw std::invalid_argument(
                "Used key doesn't exist.");
    }

    /*
    /// erases a key from _hash_map
    /// \param key
    /// \return true if key was deleted, otherwise false
     */
    virtual bool erase(const KeyT key)
    {
        std::hash<KeyT> key_hash;
        int cell = modulo_capacity(key_hash(key));
        for (auto it = _hash_map[cell].begin();
             it != _hash_map[cell].end(); it++)
        {
            if ((*it).first == key)
            {
                _hash_map[cell].erase(it);
                --_size;
                if (get_load_factor() < LOWER_LOAD_FACTOR)
                {
                    resize_hash_map(DECREASE_SIZE);
                }
                return true;
            }
        }
        return false;
    }

    double get_load_factor() const
    { return (double) _size / (double) _capacity; }

    /*
    // returns the size of the basket that key is inside
    // throws exception if key doesn't exist
    */
    int bucket_size(const KeyT key) const
    {
        if (!contains_key(key)) // key wasn't found
        {
            throw std::invalid_argument(
                    "Used key doesn't exist.");
        }
        std::hash<KeyT> key_hash;
        int cell = modulo_capacity(key_hash(key));
        return _hash_map[cell].size();
    }

    /*
    // returns the index of where key is inside _hash_map
    // throws exception if key doesn't exist
    */
    int bucket_index(const KeyT key) const
    {
        if (!contains_key(key)) // key wasn't found
        {
            throw std::invalid_argument(
                    "Used key doesn't exist.");
        }
        std::hash<KeyT> key_hash;
        int cell = modulo_capacity(key_hash(key));
        return cell;
    }

    /*
    // clears _hash_map from all it contents
    // doesn't change the capacity of _hash_map
    */
    void clear()
    {
        for (int cell = 0; cell < _capacity; cell++)
        {
            _hash_map[cell].clear();
        }
        _size = 0;
    }

    // assignment operator, assumes all members KeyT, ValueT have
    // copy constructors and copy assignment operators for deep copy
    HashMap<KeyT, ValueT> &operator=(const HashMap &other)
    {
        if (this == &other)
        {
            return *this;
        }
        clear();
        delete[]_hash_map;
        _capacity = other.capacity();
        _hash_map = new dynamic_vector[other.capacity()];
        clear();
        _size = other.size();
        for (int cell = 0; cell < other._capacity; cell++)
        {
            _hash_map[cell] = other._hash_map[cell];
        }
        return *this;
    }

    // receives key and returns its value by const_ref (unchangeable)
    // no exception
    const ValueT &operator[](const KeyT key) const
    {
        std::hash<KeyT> key_hash;
        int cell = modulo_capacity(key_hash(key));
        for (const auto &it: _hash_map[cell])
        {
            if (it.first == key)
            {
                return it.second;
            }
        }
        return default_value_type;
    }

    // receives key and returns its value by ref (changeable)
    // no exception
    ValueT &operator[](const KeyT key)
    {
        std::hash<KeyT> key_hash;
        int cell = modulo_capacity(key_hash(key));
        for (auto &it: _hash_map[cell])
        {
            if (it.first == key)
            {
                return it.second;
            }
        }
        insert(key, ValueT());
        return at(key);
    }

    // checks if keys are the same with the same value

    bool operator==(const HashMap<KeyT, ValueT> &other) const
    {
        if (size() != other.size())
        {
            return false;
        }
        dynamic_vector other_hash_map_data;
        other.insert_hash_map_data_to_vector(other_hash_map_data);
        for (const auto &it: other_hash_map_data)
        {
            if (!contains_key(it.first) || operator[](it.first) != it.second)
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const HashMap<KeyT, ValueT> &other) const
    {
        return !(operator==(other));
    }

    /*
    // returns the correct cell number inside _hash_map according to its capacity
     */
    int modulo_capacity(size_t cell) const
    {
        return cell & (_capacity - 1);
    }

    /*
     resizes hash_map, rehashing all keys and values to _hash_map
     param resize_value equals 1 if an increase of capacity is needed
     equals -1 if a decrease of capacity is needed
    */
    void resize_hash_map(int resize_value)
    {
        dynamic_vector hash_map_data;
        insert_hash_map_data_to_vector(hash_map_data);
        delete[]_hash_map;
        if (resize_value == INCREASE_SIZE)
        {
            while (get_load_factor() > UPPER_LOAD_FACTOR)
            {
                _capacity *= 2;
            }
        }
        if (resize_value == DECREASE_SIZE)
        {
            while (_capacity != 1 &&
                   get_load_factor() < LOWER_LOAD_FACTOR)
            {
                _capacity /= 2;
            }
        }
        _hash_map = new dynamic_vector[_capacity];
        clear();
        insert_to_hash_map_from_vector(hash_map_data);
    }

    /*
     * receives a vector and puts the data inside _hash_map.
     * function used by a function which resizes map.
     * guarantee all elements are different, _hash_map starts empty
     * no need to resize during the process
     */
    void insert_to_hash_map_from_vector(dynamic_vector &hash_map_data)
    {
        for (const auto &it: hash_map_data)
        {
            std::hash<KeyT> key_hash;
            int cell = modulo_capacity(key_hash(it.first));
            _hash_map[cell].push_back(std::make_pair(it.first, it.second));
            _size++;
        }
    }

    /*
    puts all data inside _hash_map into one long vector<KeyT, ValueT>
     */
    dynamic_vector &
    insert_hash_map_data_to_vector(dynamic_vector &hash_map_data) const
    {
        for (int cell = 0; cell < _capacity; cell++)
        {
            for (const auto &it: _hash_map[cell])
            {
                hash_map_data.push_back(std::make_pair(it.first, it.second));
            }
        }
        return hash_map_data;
    }


/*****************************************************************
                          * ITERATOR *
*****************************************************************/
    class ConstIterator
    {
        friend class HashMap<KeyT, ValueT>;

    private:
        const HashMap<KeyT, ValueT> &hash_map_;
        size_t basket_index_;
        size_t cell_index_;
    public:
        // Iterator traits
        typedef std::pair<KeyT, ValueT> value_type;
        typedef const std::pair<KeyT, ValueT> &reference;
        typedef const std::pair<KeyT, ValueT> *pointer;
        typedef int difference_type; // required but irrelevant as we have
        // no op-()
        typedef std::forward_iterator_tag iterator_category;
    private:
        // checks if the first bucket is empty and
        // skips it until reaching a bucket with objects
        ConstIterator(const HashMap<KeyT, ValueT> &hash_map,
                      size_t basket_index,
                      size_t cell_index)
                : hash_map_(hash_map),
                  basket_index_(basket_index),
                  cell_index_(cell_index)
        {
            if (basket_index == 0)
            {
                while (basket_index_ < (size_t) hash_map_.capacity() &&
                       hash_map_._hash_map[basket_index_].empty())
                {
                    basket_index_++;
                }
            }
        }

    public:
        // operator also checks if current basket is empty and skips it.
        ConstIterator &operator++()
        {
            // NOTE: a nested class has access to its parent's private members!
            if (++cell_index_ >= hash_map_._hash_map[basket_index_].size())
            {
                cell_index_ = 0;
                basket_index_++;
                while (basket_index_ < hash_map_.capacity() &&
                       hash_map_._hash_map[basket_index_].empty())
                {
                    basket_index_++;
                }
            }
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator it(*this); // default copy-ctr at work
            this->operator++();
            return it;
        }

        bool operator==(const ConstIterator &rhs) const
        {
            return (&hash_map_ == &rhs.hash_map_)
                   && (basket_index_ == rhs.basket_index_)
                   && (cell_index_ == rhs.cell_index_);
        }

        bool operator!=(const ConstIterator &rhs) const
        {
            return !operator==(rhs);
        }

        reference operator*() const
        {
            return hash_map_._hash_map[basket_index_][cell_index_];
        }

        pointer operator->() const
        {
            return &(operator*());
        }
    };

    /*****************************************************************
     * SequenceContainer member functions                            *
     *****************************************************************/
    using const_iterator = ConstIterator;

    const_iterator cbegin() const
    {
        return ConstIterator(*this, 0, 0);
    }

    const_iterator cend() const
    {
        return ConstIterator(*this, _capacity, 0);
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }
};


#endif //EX6_YARIVYAR_HASH_MAP_HPP
