/*
 Copyright (C) 2026 Wolf Alexander

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.                                          */

//This source code was implemented by me based on the insight I gained into the original game. Some parts do more closely follow
//the Pseudo-C Code that is available to me, and other parts are heavily modified to hopefully work inside my existing project
//in the near future.
//The original game uses fixed point arithmetic for performance reasons (it had to run on 80486 after all), and is optimized everywhere you
//look at. I do not want to do the same, because the fixed point arithmetic makes it very difficult to read and understand. And performance should not
//be the problem nowadays. Therefore the biggest change which I have to do is to change all the logic to floating point arithmetic,
//while hopefully not breaking the concept behind everything.
//Additionally I will only try to reimplement the most important mechanisms and concepts from the original game where it benefits the overall project,
//but I will not change the overall project to use the same structs and data structures as the original. This would not work, and also does not
//make sense in my opinion.

//Important note: What makes this source code very difficult to handle is the fact, that my coordinate system in this existing project is
//completely different to the one in the original game. The original uses X and Y axis for the tile map, and Z is the height.
//For the levelfile and 2D map stuff I also use X any Y axis for the tile map most of the time.
//My 3D world setup (for rendering) using Irrlicht has X and Z for the tile map, and Y is the height. And to make things worse my Irrlicht vertice X and Y coordinates
//have a swapped sign (are negative) currently.

//I have decided to also use the original games coordinate system in all vanilla calculations. At the interface between
//original game calculations and Irrlicht 3D coordinate system I have then to convert from one coordinate system setup to the other.
//Thats the drawback I will have.

//I really want to thank aybe for giving me the opportunity to look much deeper into the original game inner workings as I was ever able before.
//Without this support I would not have been able to hopefully advance the current project more true to the original.

#include "vtrack.h"
#include "../race.h"
#include "../resources/levelfile.h"
#include "../models/levelterrain.h"
#include "../resources/mapentry.h"
#include "../vanilla/vcalc.h"

VTrack::VTrack(Race* parentRace) {
    mParentRace = parentRace;

    NextColVect = 1;
    NextVectsList = 1;
}

void VTrack::add_collision_to_single_mapwho(irr::f32 x, irr::f32 y) {
    int16_t nextVectsList = this->NextVectsList;
    int16_t nextColVect;
    int16_t v6;

    int cellX = (int)(x / mParentRace->mLevelTerrain->segmentSize);
    int cellY = (int)(y / mParentRace->mLevelTerrain->segmentSize);

    //get pntr to this tile
    MapEntry *pntr = mParentRace->mLevelTerrain->levelRes->pMap[cellX][cellY];

    if ((pntr != nullptr) && ((nextVectsList + 1) < 10000)) {
        nextColVect = this->NextColVect;
        if (ColVectsList[pntr->mVector].Vect != nextColVect) {
            ColVectsList[nextVectsList].Vect = nextColVect;
            ColVectsList[this->NextVectsList].NextColList = pntr->mVector;
          /*  if (pntr->mVector != 0) {
                std::cout << "Val = " << std::hex << pntr->mVector << std::endl;
            }*/
            v6 = this->NextVectsList;
            this->NextVectsList = v6 + 1;
            pntr->mVector = v6;
            //std::cout << "X: " << std::dec << cellX << "Y: " << std::dec << cellY << std::endl;
        }
    }
}

