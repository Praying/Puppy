/*
 * Created by Praying on 2018/2/8.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <vector>
#include <string>

namespace Flow
{
    class Tokenizer
    {
    public:
        typedef std::vector<const char *> StorageType;
        typedef StorageType::size_type size_type;
        typedef StorageType::const_iterator const_iterator;
        typedef StorageType::reference reference;
        typedef StorageType::const_reference const_reference;

    private:
        char *str_;
        StorageType storage_;
    public:
        Tokenizer(const std::string &src, const char sep, uint32_t vectorReserve = 0, bool keepEmptyStrings = true);

        ~Tokenizer()
        {
            delete[] str_;
        }

        const_iterator begin() const
        {
            return storage_.begin();
        }

        const_iterator end() const
        {
            return storage_.end();
        }

        size_type size() const
        {
            return storage_.size();
        }

        reference operator[](size_type i)
        {
            return storage_[i];
        }

        const_reference operator[](size_type i) const
        {
            return storage_[i];
        }
    };
}