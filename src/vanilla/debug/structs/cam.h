/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

#ifndef CAM_H
#define CAM_H

#include <vector>
#include <stdlib.h>
#include <string>
#include <cstdint>

class MemDump;
class UInt8_Num;
class UInt16_Num;
class UInt32_Num;
class Int8_Num;
class Int16_Num;
class Int32_Num;
class FixedPointInt32_8R8Num;
class FixedPointInt16_8R8NumAngle;
class DataTools;
class Coord3DClass;

class ParseCamera {
public:
    ParseCamera(MemDump* parentMemdump);
    ~ParseCamera();

    void Update(size_t fromAdr);
    void Print();

private:
    MemDump* mParentMemDump = nullptr;

public:
    Coord3DClass* Position = nullptr;
    Coord3DClass* Destination = nullptr;

    FixedPointInt16_8R8NumAngle* AngleXY = nullptr;
    FixedPointInt16_8R8NumAngle* AngleZY = nullptr;
    FixedPointInt16_8R8NumAngle* AngleXZ = nullptr;

    Int32_Num* Shading = nullptr;
    Int32_Num* Sky = nullptr;
    Int32_Num* SplittingLow = nullptr;
    Int32_Num* SplittingHigh = nullptr;
    Int32_Num* SolidPolygons = nullptr;

    FixedPointInt32_8R8Num* Lens = nullptr;
    FixedPointInt32_8R8Num* Distance = nullptr;

    Int16_Num* Follow = nullptr;
    Int16_Num* Zoom = nullptr;
    Int16_Num* Static = nullptr;
    Int16_Num* Action = nullptr;
};

#endif // CAM_H
