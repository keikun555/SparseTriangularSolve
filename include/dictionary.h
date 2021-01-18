#pragma once

#include <vector>
#include <utility>

using std::pair;
using std::vector;

/**
* Dictionary defines a templated pure-virtual interface for a dictionary.
* K is the key type, V the value type
*/
template <typename K, typename V>
class Dictionary {
public:
    virtual ~Dictionary() {}

    /**
     * Returns the size of the dictionary.
     * @return The number of key-value pairs in the data structure.
     */
    virtual int getSize() = 0;

    /**
     * Returns true if the tree is empty.
     * @return true if there are no elements in the dictionary.
     */
    virtual bool isEmpty() = 0;

    /**
     * Inserts the key-value pair into the tree
     * @param key The key for the new mapping.
     * @param value The value to associate with that key.
     * @throws runtime_error If they key already exists.
     */
    virtual void insert(K key, V value) = 0;

    /**
     * Finds the element indexed by the given key and updates its value to the
     * provided value parameter.
     * @param key The key of the mapping to update.
     * @param value The new value to associate with that key.
     * @throws runtime_error if the key is not found in the dictionary.
     */
    virtual void update(K key, V value) = 0;

    /**
     * Returns the value associated with the given key
     * @param key The key of the mapping to find.
     * @return The value associated with that key.
     * @throws runtime_error If the key is not found in the dictionary.
     */
    virtual V get(K key) = 0;

    /**
     * Determines if a given key exists in a mapping in this dictionary.
     * @param key The key to look for.
     * @return true if a mapping in this BST has that key.
     */
    virtual bool contains(K key) = 0;

    /**
     * Deletes the element with given key from the tree.
     * @param key The key of the mapping to remove.
     * @throws runtime_error If they key was not already in this dictionary.
     */
    virtual void remove(K key) = 0;

    /**
     * Obtains a vector containing all keys in this dictionary.
     * @return An STL vector object containing the keys of every mapping in this
     *         BST (in no particular order).
     */
    virtual std::vector<K> getKeys() = 0;

    /**
     * Obtains a vector containing all key-value pairs in this dictionary.
     * @return An STL vector object containing the key-value pairs in every
     *         mapping in this dictionary (in no particular order).
     */
    virtual std::vector<pair<K,V>> getItems() = 0;

    // You can safely ignore the following code.  This eliminates some default
    // class routines, preventing you from using them accidentally.
    // Specifically, we are disabling the use of the copy constructor and the
    // copy assignment operator.  You can read more here:
    //   http://www.cplusplus.com/articles/y8hv0pDG/
public:
    Dictionary() { }
private:
    Dictionary(const Dictionary& other) = delete;
    Dictionary& operator=(const Dictionary& other) = delete;
};

