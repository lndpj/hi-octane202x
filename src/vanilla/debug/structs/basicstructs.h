/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

#ifndef BASICSTRUCTS_H
#define BASICSTRUCTS_H

#include <vector>
#include <stdlib.h>
#include <string>
#include <cstdint>

class MemDump;
class UInt8_Num;
class UInt16_Num;
class Int16_Num;
class Int32_Num;
class FixedPointInt16_8R8Num;
class FixedPointInt32_16R16Num;
class FixedPointInt32_8R8Num;
class FixedPointInt16_8R8NumAngle;
class FixedPointInt32_16R16NumAngle;
class DataTools;

class Coord3DClass {
public:
    Coord3DClass(DataTools* parent, std::string name, size_t startPosData);
    ~Coord3DClass();

    std::string GetAsString();

    FixedPointInt16_8R8Num* XPos = nullptr;
    FixedPointInt16_8R8Num* YPos = nullptr;
    FixedPointInt16_8R8Num* ZPos = nullptr;

private:
    DataTools* mParent = nullptr;
    std::string mName;
};

class MovementClass {
public:
    MovementClass(DataTools* parent, std::string name, size_t startPosData);
    ~MovementClass();

    std::string GetAsString();

    FixedPointInt16_8R8NumAngle* AngleXY = nullptr;
    FixedPointInt16_8R8NumAngle* AngleZY = nullptr;
    FixedPointInt16_8R8NumAngle* AngleXZ = nullptr;
    FixedPointInt32_8R8Num* SpeedActual = nullptr;

private:
    DataTools* mParent = nullptr;
    std::string mName;
};

#endif // BASICSTRUCTS_H
