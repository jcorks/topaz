#include <vorbis/vorbisfile.h>
#include <stdio.h>
int main() {
    OggVorbis_File ogg = {};
    printf("%d", sizeof(ogg));
    return 0;
}
