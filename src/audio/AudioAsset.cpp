/**
 * libw
 *
 * Copyright (C) 2013-2014 Woima Solutions Oy
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

#include "AudioEnginePrivate.hpp"
#include "AudioAssetPrivate.hpp"
#include "w/audio/AudioAsset.hpp"
#include "w/base/Log.hpp"

namespace w
{
    namespace audio
    {
        AudioAsset::AudioAsset(const std::string& filename, bool parallelPlay, bool looping):
            private_(new AudioAssetPrivate(filename, parallelPlay, looping))
        {
            private_->increment();
        }

        AudioAsset::AudioAsset(AudioAsset const& r):
            private_(r.private_)
        {
            private_->increment();
        }

        AudioAsset::~AudioAsset()
        {
            private_->decrement();
            private_ = NULL;
        }

        AudioAsset& AudioAsset::operator=(AudioAsset const& r)
        {
            if (this != &r)
            {
                private_ = r.private_;
                private_->increment();
            }
            return *this;
        }

        bool AudioAsset::play(float volume)
        {
            return private_->play(volume);
        }

        void AudioAsset::setVolume(float volume)
        {
            private_->setVolume(volume);
        }

        void AudioAsset::fadeOut(unsigned int fadeOutTimeMilliseconds)
        {
            private_->fadeOut(fadeOutTimeMilliseconds);
        }
    }
}
