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

#ifndef AUDIOHELLO_DUMMYAPPLICATION
#define AUDIOHELLO_DUMMYAPPLICATION

#include <w/ResourceManager.hpp>
#include <w/AudioEngine.hpp>
#include <w/AudioAsset.hpp>
#include <w/Storage.hpp>
#include <w/Log.hpp>
#include <unistd.h>

class DummyApplication
{
public:
    DummyApplication():
        resourceManager_("./"),
        audioEngine_(1.0f, resourceManager_),
        audioAsset_("teleport.wav")
        // storage_("libwhello")
    {
        /*
        std::string storageTestIntKey1("testInt1");
        int storageTestIntValue1 = 10;
        storage_.setInt(storageTestIntKey1, storageTestIntValue1);
        if (storage_.hasInt(storageTestIntKey1) == true)
        {
            LOGD("storage test, %s:&d (should be %s:&d)",
                storageTestIntKey1.c_str(), storage_.getInt(storageTestIntKey1),
                storageTestIntKey1.c_str(), storageTestIntValue1);
        }

        std::string storageTestIntKey2("testInt2");
        int storageTestIntValue2 = 22;
        storage_.setInt(storageTestIntKey2, storageTestIntValue2);
        if (storage_.hasInt(storageTestIntKey2) == true)
        {
            LOGD("storage test, %s:&d (should be %s:&d)",
                storageTestIntKey2.c_str(), storage_.getInt(storageTestIntKey2),
                storageTestIntKey2.c_str(), storageTestIntValue2);
        }
        */
    }

    ~DummyApplication()
    {
        LOG
    }

    void run()
    {
        audioAsset_.play();
        audioAsset_.play();
        audioAsset_.play();
        audioAsset_.play();
        sleep(1);
        audioAsset_.play();
        sleep(1);
    }

private:
    w::ResourceManager resourceManager_;
    w::AudioEngine audioEngine_;
    w::AudioAsset audioAsset_;
    //w::Storage storage_;
};

#endif
