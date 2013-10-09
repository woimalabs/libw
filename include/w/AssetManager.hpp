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

#ifndef LIBW_ASSETMANAGER
#define LIBW_ASSETMANAGER

#include <string>
#ifdef ANDROID
    #include <jni.h>
    #include <sys/types.h>
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>
#endif

namespace w
{
    class AssetManager
    {
    public:
        /**
         * AssetManager takes care of asset file loading and sharing.
         *
         * @param [in]  basePath    path to the asset files.
        */
        AssetManager(const std::string& basePath);
        virtual ~AssetManager();
        #ifdef ANDROID
            static void setAssetManager(AAssetManager* assetManager);
        #endif

    private:
        static class AssetManagerPrivate* private_;
    };
}

#endif
