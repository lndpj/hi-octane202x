/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

#ifndef VVECTORS_H
#define VVECTORS_H

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
class FixedPointInt16_8R8Num;
class FixedPointInt16_8R8NumAngle;
class DataTools;
class Coord2DClass;
class Coord3DClass;
class MovementClass;

class ParseColVectClass {
public:
    ParseColVectClass(DataTools* parent, std::string name, size_t startPosData);
    ~ParseColVectClass();

    std::string GetAsString();

    Coord3DClass* Pos1 = nullptr;
    Coord3DClass* Pos2 = nullptr;
    FixedPointInt16_8R8NumAngle* Angle = nullptr;

private:
    DataTools* mParent = nullptr;
    std::string mName;
};

class ParseColVectsListClass {
public:
    ParseColVectsListClass(DataTools* parent, std::string name, size_t startPosData);
    ~ParseColVectsListClass();

    std::string GetAsString();

    UInt16_Num* Vect = nullptr;
    UInt16_Num* NextColList = nullptr;

private:
    DataTools* mParent = nullptr;
    std::string mName;
};

class ParseVectors {
public:
    ParseVectors(MemDump* parentMemdump);
    ~ParseVectors();

    ParseColVectClass* ColVects[250];
    ParseColVectsListClass* ColVectsList[10000];

    Coord2DClass* TrackCollisionVector = nullptr;
    UInt16_Num* NextColVect = nullptr;
    UInt16_Num* NextVectsList = nullptr;
    FixedPointInt16_8R8NumAngle* TrackCollisionVectorAngle = nullptr;

    void Update(size_t fromAdr);
    void Print();

private:
    MemDump* mParentMemDump = nullptr;

public:

};

#endif // VVECTORS_H
