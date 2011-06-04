#include "CHash.h"

int c_crctab[256];

//-------------------------------------------------------------------------------------------------------
uint32 CHash::Hash (void* ptr, uint32 length) {
    char* block = (char*)ptr;
    //register uint32 crc;
    uint32 crc;
    uint32 i;
    crc = 0xFFFFFFFF;
    for  (i = 0; i < length; i++) {
        crc = ((crc >> 8) & 0x00FFFFFF) ^ c_crctab[(crc ^ *block++) & 0xFF];
    }
    return (crc ^ 0xFFFFFFFF);
}
//-------------------------------------------------------------------------------------------------------
void CHash::Initialize () {
    uint32 crc, poly;
    int i, j;
    poly = 0xEDB88320L;
    for  (i = 0; i < 256; i++) {
        crc = i;
        for  (j = 8; j > 0; j--) {
            if  (crc & 1) {
                crc = (crc >> 1) ^ poly;
            } else {
                crc >>= 1;
            }
        }
        c_crctab[i] = crc;
    }
}