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

#include "AudioEnginePrivate.hpp"
#include <w/Log.hpp>
#include <w/Exception.hpp>
#include "Timer.hpp"


namespace w
{
    AudioEnginePrivate* AudioEnginePrivate::singleton_ = NULL;

    static OSStatus playbackCallback(void* selfPointer, AudioUnitRenderActionFlags* ioActionFlags,
        const AudioTimeStamp* inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames,
        AudioBufferList* ioData)
    {
        AudioEnginePrivate::AudioEnginePrivate* self = (AudioEnginePrivate::AudioEnginePrivate*)selfPointer;
        SInt16* outSamples = (SInt16*)ioData->mBuffers[0].mData;
        self->writeCallback(inNumberFrames * 2 /* 2 bytes per sample */, outSamples);
        return noErr;
    }

    static OSStatus recordingCallback(void* self,
        AudioUnitRenderActionFlags* ioActionFlags,
        const AudioTimeStamp* inTimeStamp,
        UInt32 inBusNumber,
        UInt32  inNumberFrames,
        AudioBufferList* ioData)
    {
        return noErr;
    }

    AudioEnginePrivate::AudioEnginePrivate(float volume, const std::string& assetPath):
        audioResourceManager_(assetPath),
        tracker_(volume)
    {
        // iOS's AudioUnit initialization
        setupAudioUnitSession();
        setupAudioGraph(1);

        singleton_ = this;
    }

    AudioEnginePrivate::~AudioEnginePrivate()
    {
        LOGI("AudioEnginePrivate::shutting down...\n");
        OSStatus status = AudioOutputUnitStop(ioUnit_);
        if (noErr != status)
        {
            throw Exception("AudioOutputUnitStop failed");
        }

        //while (state_ == AudioMixer::State::ShuttingDown) // TODO
        //{
            Timer::sleepMilliseconds(500); // wait until shutdown is completed
        //}

        AudioComponentInstanceDispose(ioUnit_);
        LOGI("AudioEnginePrivate::shutdown completed.\n");
    }

    void AudioEnginePrivate::setupAudioUnitSession()
    {
        AVAudioSession* session = [AVAudioSession sharedInstance];

        // TODO: Specify that this object is the delegate of the audio session, so that
        //       this object's endInterruption method will be invoked when needed.

        // Set to handle interrupts (call, alarm, etc.)
        NSError *error = nil;
        [session setCategory: AVAudioSessionCategoryAmbient error: &error];
        if (error != nil)
        {
            throw Exception("AVAudioSession setCategory() failed.");
            return;
        }

        // Activate
        [session setActive: YES error: &error];
        if (error != nil)
        {
            throw Exception("Error activating audio session during initial setup.");
            return;
        }
    }

