/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

#ifndef THINGLIST_H
#define THINGLIST_H

#include <vector>
#include <stdlib.h>
#include <string>
#include <cstdint>

class MemDump;

class ParseThingList {
public:
    ParseThingList(MemDump* parentMemdump);
    ~ParseThingList();

    void Read(size_t dumpLevelStructStart);
    void Print();

    int32_t mNrThingsExisting = 0;

private:
    MemDump* mParentMemDump = nullptr;

    int16_t Thing[1000];
    int32_t NextIndex;
};

#endif // THINGLIST_H
