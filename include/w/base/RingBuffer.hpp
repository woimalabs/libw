/**
 * libw
 *
 * Copyright (C) 2012-2015 Woima Solutions
 *
 * This software is provided 'as-is', without any express or implied warranty. In
 * no event will the authors be held liable for any damages arising from the use
 * of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including
 * commercial applications, and to alter it and redistribute it freely, subject to
 * the following restrictions:
 *
 * 1) The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a product,
 *    an acknowledgment in the product documentation is appreciated.
 *
 * 2) Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3) This notice may not be removed or altered from any source distribution.
 *
 * @author antti.peuhkurinen@woimasolutions.com
 */

#ifndef LIBW_RINGBUFFER
#define LIBW_RINGBUFFER

#include "w/base/Class.hpp"
#include "w/base/Exception.hpp"

namespace w
{
    template <class T> class RingBuffer
    {
    public:
        UNCOPYABLE(RingBuffer)

        /**
         * Creates one producer - one consumer ring buffer.
         * This class can be used from two different threads safely.
         *
         * @param [in]  size    Buffer size in number of values.
         */
        RingBuffer(size_t size):
            size_(size),
            buffer_(new T[size]),
            readIndex_(0),
            writeIndex_(0)
        {
            if (size == 0)
            {
                throw Exception("RingBuffer size cannot be 0.");
            }
        }

        ~RingBuffer()
        {
            delete [] buffer_;
        };

        size_t next(size_t n) const
        {
            return (n + 1) % size_;
        }

        bool empty() const
        {
            return (readIndex_ == writeIndex_);
        }

        bool full() const
        {
            return (next(writeIndex_) == readIndex_);
        }

        bool put(const T& value)
        {
            if (full())
            {
                return false;
            }
            buffer_[writeIndex_] = value;
            writeIndex_ = next(writeIndex_);
            return true;
        }

        bool get(T& value)
        {
            if (empty())
            {
                return false;
            }
            value = buffer_[readIndex_];
            readIndex_ = next(readIndex_);
            return true;
        }

    private:
        T* buffer_;
        size_t size_;

        // Volatile to keep values in registers
        volatile size_t readIndex_;
        volatile size_t writeIndex_;
    };
}

#endif
