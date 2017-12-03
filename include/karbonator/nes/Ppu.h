#ifndef KARBONATOR_NES_PPU_H
#define KARBONATOR_NES_PPU_H

#include <stdint.h>

typedef enum PpuConstant_ {
    ppuSpriteCount = 64,
    ppuSpritesPerScanline = 8,
    ppuCyclesPerScanline = 341,
    ppuVblankScanlineCount = 20,
    ppuVisibleScanlineCount = 240,
    ppuTotalScanlineCount = ppuVisibleScanlineCount + 1 + ppuVblankScanlineCount + 1,
    ppuPaletteCount = 64
} PpuConstant;

typedef struct Ppu_ {
    //Internal Memory Chips
    uint8_t oam [ppuSpriteCount*4];
    uint8_t tempOAM [ppuSpritesPerScanline*4];
    uint8_t palette [0x10*2];

    //PPU Register Open Bus
    uint8_t regDecayedValue;

    //PPU Controller Registers
    uint8_t regController;    //(0, W)
    uint8_t regMask;          //(1, W)
    uint8_t regStatus;        //(2, R)
    uint8_t vBlankFlagSignal; //VBlank 플래그 신호

    //VRAM Registers
    uint16_t regVRAMAddr;      //(14, R/W)
    uint16_t tempVRAMAddr;
    uint8_t regVRAMFinePosX;  //(3)
    uint8_t regVRAMLatch;     //(1)
    uint8_t regVRAMData;      //(7, R/W)

    //OAM Registers
    uint8_t regOAMAddr;       //(3, W)
    uint8_t tempOAMAddr;
    uint8_t regOAMData;       //(4, R/W)
    uint8_t sprInRangeCount;
    bool sprZeroFound;

    //Registers For Rendering
    uint16_t tileBitmapL; //shift register
    uint16_t tileBitmapH; //shift register
    uint8_t tileAttrL; //shift register
    uint8_t tileAttrH; //shift register
    uint16_t tileAttrLLatch;
    uint16_t tileAttrHLatch;
    uint16_t tempTileNo;
    uint16_t tempAttrNo;
    uint16_t tempBitmapL;
    uint16_t tempBitmapH;
    uint16_t tempPosY;
    uint16_t tempPosX;

    uint8_t sprTileBitmapL [ppuSpritesPerScanline]; //shift register
    uint8_t sprTileBitmapH [ppuSpritesPerScanline]; //shift register
    uint8_t sprAttr [ppuSpritesPerScanline];
    uint8_t sprPosXCounter [ppuSpritesPerScanline];
    uint8_t sprPixelIndex [ppuSpritesPerScanline];
    uint8_t sprCount;
    uint8_t sprZeroPosXCounter;
    bool hasSprZero;

    //CPU Pins
    //HByte * addrBusH;
    //HByte * addrLLatch;
    //HWord * addrBus;                //(6) Address high byte
    //HByte * dataBus;                //(8) Data bus, address low byte
    //HEInterruptSignal * vBlank;     //(1) 2A03 NMI 버스로 연결

    //HWord addrLatch;                //(14) (External Chip)

    //Internal Status
    //HAddressDecoder * decoder;              //주소 디코더
    //void (HRP2C02::* sequencerCodeBlock)(); //시퀀서 코드 블록
    //void (HRP2C02::* bgCodeBlock)();        //VRAM 코드 블록
    //void (HRP2C02::* sprCodeBlock)();       //OAM 코드 블록
    unsigned int cycles;                   //사이클 카운터
    uint16_t scanline;                 //스캔라인 카운터
    bool oddFrame;                  //짝/홀 프레임 플래그
    bool renderingFlag;             //렌더링 플래그
    bool bgBitmapShiftFlag;         //BG 비트맵 시프트 플래그
    unsigned int sqDelayCounter;
    unsigned int bgDelayCounter;
    unsigned int sprDelayCounter;

    //Generated Pixel
    uint8_t pixel;                    //생성된 픽셀
    
    //Output Device
    //HUInt32 * _surface;              //서페이스
    //HUInt32 * _surfacePtr;           //서페이스 포인터
    //HUInt32 * _rgbTable;             //RGB 색상 테이블
} Ppu;

#endif
