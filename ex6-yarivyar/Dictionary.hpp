

#ifndef EX6_YARIVYAR_DICTIONARY_HPP
#define EX6_YARIVYAR_DICTIONARY_HPP

#include <utility>

#include "HashMap.hpp"

#define KEY_ERR "Key Doesn't exist"

// class has 2 constructors for using INVALID ARGUMENT ERROR
// allows to use INVALID ARGUMENT with a message or without
class InvalidKey : std::invalid_argument
{

public:

    InvalidKey() : std::invalid_argument("Invalid Argument used in Dictionary")
    {};

    explicit InvalidKey(const std::string &err_msg) : std::invalid_argument(
            err_msg)
    {}
};

class Dictionary : public HashMap<std::string, std::string>
{
    // Typedefs
    typedef std::vector<std::pair<std::string, std::string>>
    ::iterator string_iterator;
    typedef std::vector<std::pair<std::string, std::string>>
    ::const_iterator const_string_iterator;
public:

    /*****************************************************************
     *       Constructors for supporting HashMap Constructors.       *
     *****************************************************************/
    Dictionary() : HashMap<std::string, std::string>()
    {};

    explicit Dictionary(HashMap<std::string, std::string> &other)
            : HashMap<std::string, std::string>(other)
    {};

    Dictionary(const std::vector<std::string> &key_vector,
               const std::vector<std::string> &value_vector)
            : HashMap<std::string, std::string>(key_vector, value_vector)
    {};

    // uses HashMap erase, throws INVALID KEY if key wasn't successfully deleted
    bool erase(const std::string key) override
    {
        if (!HashMap<std::string, std::string>::erase(key))
        {
            throw InvalidKey(KEY_ERR);
        }
        return true;
    }

    // receives two iterators (begin, end) and inserts all the contents to dictionary
    void
    update(string_iterator begin,
           const string_iterator end)
    {
        for (; begin != end; begin++)
        {
            if (!insert((*begin).first, (*begin).second))
            {
                at((*begin).first) = (*begin).second;
            }

        }
    }

    // const version for const iterators
    void
    update(const_string_iterator begin,
           const_string_iterator end)
    {
        for (; begin != end; begin++)
        {
            insert((*begin).first, (*begin).second);
        }
    }

};

#endif //EX6_YARIVYAR_DICTIONARY_HPP
