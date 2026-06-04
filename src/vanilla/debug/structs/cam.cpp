/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

#include "basicstructs.h"
#include "cam.h"
#include "../memdump.h"
#include "../datatools.h"
#include <iostream>
#include <sstream>

ParseCamera::ParseCamera(MemDump* parentMemdump) {
   mParentMemDump = parentMemdump;
}

void ParseCamera::Print() {
    std::cout << Position->GetAsString() << std::endl;
    std::cout << Destination->GetAsString() << std::endl;
    std::cout << AngleXY->GetAsString() << std::endl;
    std::cout << AngleZY->GetAsString() << std::endl;
    std::cout << AngleXZ->GetAsString() << std::endl;
    std::cout << Shading->GetAsString() << std::endl;
    std::cout << Sky->GetAsString() << std::endl;
    std::cout << SplittingLow->GetAsString() << std::endl;
    std::cout << SplittingHigh->GetAsString() << std::endl;
    std::cout << SolidPolygons->GetAsString() << std::endl;
    std::cout << Lens->GetAsString() << std::endl;
    std::cout << Distance->GetAsString() << std::endl;
    std::cout << Follow->GetAsString() << std::endl;
    std::cout << Zoom->GetAsString() << std::endl;
    std::cout << Static->GetAsString() << std::endl;
    std::cout << Action->GetAsString() << std::endl;
}

void ParseCamera::Update(size_t fromAdr) {
    Position = new Coord3DClass(mParentMemDump->mDataTools, std::string("Position"), fromAdr);
    Destination = new Coord3DClass(mParentMemDump->mDataTools, std::string("Destination"), fromAdr + 0x8);

    AngleXY  = mParentMemDump->mDataTools->AddFixedPointInt16_8R8NumVarAngle(std::string("AngleXY"), fromAdr + 0x10);
    AngleZY = mParentMemDump->mDataTools->AddFixedPointInt16_8R8NumVarAngle(std::string("AngleZY"), fromAdr + 0x12);
    AngleXZ = mParentMemDump->mDataTools->AddFixedPointInt16_8R8NumVarAngle(std::string("AngleXZ"), fromAdr + 0x14);

    Shading = mParentMemDump->mDataTools->AddInt32_NumVar(std::string("Shading"), fromAdr + 0x18);
    Sky = mParentMemDump->mDataTools->AddInt32_NumVar(std::string("Sky"), fromAdr + 0x1C);
    SplittingLow = mParentMemDump->mDataTools->AddInt32_NumVar(std::string("SplittingLow"), fromAdr + 0x20);
    SplittingHigh = mParentMemDump->mDataTools->AddInt32_NumVar(std::string("SplittingHigh"), fromAdr + 0x24);
    SolidPolygons = mParentMemDump->mDataTools->AddInt32_NumVar(std::string("SolidPolygons"), fromAdr +  0x28);

    Lens = mParentMemDump->mDataTools->AddFixedPointInt32_8R8NumVar(std::string("Lens"), fromAdr + 0x2C);
    Distance = mParentMemDump->mDataTools->AddFixedPointInt32_8R8NumVar(std::string("Distance"), fromAdr + 0x30);

    Follow = mParentMemDump->mDataTools->AddInt16_NumVar(std::string("Follow"), fromAdr + 0x34);
    Zoom = mParentMemDump->mDataTools->AddInt16_NumVar(std::string("Zoom"), fromAdr + 0x36);
    Static = mParentMemDump->mDataTools->AddInt16_NumVar(std::string("Static"), fromAdr + 0x38);
    Action = mParentMemDump->mDataTools->AddInt16_NumVar(std::string("Action"), fromAdr + 0x3A);
}

ParseCamera::~ParseCamera() {
    if (Position != nullptr) {
        delete Position;
        Position = nullptr;
    }

    if (Destination != nullptr) {
        delete Destination;
        Destination = nullptr;
    }

    if (AngleXY != nullptr) {
        delete AngleXY;
        AngleXY = nullptr;
    }

    if (AngleZY != nullptr) {
        delete AngleZY;
        AngleZY = nullptr;
    }

    if (AngleXZ != nullptr) {
        delete AngleXZ;
        AngleXZ = nullptr;
    }

    if (Shading != nullptr) {
        delete Shading;
        Shading = nullptr;
    }

    if (Sky != nullptr) {
        delete Sky;
        Sky = nullptr;
    }

    if (SplittingLow != nullptr) {
        delete SplittingLow;
        SplittingLow = nullptr;
    }

    if (SplittingHigh != nullptr) {
        delete SplittingHigh;
        SplittingHigh = nullptr;
    }

    if (SolidPolygons != nullptr) {
        delete SolidPolygons;
        SolidPolygons = nullptr;
    }

    if (Lens != nullptr) {
        delete Lens;
        Lens = nullptr;
    }

    if (Distance != nullptr) {
        delete Distance;
        Distance = nullptr;
    }

    if (Follow != nullptr) {
        delete Follow;
        Follow = nullptr;
    }

    if (Zoom != nullptr) {
        delete Zoom;
        Zoom = nullptr;
    }

    if (Static != nullptr) {
        delete Static;
        Static = nullptr;
    }

    if (Action != nullptr) {
        delete Action;
        Action = nullptr;
    }
}
