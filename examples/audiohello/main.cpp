#include <w/AudioEngine.hpp>
#include <w/Exception.hpp>
#include <w/Log.hpp>

int main(int argc, char *argv[])
{
    try
    {
        w::AudioEngine(1.0f, "./");
    }
    catch(w::Exception& e)
    {
        LOGE("%s", e.what());
    }

    return 0;
}

