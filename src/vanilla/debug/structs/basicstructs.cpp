/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

#include "basicstructs.h"
#include "../memdump.h"
#include "../datatools.h"
#include <iostream>
#include <sstream>

Coord2DClass::Coord2DClass(DataTools* parent, std::string name, size_t startPosData) {
    mParent = parent;
    mName = name;
    XPos = mParent->AddFixedPointInt16_8R8NumVar(std::string("XPos"), startPosData);
    YPos = mParent->AddFixedPointInt16_8R8NumVar(std::string("YPos"), startPosData + 0x2);
}

std::string Coord2DClass::GetAsString() {
    std::ostringstream output;

    output << mName << ": " << XPos->GetAsString() << ", " << YPos->GetAsString();

    return output.str();
}

Coord2DClass::~Coord2DClass() {
    if (XPos != nullptr) {
        delete XPos;
        XPos = nullptr;
    }
    if (YPos != nullptr) {
        delete YPos;
        YPos = nullptr;
    }
}

Coord3DClass::Coord3DClass(DataTools* parent, std::string name, size_t startPosData) {
    mParent = parent;
    mName = name;
    XPos = mParent->AddFixedPointInt16_8R8NumVar(std::string("XPos"), startPosData);
    YPos = mParent->AddFixedPointInt16_8R8NumVar(std::string("YPos"), startPosData + 0x2);
    ZPos = mParent->AddFixedPointInt16_8R8NumVar(std::string("ZPos"), startPosData + 0x4);
}

std::string Coord3DClass::GetAsString() {
    std::ostringstream output;

    output << mName << ": " << XPos->GetAsString() << ", " << YPos->GetAsString() << ", " << ZPos->GetAsString();

    return output.str();
}

Coord3DClass::~Coord3DClass() {
    if (XPos != nullptr) {
        delete XPos;
        XPos = nullptr;
    }
    if (YPos != nullptr) {
        delete YPos;
        YPos = nullptr;
    }
    if (ZPos != nullptr) {
        delete ZPos;
        ZPos = nullptr;
    }
}

MovementClass::MovementClass(DataTools* parent, std::string name, size_t startPosData) {
    mParent = parent;
    mName = name;
    AngleXY  = mParent->AddFixedPointInt16_8R8NumVarAngle(std::string("AngleXY"), startPosData);
    AngleZY = mParent->AddFixedPointInt16_8R8NumVarAngle(std::string("AngleZY"), startPosData + 0x2);
    AngleXZ = mParent->AddFixedPointInt16_8R8NumVarAngle(std::string("AngleXZ"), startPosData + 0x4);
    SpeedActual = mParent->AddFixedPointInt32_8R8NumVar(std::string("SpeedActual"), startPosData + 0x8);
}

std::string MovementClass::GetAsString() {
    std::ostringstream output;

    output << mName << ": " << AngleXY->GetAsString() << ", " << AngleZY->GetAsString() << ", " << AngleXZ->GetAsString() << ", " << SpeedActual->GetAsString();

    return output.str();
}

MovementClass::~MovementClass() {
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
    if (SpeedActual != nullptr) {
        delete SpeedActual;
        SpeedActual = nullptr;
    }
}
