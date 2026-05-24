/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

#include "binaryfile.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "../../utils/logging.h"

BinaryFile::BinaryFile(std::string inputFile)
{
    //read data from binary file
    std::ifstream ifile;
    std::streampos fileSize;

    ifile.open(inputFile, std::ifstream::binary);
    if(ifile) {
        std::string infoMsg("Input file ");
        infoMsg.append(inputFile);
        infoMsg.append(" found and opened succesfully");
        logging::Info(infoMsg);
    } else {
        std::string infoMsg("Could not open input file ");
        infoMsg.append(inputFile);
        logging::Error(infoMsg);
        return;
    }

    // get its size:
    ifile.seekg(0, std::ios::end);
    fileSize = ifile.tellg();
    mLength = ifile.tellg();
    ifile.seekg(0, std::ios::beg);

    mData = new std::vector<uint8_t>();

    //read the data
    this->mData->resize(fileSize);
    ifile.read(reinterpret_cast<char*>(this->mData->data()), this->mData->size());

    ifile.close();
}

BinaryFile::~BinaryFile() {
    delete mData;
}

std::vector<uint8_t> BinaryFile::GetDataRange(size_t startOff, size_t endOff) {
    std::vector<uint8_t> resultData;
    resultData.clear();

    if (mData == nullptr) {
        return resultData;
    }

    if (startOff > mData->size()) {
        startOff = mData->size();
    }

    if (endOff > mData->size()) {
        endOff = mData->size();
    }

    std::vector<uint8_t>::iterator itStart =
            this->mData->begin() + startOff;

    std::vector<uint8_t>::iterator itEnd =
            this->mData->begin() + endOff + 1;

    std::vector<uint8_t>::iterator it;

    for (it = itStart; (it != itEnd) && (it != mData->end()); ++it) {
        resultData.push_back((*it));
    }

    return resultData;
}


