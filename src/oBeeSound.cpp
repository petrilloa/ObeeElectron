#include "oBeeSound.h"
#include "application.h"

oBeeSound::oBeeSound()
{
    }

void oBeeSound::Begin(char * iSongBuffer)
{
  rtttl::stop();
  rtttl::begin(pin, iSongBuffer);
}

bool oBeeSound::IsPlaying()
{
  return rtttl::isPlaying();
}

void oBeeSound::Stop()
{
  return rtttl::stop();
}

void oBeeSound::Update()
{
  if ( rtttl::isPlaying() )
  {
    rtttl::play();
  }
}