    void AudioEnginePrivate::setupAudioGraph(UInt32 busCount)
    {
        OSStatus result = noErr;

        // Setup audio units
        AudioComponentDescription iODescription;
        iODescription.componentType = kAudioUnitType_Output; // is both: i and o, NOTE Recording to be removed!
        iODescription.componentSubType = kAudioUnitSubType_RemoteIO;
        iODescription.componentManufacturer = kAudioUnitManufacturer_Apple;
        iODescription.componentFlags = 0;
        iODescription.componentFlagsMask = 0;

        // Get component
        AudioComponent inputComponent = AudioComponentFindNext(NULL, &iODescription);

        // Get audio units
        result = AudioComponentInstanceNew(inputComponent, &ioUnit_);
        if (noErr != result)
        {
            throw Exception("AudioComponentInstanceNew failed");
        }

        // Enable IO for recording, NOTE: To be removed!
        UInt32 flag = 1;
        result = AudioUnitSetProperty(ioUnit_,
            kAudioOutputUnitProperty_EnableIO,
            kAudioUnitScope_Input,
            1, // input
            &flag,
            sizeof(flag));

        if (noErr != result)
        {
            throw Exception("AudioUnitSetProperty failed");
        }

        // Enable IO for playback
        result = AudioUnitSetProperty(ioUnit_,
            kAudioOutputUnitProperty_EnableIO,
            kAudioUnitScope_Output,
            0, // output
            &flag,
            sizeof(flag));

        if (noErr != result)
        {
            throw Exception("AudioUnitSetProperty failed");
        }

        // Describe format
        streamFormat_.mSampleRate       = 44100.00;
        streamFormat_.mFormatID         = kAudioFormatLinearPCM;
        streamFormat_.mFormatFlags      = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        streamFormat_.mFramesPerPacket  = 1;
        streamFormat_.mChannelsPerFrame = 1;
        streamFormat_.mBitsPerChannel   = 16;
        streamFormat_.mBytesPerPacket   = 2;
        streamFormat_.mBytesPerFrame    = 2;

        // Apply format
        result = AudioUnitSetProperty(ioUnit_,
            kAudioUnitProperty_StreamFormat,
            kAudioUnitScope_Output,
            1, // input
            &streamFormat_,
            sizeof(streamFormat_));

        if (noErr != result)
        {
            throw Exception("AudioUnitSetProperty (input format) failed");
        }

        result = AudioUnitSetProperty(ioUnit_,
            kAudioUnitProperty_StreamFormat,
            kAudioUnitScope_Input,
            0, // output
            &streamFormat_,
            sizeof(streamFormat_));

        if (noErr != result)
        {
            throw Exception("AudioUnitSetProperty (output format) failed");
        }

        // Set input callback
        AURenderCallbackStruct callbackStruct;
        callbackStruct.inputProc = recordingCallback;
        callbackStruct.inputProcRefCon = this; // THIS
        result = AudioUnitSetProperty(ioUnit_,
            kAudioOutputUnitProperty_SetInputCallback,
            kAudioUnitScope_Global,
            1,
            &callbackStruct,
            sizeof(callbackStruct));

        if (noErr != result)
        {
            throw Exception("AudioUnitSetProperty (recordingCallback) failed");
        }

        // Set output callback
        callbackStruct.inputProc = playbackCallback;
        callbackStruct.inputProcRefCon = this;
        result = AudioUnitSetProperty(ioUnit_,
            kAudioUnitProperty_SetRenderCallback,
            kAudioUnitScope_Global,
            0,
            &callbackStruct,
            sizeof(callbackStruct));

        if (noErr != result)
        {
            throw Exception("AudioUnitSetProperty (playbackCallback) failed");
        }

        // Disable recorder buffer, we use own
        flag = 0;
        result = AudioUnitSetProperty(ioUnit_,
            kAudioUnitProperty_ShouldAllocateBuffer,
            kAudioUnitScope_Output,
            1,
            &flag,
            sizeof(flag));

        if (noErr != result)
        {
            throw Exception("AudioUnitSetProperty (output format) failed");
        }

        result = AudioUnitInitialize(ioUnit_);
        if (noErr != result)
        {
            throw Exception("AudioUnitInitialize failed");
        }

        result = AudioOutputUnitStart(ioUnit_);
        if (noErr != result)
        {
            throw Exception("AudioOutputUnitStart failed");
        }

        CAShow (ioUnit_);

        union
        {
            OSStatus propertyResult;
            char a[4];
        } u;

        Float32 realIOBufferDuration = 0.0;
        Float32 requestedIOBufferDuration = 0.005;
        UInt32 datasize = 4;

        u.propertyResult = AudioSessionGetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, &datasize, &realIOBufferDuration);
        //LOGD("Get IO Duration Time %ld %lx %c%c%c%c", u.propertyResult, u.propertyResult, u.a[3], u.a[2], u.a[1], u.a[0]);
        //LOGD("IO Buffer Duration is %f", realIOBufferDuration);

        u.propertyResult = AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, sizeof(requestedIOBufferDuration), &requestedIOBufferDuration);
        //LOGD("Set IO Duration Time %ld %lx %c%c%c%c", u.propertyResult, u.propertyResult, u.a[3], u.a[2], u.a[1], u.a[0]);

        u.propertyResult = AudioSessionGetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, &datasize, &realIOBufferDuration);
        //LOGD("Get IO Duration Time %ld %lx %c%c%c%c", u.propertyResult, u.propertyResult, u.a[3], u.a[2], u.a[1], u.a[0]);
        //LOGD("IO Buffer Duration is %f", realIOBufferDuration);
    }

    void AudioEnginePrivate::writeCallback(size_t size, SInt16* targetBuffer)
    {
        tracker_.getData(size, (unsigned char*)targetBuffer);
    }

    void audioRouteChangeListenerCallback (void *inUserData,
        AudioSessionPropertyID inPropertyID, UInt32 inPropertyValueSize,
        const void* inPropertyValue)
    {
        // TODO
        LOGE("Stub: audioRouteChangeListenerCallback\n");
    }

}
