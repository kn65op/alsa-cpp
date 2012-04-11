#include "../include/extras.h"

using namespace TALSA;

snd_pcm_format_t getFormat(DataFormat df)
{
  switch (df)
  {
    case DataFormat::U8:
      return SND_PCM_FORMAT_U8;
    case DataFormat::U16:
      return SND_PCM_FORMAT_U16;
    default:
      throw InvalidOperation("Wrong data format");
  }
}

int getSizeOfFormat(DataFormat df)
{
  switch (df)
  {
    case DataFormat::U8:
      return 1;
    case DataFormat::U16:
      return 2;
    default:
      throw InvalidOperation("Wrong data format");
  }
}
