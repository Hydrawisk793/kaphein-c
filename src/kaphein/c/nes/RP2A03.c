#include "kaphein/c/nes/RP2A03.h"

enum Action
{
    push,
    pop,
    
    load,
    store,

    move,
    setStatusBit,

    add,
    subtract,
    
    compare,
    noOperation,

    increment,
    decrement,

    rotateLeft,
    rotateRight,
    
    arithmeticShiftLeft,
    logicalShiftRight,

    bitAnd,
    bitTest,

    bitOr,
    bitXor,

    branch,
    jump,

    returnFromSubroutine,
    returnFromInterrupt,
    
    jumpToSubroutine,
    interrupt
};

static int decodeOpcode(
    uint8_t opCode
) {
    unsigned int groupNdx = opCode & 0x03;
    //unsigned int groupTblNdx = opCode & 0x01;
    unsigned int addrModeNdx = (opCode & 0x1C) >> 2;
    unsigned int actionNdx = (opCode & 0xE0) >> 5;
    
    unsigned int addrMode = 0;
    unsigned int actionCode = 0;
    
    switch(groupNdx) {
    case 0:
        switch(addrModeNdx) {
        case 0:
            switch(actionNdx) {
            case 0:
                actionCode = 0;//Instruction._intSrActionCodes[actionNdx];
                addrMode = 0;//Instruction.AddressingMode.imp;
            break;
            case 1:
                actionCode = 0;//Instruction._intSrActionCodes[actionNdx];
                addrMode = 0;//Instruction.AddressingMode.abs;
            break;
            case 2:
            case 3:
                actionCode = 0;//Instruction._intSrActionCodes[actionNdx];
                addrMode = 0;//Instruction.AddressingMode.imp;
            break;
            case 4:
                //throw new Error("An invalid opcode has been detected.");
                return -1;
            //break;
            case 5:
            case 6:
            case 7:
                actionCode = 0;//Instruction._groupActionCodeTable[0][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            break;
            }
        break;
        case 1:
            switch(actionNdx) {
            case 0:
                //throw new Error("An invalid opcode has been detected.");
                return -2;
            //break;
            case 1:
                actionCode = 0;//Instruction._groupActionCodeTable[0][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            break;
            case 2:
            case 3:
                //throw new Error("An invalid opcode has been detected.");
                return -3;
            //break;
            case 4:
            case 5:
            case 6:
            case 7:
                actionCode = 0;//Instruction._groupActionCodeTable[0][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            break;
            }
        break;
        case 2:
            actionCode = 0;//Instruction._stackMoveIncDecActionCodes[actionNdx];
            addrMode = 0;//Instruction.AddressingMode.imp;
        break;
        case 3:
            switch(actionNdx) {
            case 0:
                //throw new Error("An invalid opcode has been detected.");
                return -4;
            //break;
            case 1:
            case 2:
                actionCode = 0;//Instruction._groupActionCodeTable[0][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            break;
            case 3:
                actionCode = 0;//Instruction._groupActionCodeTable[0][actionNdx];
                addrMode = 0;//Instruction.AddressingMode.absInd;
            break;
            case 4:
            case 5:
            case 6:
            case 7:
                actionCode = 0;//Instruction._groupActionCodeTable[0][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            break;
            }
        break;
        case 4:
            actionCode = 0;//Instruction._branchActionCodes[actionNdx];
            addrMode = 0;//Instruction.AddressingMode.pcRel;
        break;
        case 5:
            switch(actionNdx) {
            case 0:
            case 1:
            case 2:
            case 3:
                //throw new Error("An invalid opcode has been detected.");
                return -5;
            //break;
            case 4:
            case 5:
                actionCode = 0;//Instruction._groupActionCodeTable[0][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            break;
            case 6:
            case 7:
                //throw new Error("An invalid opcode has been detected.");
                return -6;
            //break;
            }
        break;
        case 6:
            actionCode = 0;//Instruction._flagMoveActionCodes[actionNdx];
            addrMode = 0;//Instruction.AddressingMode.imp;
        break;
        case 7:
            if(actionNdx == 5) {
                actionCode = 0;//Instruction._groupActionCodeTable[0][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            }
            else {
                //4 : SHY
                //.throw new Error("An invalid opcode has been detected.");
                return -7;
            }
        break;
        }
    break;
    case 1:
        if(opCode != 0x89) {
            actionCode = 0;//Instruction._groupActionCodeTable[1][actionNdx];
            addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
        }
        else {
            //throw new Error("An invalid opcode has been detected.");
            return -8;
        }
    break;
    case 2:
        switch(addrModeNdx) {
        case 0:
            if(actionNdx == 5) {
                actionCode = 0;//Instruction._groupActionCodeTable[2][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            }
            else {
                //throw new Error("An invalid opcode has been detected.");
                return -9;
            }
        break;
        case 1:
            actionCode = 0;//Instruction._groupActionCodeTable[2][actionNdx];
            addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
        break;
        case 2:
            switch((actionNdx & 0x04) >> 2) {
            case 0:
                actionCode = 0;//Instruction._groupActionCodeTable[2][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            break;
            case 1:
                actionCode = 0;//Instruction._moveDecNopActionCodes[actionNdx & 0x03];
                addrMode = 0;//Instruction.AddressingMode.imp;
            break;
            }
        break;
        case 3:
            actionCode = 0;//Instruction._groupActionCodeTable[2][actionNdx];
            addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
        break;
        case 4:
            //throw new Error("An invalid opcode has been detected.");
            return -10;
        //break;
        case 5:
            actionCode = 0;//Instruction._groupActionCodeTable[2][actionNdx];
            addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
        break;
        case 6:
            switch(actionNdx) {
            case 0:
            case 1:
            case 2:
            case 3:
                //throw new Error("An invalid opcode has been detected.");
                return -11;
            //break;
            case 4:
            case 5:
                actionCode = 0;//Instruction._stackPointerActionCodes[actionNdx - 4];
                addrMode = 0;//Instruction.AddressingMode.imp;
            break;
            case 6:
            case 7:
                //throw new Error("An invalid opcode has been detected.");
                return -12;
            //break;
            }
        break;
        case 7:
            if(actionNdx != 4) {
                actionCode = 0;//Instruction._groupActionCodeTable[2][actionNdx];
                addrMode = 0;//Instruction._addrModeTable[groupTblNdx][addrModeNdx];
            }
            else {
                //SHX
                //throw new Error("An invalid opcode has been detected.");
                return -13;
            }
        break;
        }
    break;
    case 3:
        //throw new Error("An invalid opcode has been detected.");
        return -14;
    //break;
    }

    return 0;
}

int kphnNesRP2A03Run(
    struct kphnNesRP2A03* pContext
)
{
    pContext;

    return -1;
}
