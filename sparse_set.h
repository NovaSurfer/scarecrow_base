//
// Created by novasurfer on 02/10/2025
//

#ifndef SC_SPARSE_SET_H
#define SC_SPARSE_SET_H

#include "typeutils.h"
#include "vec.h"

struct allocator;

namespace sc
{
    template <typename T>
    class sparse_set
    {
        SC_requires_unsigned(T);

    public:
        constexpr sparse_set() = delete;
        constexpr explicit sparse_set(allocator& alloc);

        constexpr void add(T item);
        constexpr bool remove(T item);
        constexpr const vec<T>& get_dense() const;
        constexpr size_t get_item_index(T item) const;

        constexpr void resize(size_t new_size);
        // constexpr void shrink_to_fit();
        constexpr void clear();

        constexpr bool has(T item) const;
        constexpr bool empty() const;
        constexpr size_t len() const;
        constexpr size_t capacity() const;

    private:
        vec<T> dense;
        vec<T> sparse;
    };

    template <typename T>
    constexpr sparse_set<T>::sparse_set(allocator& alloc)
        : dense {alloc}
        , sparse {alloc}
    { }

    template <typename T>
    constexpr void sparse_set<T>::add(T item)
    {
        if(has(item)) {
            return;
        }

        dense.push(item);
        if(item >= sparse.len()) {
            // resize sparse array to the new int that we just stored
            // to know later at what index it was stored inside dense array
            sparse.resize(item + 1);
        }
        sparse[item] = dense.len() - 1;
    }

    template <typename T>
    constexpr bool sparse_set<T>::remove(T item)
    {
        if(item >= sparse.capacity() && !has(item)) {
            return false;
        }

        const T dense_index = sparse[item];
        const size_t dense_length = dense.len() - 1;
        const T last_item = dense[dense_length];

        // last element = element to remove
        dense[dense_length] = dense[dense_index];
        // set that slot to what was the last item
        dense[dense_index] = last_item;
        // remove last element
        dense.pop_back();

        sparse[last_item] = dense_index;
        const size_t sparse_length = sparse.len() - 1;
        const T last_sparse_item = sparse[sparse_length];
        sparse[sparse_length] = sparse[item];
        sparse[item] = last_sparse_item;
        sparse.pop_back();

        return true;
    }

    template <typename T>
    constexpr const vec<T>& sparse_set<T>::get_dense() const
    {
        return dense;
    }

    template <typename T>
    constexpr void sparse_set<T>::resize(size_t new_size)
    {
        dense.resize(new_size);
        sparse.resize(new_size);
    }

    // TODO: fix + uncomment test case
    // template <typename T>
    // constexpr void sparse_set<T>::shrink_to_fit()
    // {
    //     dense.shrink_to_fit();
    //     sparse.shrink_to_fit();
    // }

    template <typename T>
    constexpr void sparse_set<T>::clear()
    {
        dense.clear();
        sparse.clear();
    }

    template <typename T>
    constexpr bool sparse_set<T>::has(T item) const
    {
        // Using .len() instead of .capacity() because sparse_set can be used after .clean().
        // I want to avoid accessing uninitialized memory,
        // as Valgrind reports: "Conditional jump or move depends on uninitialized value."
        if(item >= sparse.len()) {
            return false;
        }

        T dense_index = sparse[item];

        return dense_index < dense.len() && dense[dense_index] == item;
    }

    template <typename T>
    constexpr bool sparse_set<T>::empty() const
    {
        return dense.empty();
    }

    template <typename T>
    constexpr size_t sparse_set<T>::len() const
    {
        return dense.len();
    }

    template <typename T>
    constexpr size_t sparse_set<T>::capacity() const
    {
        return dense.capacity();
    }

    // TODO: write test
    template <typename T>
    constexpr size_t sparse_set<T>::get_item_index(T item) const
    {
        if(item >= sparse.capacity()) {
            return size_t(-1);
        }

        return sparse[item];
    }

} // namespace sc

#endif // SC_SPARSE_SET_H
