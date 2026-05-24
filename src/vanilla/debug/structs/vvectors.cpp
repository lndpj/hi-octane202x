/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

#include "vvectors.h"
#include "../memdump.h"
#include "../datatools.h"
#include "basicstructs.h"
#include <iostream>
#include <sstream>

ParseColVectClass::ParseColVectClass(DataTools* parent, std::string name, size_t startPosData) {
    mParent = parent;
    mName = name;
    Pos1 = new Coord3DClass(mParent, std::string("Pos1"), startPosData);
    Pos2 = new Coord3DClass(mParent, std::string("Pos2"), startPosData + 0x6);
    Angle = mParent->AddFixedPointInt16_8R8NumVarAngle(std::string("Angle"), startPosData + 0xC);
}

std::string ParseColVectClass::GetAsString() {
    std::ostringstream output;

    output << "Pos1: " << Pos1->GetAsString() << ", Pos2: " << Pos2->GetAsString() << ", Angle: " << Angle->GetAsString();

    return output.str();
}

ParseColVectClass::~ParseColVectClass() {
    if (Pos1 != nullptr) {
        delete Pos1;
        Pos1 = nullptr;
    }
    if (Pos2 != nullptr) {
        delete Pos2;
        Pos2 = nullptr;
    }
    if (Angle != nullptr) {
        delete Angle;
        Angle = nullptr;
    }
}

ParseColVectsListClass::ParseColVectsListClass(DataTools* parent, std::string name, size_t startPosData) {
    mParent = parent;
    mName = name;
    Vect = mParent->AddUInt16_NumVar(std::string("Vect"), startPosData);
    NextColList = mParent->AddUInt16_NumVar(std::string("NextColList"), startPosData + 0x2);
}

std::string ParseColVectsListClass::GetAsString() {
    std::ostringstream output;

    output << "Vect: " << Vect->GetAsString() << ", NextColList: " << NextColList->GetAsString();

    return output.str();
}

ParseColVectsListClass::~ParseColVectsListClass() {
    if (Vect != nullptr) {
        delete Vect;
        Vect = nullptr;
    }
    if (NextColList != nullptr) {
        delete NextColList;
        NextColList = nullptr;
    }
}

ParseVectors::ParseVectors(MemDump* parentMemdump) {
   mParentMemDump = parentMemdump;

   for (size_t idx = 0; idx < 250; idx++) {
       this->ColVects[idx] = nullptr;
   }

   for (size_t idx = 0; idx < 10000; idx++) {
       this->ColVectsList[idx] = nullptr;
   }
}

void ParseVectors::Print() {
    std::cout << TrackCollisionVector->GetAsString() << std::endl;
    std::cout << NextColVect->GetAsString() << std::endl;
    std::cout << NextVectsList->GetAsString() << std::endl;
    std::cout << TrackCollisionVectorAngle->GetAsString() << std::endl;
}

void ParseVectors::Update(size_t fromAdr) {

    for (size_t idx = 0; idx < 250; idx++) {
        std::ostringstream output;

        output << "ColVect @" << std::dec << idx;

        this->ColVects[idx] = new ParseColVectClass(mParentMemDump->mDataTools, std::string(output.str()), fromAdr + 0x10 * idx);
    }

    for (size_t idx = 0; idx < 10000; idx++) {
        std::ostringstream output;

        output << "ColVectsList @" << std::dec << idx;

        this->ColVectsList[idx] = new ParseColVectsListClass(mParentMemDump->mDataTools, std::string(output.str()), fromAdr + 0xFA0 + 0x4 * idx);
    }

    TrackCollisionVector = new Coord2DClass(mParentMemDump->mDataTools, std::string("TrackCollisionVector"), fromAdr + 0xABE0);
    NextColVect = mParentMemDump->mDataTools->AddUInt16_NumVar(std::string("NextColVect"), fromAdr + 0xABE4);
    NextVectsList = mParentMemDump->mDataTools->AddUInt16_NumVar(std::string("NextVectsList"), fromAdr + 0xABE6);
    TrackCollisionVectorAngle = mParentMemDump->mDataTools->AddFixedPointInt16_8R8NumVarAngle(std::string("TrackCollisionVectorAngle"), fromAdr + 0xABE8);
}

ParseVectors::~ParseVectors() {
    for (size_t idx = 0; idx < 250; idx++) {
        if (this->ColVects[idx] != nullptr) {
            delete this->ColVects[idx];
            this->ColVects[idx] = nullptr;
        }
    }

    for (size_t idx = 0; idx < 10000; idx++) {
        if (this->ColVectsList[idx] != nullptr) {
            delete this->ColVectsList[idx];
            this->ColVectsList[idx] = nullptr;
        }
    }

    if (TrackCollisionVector != nullptr) {
        delete TrackCollisionVector;
        TrackCollisionVector = nullptr;
    }

    if (NextColVect != nullptr) {
        delete NextColVect;
        NextColVect = nullptr;
    }

    if (NextVectsList != nullptr) {
        delete NextVectsList;
        NextVectsList = nullptr;
    }

    if (TrackCollisionVectorAngle != nullptr) {
        delete TrackCollisionVectorAngle;
        TrackCollisionVectorAngle = nullptr;
    }
}