void VTrack::insert_vect(irr::core::vector3df position1, irr::core::vector3df position2) {
   //better use fixed point arithmetic here
   int32_t pos1X = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(position1.X));
   int32_t pos1Y = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(position1.Y));
   int32_t pos1Z = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(position1.Z));

   int32_t pos2X = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(position2.X));
   int32_t pos2Y = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(position2.Y));
   int32_t pos2Z = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(position2.Z));

   int32_t diffX = (pos2X - pos1X);
   int32_t diffY = (pos2Y - pos1Y);

   int32_t v23;
   int32_t v25;
   int32_t v26;
   int32_t v22;
   int32_t v19;
   int32_t v24;
   int32_t v20;
   int32_t v21;
   int32_t v27;
   int32_t v28;
   int32_t v29;
   int32_t v30;

   if (diffX < 0) {
       diffX = (pos1X - pos2X);
   }

   if (diffY < 0) {
       diffY = (pos1Y - pos2Y);
   }

   if ((diffX <= 0x7FFF) && (diffY <= 0x7FFF) &&
           ((this->NextColVect + 1) < 250) && ((this->NextVectsList + 1) < 10000)) {
        this->ColVects[this->NextColVect].pos1X = pos1X;
        this->ColVects[this->NextColVect].pos1Y = pos1Y;
        this->ColVects[this->NextColVect].pos1Z = pos1Z;
        this->ColVects[this->NextColVect].pos2X = pos2X;
        this->ColVects[this->NextColVect].pos2Y = pos2Y;
        this->ColVects[this->NextColVect].pos2Z = pos2Z;
        this->ColVects[this->NextColVect].Angle =
                mParentRace->mVCalc->angle_get_xy(position1, position2);

        int32_t v15 = (pos2X - pos1X) << 9;
        int32_t v16 = abs(v15);
        int32_t v17 = (pos2Y - pos1Y) << 9;
        int32_t v18 = abs(v17);

        if (v16 < v18) {
           v23 = 0;

           if (v18) {
               v23 = (v17 / v18) << 8;
           }

           v25 = v18;
           v26 = abs(v23);
           v22 = v25 / v26;

           if (!(v25 / v26)) {
              // break;
              std::cout << "break!" << std::endl;
           }

           v19 = v15 / v22;
           v24 = pos1X;
        } else {
           v19 = 0;

           if (v16) {
               v19 = (v15 / v16) << 8;
           }

           v20 = v16;
           v21 = abs(v19);
           v22 = v20 / v21;

           if (!(v20 / v21)) {
             //  break;
             std::cout << "break!" << std::endl;
           }

           v23 = v17 / v22;
           v24 = pos1X;
        }

        v27 = v24 << 9;
        v28 = pos1Y << 9;

        if (v19 == v23) {
            v28 += 512;
        }

        irr::f32 floatXPos;
        irr::f32 floatYPos;

        while (v22 >= 0) {
            v29 = v27 / 512;
            v30 = v28 / 512;
            --v22;
            v27 += v19;
            v28 += v23;

            floatXPos = mParentRace->mVCalc->FixedPointToFloat8D8(static_cast<int16_t>(v29));
            floatYPos = mParentRace->mVCalc->FixedPointToFloat8D8(static_cast<int16_t>(v30));

            add_collision_to_single_mapwho(floatXPos, floatYPos);
        }
        ++this->NextColVect;
   }
}

