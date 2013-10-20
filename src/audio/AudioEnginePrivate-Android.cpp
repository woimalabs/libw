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

#include "w/audio/AudioEngine.hpp"
#include "w/Exception.hpp"
#include "AudioEnginePrivate.hpp"
#include "Lock.hpp"
#include "w/Log.hpp"

namespace w
{
    AudioEnginePrivate* AudioEnginePrivate::singleton_ = NULL;

    // Callback called when buffer finishes playing
    void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void* handle)
    {
        LOGD("bqPlayerCallback");
        AudioEnginePrivate* self = (AudioEnginePrivate*)handle;
        if (self == NULL)
        {
            LOGE("bqPlayerCallback, NULL == self");
            return;
        }
        else
        {
            self->writeCallback();
        }
    }

    void AudioEnginePrivate::test()
    {
        unsigned int dataGot = tracker_.getData(44100, &(buffer_[0]));

        SLresult result = (*bqPlayerBufferQueue_)->Enqueue(bqPlayerBufferQueue_, &(buffer_[0]), dataGot);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("enqueue, Enqueue !SL_RESULT_SUCCESS");
        }
    }

    // create the engine and output mix objects
    void AudioEnginePrivate::createEngine()
    {
        SLresult result;

        // Create engine
        result = slCreateEngine(&engineObject_, 0, NULL, 0, NULL, NULL);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createEngine, slCreateEngine !SL_RESULT_SUCCESS");
            return;
        }

        // Realize the engine
        result = (*engineObject_)->Realize(engineObject_, SL_BOOLEAN_FALSE);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createEngine, Realize engine !SL_RESULT_SUCCESS");
            return;
        }

        // Get the engine interface, which is needed in order to create other objects
        result = (*engineObject_)->GetInterface(engineObject_, SL_IID_ENGINE, &engineEngine_);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createEngine, GetInterface !SL_RESULT_SUCCESS");
            return;
        }

        // Create OutputMixer.
        result = (*engineEngine_)->CreateOutputMix(engineEngine_, &outputMixObject_, 0, NULL, NULL);
        if (result != SL_RESULT_SUCCESS)
        {
            LOGE("createEngine, CreateOutputMix !SL_RESULT_SUCCESS");
            return;
        }

        // Realize the output mix
        result = (*outputMixObject_)->Realize(outputMixObject_, SL_BOOLEAN_FALSE);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createEngine, Realize ouput !SL_RESULT_SUCCESS");
            return;
        }
    }

    // create buffer queue audio player
    void AudioEnginePrivate::createBufferQueueAudioPlayer()
    {
        SLresult result;

        // Configure audio source
        SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};

        // Configure data format.
        SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};

         SLDataSource audioSrc = {&loc_bufq, &format_pcm};

        // Configure audio sink
        SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject_};
        SLDataSink audioSnk = {&loc_outmix, NULL};

        // Create audio player
        const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, /*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
        const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
        result = (*engineEngine_)->CreateAudioPlayer(engineEngine_, &bqPlayerObject_, &audioSrc, &audioSnk, 3, ids, req);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createBufferQueueAudioPlayer, CreateAudioPlayer !SL_RESULT_SUCCESS");
            return;
        }

        // Realize the player
        result = (*bqPlayerObject_)->Realize(bqPlayerObject_, SL_BOOLEAN_FALSE);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createBufferQueueAudioPlayer, Realize player !SL_RESULT_SUCCESS");
            return;
        }

        // Get the play interface
        result = (*bqPlayerObject_)->GetInterface(bqPlayerObject_, SL_IID_PLAY, &bqPlayerPlay_);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createBufferQueueAudioPlayer, GetInterface play !SL_RESULT_SUCCESS");
            return;
        }

        // Get the buffer queue interface
        result = (*bqPlayerObject_)->GetInterface(bqPlayerObject_, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue_);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createBufferQueueAudioPlayer, GetInterface buffer !SL_RESULT_SUCCESS");
            return;
        }

        // Register callback on the buffer queue
        result = (*bqPlayerBufferQueue_)->RegisterCallback(bqPlayerBufferQueue_, bqPlayerCallback, this);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createBufferQueueAudioPlayer, RegisterCallback !SL_RESULT_SUCCESS");
            return;
        }

        // Get the volume interface
        result = (*bqPlayerObject_)->GetInterface(bqPlayerObject_, SL_IID_VOLUME, &bqPlayerVolume_);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createBufferQueueAudioPlayer, GetInterface volume !SL_RESULT_SUCCESS");
            return;
        }

        // Set the player's state to playing
        result = (*bqPlayerPlay_)->SetPlayState(bqPlayerPlay_, SL_PLAYSTATE_PLAYING);
        if (SL_RESULT_SUCCESS != result)
        {
            LOGE("createBufferQueueAudioPlayer, set player state !SL_RESULT_SUCCESS");
            return;
        }
    }

    // Shut down the native audio system
    void AudioEnginePrivate::shutdown()
    {
        // destroy buffer queue audio player object, and invalidate all associated interfaces
        if (bqPlayerObject_ != NULL)
        {
            (*bqPlayerObject_)->Destroy(bqPlayerObject_);
            bqPlayerObject_ = NULL;
            bqPlayerPlay_ = NULL;
            bqPlayerBufferQueue_ = NULL;
            bqPlayerEffectSend_ = NULL;
            bqPlayerMuteSolo_ = NULL;
            bqPlayerVolume_ = NULL;
        }

        // Destroy output mix object, and invalidate all associated interfaces
        if (outputMixObject_ != NULL)
        {
            (*outputMixObject_)->Destroy(outputMixObject_);
            outputMixObject_ = NULL;
        }

        // Destroy engine object, and invalidate all associated interfaces
        if (engineObject_ != NULL)
        {
            (*engineObject_)->Destroy(engineObject_);
            engineObject_ = NULL;
            engineEngine_ = NULL;
        }
    }

    AudioEnginePrivate::AudioEnginePrivate(float volumeAtStart, ResourceManager& resourceManager):
        resourceManager_(resourceManager),
        tracker_(1.0f),
        volumeAtStart_(volumeAtStart)
    {
        if (singleton_ != NULL)
        {
            throw Exception("AudioEngine exists! Create only one AudioEngine per process!");
        }

        for (unsigned int i = 0; i < 44100; i++)
        {
            buffer_[i] = 0;
        }

        // TODO: set-up android audio sink
        createEngine();
        createBufferQueueAudioPlayer();
        test();

        // All done
        singleton_ = this;
        LOGD("AudioEnginePrivate created!");
    }

    AudioEnginePrivate::~AudioEnginePrivate()
    {
        tracker_.shutdown();
        while (tracker_.shutdownIsDone() == false)
        {
        }
    }

    void AudioEnginePrivate::writeCallback()
    {
        unsigned int gotSize = tracker_.getData(44100, &(buffer_[0]));
        SLresult result = (*bqPlayerBufferQueue_)->Enqueue(bqPlayerBufferQueue_, &(buffer_[0]), gotSize);

        if (SL_RESULT_SUCCESS == result)
        {
            // Alles klar
        }
        else if (SL_RESULT_BUFFER_INSUFFICIENT == result)
        {
            LOGI("bqPlayerCallback, Enqueue result !SL_RESULT_SUCCESS");
        }
        else
        {
            LOGE("bqPlayerCallback, Enqueue result !SL_RESULT_SUCCESS");
        }
    }
}
