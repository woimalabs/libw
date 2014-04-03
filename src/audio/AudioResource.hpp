/**
 * libw
 *
 * Copyright (C) 2013-2014 Woima Solutions Oy, Ltd.
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

#ifndef LIBW_AUDIO_AUDIORESOURCE
#define LIBW_AUDIO_AUDIORESOURCE

#include "w/base/Resource.hpp"
#include "w/base/FileHandle.hpp"
#include <string>
#include <stdint.h>

namespace w
{
    namespace audio
    {
        class AudioResource: public Resource
        {
        public:
            AudioResource(FileHandle* fileHandle);
            ~AudioResource();
            unsigned int channels() const;
            unsigned int bitPerSample(void) const;
            unsigned int bytesPerSample(void) const;
            int playBackRate(void) const;
            unsigned int sizeInBytes(void) const;
            bool isSigned(void) const;
            const unsigned char* data(void) const;
            int16_t sample(unsigned int location, bool& end) const;

        private:
            void load(FileHandle*);
            bool channels_;
            unsigned int bit_;
            unsigned int bytesPerSample_;
            int rate_;
            unsigned int sizeInBytes_;
            bool isSigned_;
            unsigned char* data_;
        };
    }
}

#endif