uint8_t VTrack::do_move_colide(irr::f32 x1Float, irr::f32 y1Float, irr::f32 x2Float, irr::f32 y2Float, MapEntry* me)
{
    uint8_t v7;
    int16_t v6;
    int32_t v8;
    int32_t v9;
    int32_t v10;
    int32_t v13;
    int32_t v15;
    int32_t v16;
    int32_t v17;
    int32_t v19;
    int32_t v20;
    int32_t v21;
    int32_t v22;
    int32_t v23;
    int32_t v24;
    int32_t v25;
    int32_t v26;
    int32_t v27;
    int32_t v28;
    int32_t v29;

    int32_t Xpos;
    int32_t Ypos;

    bool v18;
    bool v30;

    TrackColVectStruct* v11;

    //better do this in fixed point arithmetic
    int32_t x1 = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(x1Float));
    int32_t y1 = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(y1Float));

    int32_t x2 = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(x2Float));
    int32_t y2 = static_cast<int32_t>(mParentRace->mVCalc->FloatToFixedPoint8D8(y2Float));

    int16_t Vector = me->mVector;
    v6 = Vector;
    v7 = 0;

    if (me->mVector) {
        v8 = x1;
        v9 = x2;
        v10 = x1 - x2;
        do {
            v11 = &ColVects[ColVectsList[v6].Vect];
            Xpos = ColVects[ColVectsList[v6].Vect].pos2X;
            v13 = v11->pos1X;
            Ypos = ColVects[ColVectsList[v6].Vect].pos1Y;
            v15 = ColVects[ColVectsList[v6].Vect].pos2Y;
            v16 = Xpos - v13;
            v17 = v13;
            if ((Xpos - v13) < 0) {
                v17 = ColVects[ColVectsList[v6].Vect].pos2X;
                Xpos = v11->pos1X;
            }
            if (v10 <= 0) {
               v18 = Xpos < v8;
               v19 = 0;
               if (v18) {
                   goto do_move_colide_LABEL36;
               }
               v19 = 0;
               if (v9 < v17) {
                   goto do_move_colide_LABEL36;
               }
               v20 = v15 - Ypos;
            } else {
               v18 = Xpos < v9;
               v19 = 0;
               if (v18) {
                   goto do_move_colide_LABEL36;
               }
               v20 = v15 - Ypos;
               if (v8 < v17) {
                   v19 = 0;
                   goto do_move_colide_LABEL36;
               }
            }

            v21 = y1 - y2;
            if (v20 >= 0) {
                v23 = v15;
                v22 = Ypos;
            } else {
                v22 = v15;
                v23 = Ypos;
            }

            if (v21 <= 0) {
                v18 = v23 < y1;
                v19 = 0;
                if (v18) {
                   goto do_move_colide_LABEL36;
                }
                v19 = 0;
                if (y2 < v22) {
                   goto do_move_colide_LABEL36;
                }
                v24 = v13 - v8;
            } else {
                v18 = v23 < y2;
                v19 = 0;
                if (v18) {
                   goto do_move_colide_LABEL36;
                }
                v24 = v13 - v8;
                if (y1 < v22) {
                    v19 = 0;
                    goto do_move_colide_LABEL36;
                }
            }

            v25 = Ypos - y1;
            v26 = v21 * v24 - v10 * v25;
            v27 = v20 * v10 - v16 * v21;
            if (v27 <= 0) {
                v19 = 0;
                if (v26 <= 0) {
                    v28 = v16 * v25;
                    if (v26 >= v27) {
do_move_colider_LABEL28:
                        v29 = v28 - v20 * v24;
                        if (v27 <= 0) {
                           v18 = v29 > 0;
                           v30 = v29 < v27;
                           if (!v18) {
do_move_colider_LABEL32:
                               if (!v30) {
                                 v19 = 2;
                                 if (!v27) {
                                     v19 = 1;
                                 }
                                 goto do_move_colide_LABEL36;
                               }
                           }
                        } else {
                            v18 = v29 < 0;
                            v30 = v27 < v29;
                            if (!v18) {
                                goto do_move_colider_LABEL32;
                            }
                        }
                    }
do_move_colider_LABEL33:
                        v19 = 0;
                    }
                } else {
                  if (v26 < 0) {
                      goto do_move_colider_LABEL33;
                  }
                  v28 = v16 * v25;
                  if (v27 >= v26) {
                      goto do_move_colider_LABEL28;
                  }
                  v19 = 0;
                }
do_move_colide_LABEL36:
                if (v19) {
                   TrackCollisionVectorX = v11->pos2X - v11->pos1X;
                   TrackCollisionVectorY = v11->pos2Y - v11->pos1Y;
                   v7 = 1;
                   TrackCollisionVectorAngle = v11->Angle;
                }
                Vector = ColVectsList[Vector].NextColList;
                v6 = Vector;
            } while (Vector && !v7);
        }

        return v7;
}

uint16_t VTrack::track_vector_collide(irr::core::vector3df position1, irr::core::vector3df position2) {
    int v5;
    int v8;

    int cell1X = (int)(position1.X / mParentRace->mLevelTerrain->segmentSize);
    int cell1Y = (int)(position1.Y / mParentRace->mLevelTerrain->segmentSize);

    int cell2X = (int)(position2.X / mParentRace->mLevelTerrain->segmentSize);
    int cell2Y = (int)(position2.Y / mParentRace->mLevelTerrain->segmentSize);

    v5 = cell2X - cell1X;
    v8 = cell2Y - cell1Y;

    MapEntry *me1 = mParentRace->mLevelTerrain->levelRes->pMap[cell1X][cell1Y];
    MapEntry *me2 = mParentRace->mLevelTerrain->levelRes->pMap[cell1X][cell2Y];
    MapEntry *me3 = mParentRace->mLevelTerrain->levelRes->pMap[cell1Y + v8][cell1X + v5];

    uint8_t v9 = do_move_colide(position1.X, position1.Y, position2.X, position2.Y, me1);
    if (v8 && !v9) {
        v9 = do_move_colide(position1.X, position1.Y, position2.X, position2.Y, me2);
    }
    if (v5 && !v9) {
        v9 = do_move_colide(position1.X, position1.Y, position2.X, position2.Y, me3);
    }

    return v9;
}

VTrack::~VTrack() {
}

