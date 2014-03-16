/**
 * libw
 *
 * Copyright (C) 2013 Woima Solutions
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

#include "AudioResource.hpp"
#include <w/base/Log.hpp>
#include <w/base/Exception.hpp>
#include <string.h>

namespace w
{
    namespace audio
    {
        static unsigned getUnsigned(const unsigned char buf[])
        {
            return buf[0] + (buf[1] * 0x100) + (buf[2] * 0x10000) + (buf[3] * 0x1000000);
        }

        static unsigned getUnsignedShort(const unsigned char buf[])
        {
            return buf[0] + (buf[1] * 0x100);
        }

        AudioResource::AudioResource(FileHandle* fileHandle):
            Resource(fileHandle->filename()),
            channels_(false),
            bit_(0),
            bytesPerSample_(0),
            rate_(0),
            sizeInBytes_(0),
            isSigned_(false),
            data_(NULL)
        {
            load(fileHandle);
        }

        AudioResource::~AudioResource()
        {
        }

        unsigned int AudioResource::channels() const
        {
            return channels_;
        }

        unsigned int AudioResource::bitPerSample(void) const
        {
            return bit_;
        }

        unsigned int AudioResource::bytesPerSample(void) const
        {
            return bytesPerSample_;
        }

        int AudioResource::playBackRate(void) const
        {
            return rate_;
        }

        unsigned int AudioResource::sizeInBytes(void) const
        {
            return sizeInBytes_;
        }

        bool AudioResource::isSigned(void) const
        {
            return isSigned_;
        }

        unsigned char const* AudioResource::data(void) const
        {
            return data_;
        }

        int16_t AudioResource::sample(unsigned int location, bool& end) const
        {
            int16_t r = 0;

            if (location >= (sizeInBytes_ - bytesPerSample_))
            {
                end = true;
            }
            else
            {
                // TODO: Polish this cast.
                r = *(int*)(&data_[location]);
            }
            return r;
        }

        void AudioResource::load(FileHandle* fileHandle)
        {
            if (fileHandle != NULL)
            {
                unsigned char buf[256] = {0};
                unsigned int l = 0;

                // Header
                if (fileHandle->read((char*)&buf[0], 4) != 4)
                {
                    LOGE("Error in reading RIFF.");
                    goto error;
                }

                if (strncmp((char*)buf, "RIFF", 4) != 0)
                {
                    LOGI("RIFF not found.");
                }

                if (fileHandle->read((char*)&buf[0], 4) != 4)
                {
                    LOGE("Error in reading file size.");
                    goto error;
                }

                if (fileHandle->read((char*)&buf[0], 8) != 8)
                {
                    LOGE("WAVEfmt could not be read.");
                    goto error;
                }

                if (strncmp((char*)buf, "WAVEfmt", 7) != 0)
                {
                    LOGI("WAVEfmt not found.");
                }

                if (fileHandle->read((char*)&buf[0], 4) != 4)
                {
                    LOGE("Header size could not be read.");
                    goto error;
                }
                unsigned int headerSize = getUnsigned(buf);

                // WORD  formatTag;
                // WORD  channels;
                // DWORD samplesPerSecond;
                // DWORD averageBytesPerSecond;
                // WORD  blockAlign;
                // WORD  bitsPerSample;
                // WORD  cbSize;
                if (fileHandle->read((char*)&buf[0], headerSize) != headerSize)
                {
                    LOGE("Error in reading header.");
                    goto error;
                }

                unsigned short channels = getUnsignedShort(buf + 2);
                unsigned int samplesPerSecond = getUnsigned(buf + 4);
                unsigned short bitsPerSample = (headerSize >= 16 ? getUnsignedShort(buf + 14) : 0);

                /*LOGD("formatTag=%d\n", formatTag);
                LOGD("channels=%d\n", channels);
                LOGD("samplesPerSecond=%d\n", samplesPerSecond);
                LOGD("averageBytesPerSecond=%d\n", averageBytesPerSecond);
                LOGD("blockAlign=%d\n", blockAlign);
                LOGD("bitsPerSample=%d\n", bitsPerSample);
                LOGD("cbSize=%d\n", cbSize);*/

                // Data
                while (1)
                {
                    if (fileHandle->read((char*)&buf[0], 4) != 4)
                    {
                        printf("Error while waiting for data.");
                        goto error;
                    }

                    if ((buf[0]=='d' || buf[0]=='D') && (buf[1]=='a' || buf[1]=='A') &&
                        (buf[2]=='t' || buf[2]=='T') && (buf[3]=='a' ||  buf[3]=='A'))
                    {
                        // "DATA" block found
                        break;
                    }
                    else
                    {
                        LOGI("Skipping block \"%c%c%c%c\"", buf[0], buf[1], buf[2], buf[3]);

                        if (fileHandle->read((char*)&buf[0], 4) != 4)
                        {
                            LOGE("Skipping unknown block failed!");
                            goto error;
                        }

                        l = getUnsigned(buf);
                        if (fileHandle->read((char*)&buf[0], l) != l)
                        {
                            LOGE("Skipping unknown block failed!");
                            goto error;
                        }
                    }
                }

                if (fileHandle->read((char*)&buf[0], 4) != 4)
                {
                    LOGE("Data size could not be read.");
                    goto error;
                }
                unsigned int dataSize = getUnsigned(buf);

                if (data_ != NULL)
                {
                    LOGI("Replacing earlier data.");
                    delete data_;
                    data_ = NULL;
                }

                data_ = new unsigned char [dataSize];
                if ((l = fileHandle->read((char*)data_, dataSize)) != dataSize)
                {
                    LOGI("File ended before reading all data.");
                    LOGI("%d (0x%x) bytes have been read.", l, l);
                }

                channels_ = channels;
                bit_ = bitsPerSample;
                bytesPerSample_ = bitsPerSample >> 3;
                sizeInBytes_ = dataSize;
                rate_ = samplesPerSecond;

                if (bitsPerSample == 8)
                {
                    isSigned_ = false;
                }
                else
                {
                    isSigned_= true;
                }

                return;
            }
            else
            {
                LOGE("No file: \"%s\"", fileHandle->filename().c_str());
            }
            return;

        error:
            throw Exception("Could not load audio asset!");
        }
    }
}
