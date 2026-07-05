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

#ifndef VVEHICLE_H
#define VVEHICLE_H

#include <irrlicht.h>
#include <cstdint>
#include <vector>
#include <string>
#include "../vanilla/vcalc.h"
#include "../utils/path.h"
#include "SFML/Audio.hpp"

//This are player states I defined myself
//are not defined in the original game!
#define STATE_PLAYER_BEFORESTART 0
#define STATE_PLAYER_ONFIRSTWAYTOFINISHLINE 1
#define STATE_PLAYER_RACING 2
#define STATE_PLAYER_EMPTYFUEL 3
#define STATE_PLAYER_BROKEN 4
#define STATE_PLAYER_GRABEDBYRECOVERYVEHICLE 5

/************************
 * Forward declarations *
 ************************/

class Collectable;
class HUD;

struct VehicleSensorPointStruct {
    irr::core::vector3df Position;
    int8_t CollideFlags;
    irr::f32 Zpos;
    irr::f32 ZposFloor;
    irr::f32 ZposDiff;
    //Important: Never remove initial value of 0.0f for
    //ZposDisplacement below, otherwise the craft will
    //show random/undefined behavior at the physics model start
    irr::f32 ZposDisplacement = 0.0f;
    irr::f32 Rebound;
    irr::f32 ReboundLimit;
};

struct VehicleControlFlagsStruct {
    bool Booster;
    bool Brake;
    bool Airbourn;
    bool BarrelRoll;
    bool AutoPilot;
    bool AutoRefuel;
    bool AutoRearm;
    bool AutoRepair;
    bool AutoStop;
    bool AutoPilotSet;
    bool AutoDrive;
    bool HealthDeath;
    bool FuelDeath;
    bool Reposition;
    //pad1 and pad2 seems to be used by
    //autopilot
    bool pad1;
    bool pad2;
};

struct VehicleFunctionFlagsStruct {
    bool Booster;
    bool Brake;
    bool BarrelRoll;
    bool Pad4;  //seems to be used for computer player control
    bool Pad6;  //seems to be used during collision detection with vector collision
    bool Pad9;  //seems to be used for computer player control
};

struct VehicleCollideControlStruct {
    irr::f32 SizeForward;
    irr::f32 SizeRear;
    irr::f32 SizeSideways;
    irr::f32 SizeSensorOffset;
    VehicleSensorPointStruct FrontLeft;
    VehicleSensorPointStruct FrontRight;
    VehicleSensorPointStruct RearLeft;
    VehicleSensorPointStruct RearRight;
    VehicleControlFlagsStruct Flag;
    VehicleFunctionFlagsStruct FunctionFlag;
    irr::f32 RideHeight;
    irr::f32 BrakePower;
};

struct VehicleStatsStruct {
    int16_t Behind;
    int16_t Fuel;
    int16_t Weapons;

    int16_t Health;
    irr::f32 Velocity;

    //TODO: Move to the MGun and Rocket
    //weapon structs later
    int16_t MGunUpgrade;
    int16_t MRocketUpgrade;

    //player names in Hi-Octane are limited
    //to 8 characters, plus 1 termination char + 1 extra
    //char to be on the safe side :)
    char name[10];
};

struct VehicleViewStruct {
    irr::core::vector3df Position;
    irr::f32 AngleXY;
    irr::f32 AngleZY;
    irr::f32 AngleXZ;
};

struct VehicleBoosterStruct {
    irr::f32 InitialThrust;
    irr::f32 BurnThrust;
    int16_t BurnSetting;
    uint8_t BurnTime;
    uint8_t Burn;

    //variables moved here
    //from Thing (ThingWeapon)
    int16_t Trigger = 0;
    int16_t TriggerTime = 0;
    int16_t TriggerRestrictionCount = 0;
    int16_t Target = 0;

    //variables moved here
    //from Thing
    int16_t Upgrade = 0;
};

//all this values seem to be reset to 0
//inside this struct
//In the original game this struct is located
//in the Control related struct, but I moved it
//to vehicle
struct VehicleConditionsStruct {
    int32_t FuelUsed = 0;
    int32_t WeaponsUsed = 0;

    //not sure if the variables below
    //are needed for my purpose
 /*   int8_t FuelLowCounter = 0;
    int8_t FuelRechargeCounter = 0;
    int8_t FuelFullCounter = 0;
    int8_t WeaponsLowCounter = 0;
    int8_t WeaponsRechargeCounter = 0;
    int8_t WeaponsFullCounter = 0;
    int8_t HealthLowCounter = 0;
    int8_t HealthRechargeCounter = 0;
    int8_t HealthFullCounter = 0;*/

    int8_t FuelLowCounter = 0;
    int8_t WeaponsLowCounter = 0;
    int8_t HealthLowCounter = 0;
};

struct VehicleComputerPlayerStruct {
    int16_t EnemyIndex;
    int16_t Count1;
    int16_t Count2;
    int16_t Count3;
    uint8_t Skill;
    uint8_t Param2;
    uint8_t Param3;
    uint8_t Param4;
};

/************************
 * Forward declarations *
 ************************/

class Race;
struct MapTileRegionStruct;

class VVehicle {
public:
    VVehicle(Race* mParentRace, std::string model, irr::core::vector3d<irr::f32> NewPosition,
             irr::core::vector3d<irr::f32> NewFrontAt, irr::u8 nrLaps, bool humanPlayer);
    ~VVehicle();

    void Update(irr::f32 frameDeltaTime);

    void DrawDebug();
    void TestCamera();

    bool KeyPressedTurnLeft = false;
    bool KeyPressedTurnRight = false;
    bool KeyPressedAccel = false;
    bool KeyPressedDeaccel = false;
    bool KeyPressedBooster = false;

    //Thing data
    ThingDataStruct ThingData;

private:
    Race* mRace = nullptr;

public:
    MomentumStruct Momentum;

    //for player input
    MovementStruct MovementInput;

    MovementStruct Increment;

    //flight model constants
    MovementStruct IncrementAdd;
    MovementStruct IncrementSub;
    MovementStruct IncrementLimit;

    VehicleCollideControlStruct FlightModel;
    irr::core::vector3df Displacement;
    irr::core::vector3df Slope;
    irr::core::vector3df Bump;

    //Stats
    VehicleStatsStruct Stats;

    int16_t mThrustEffectiveness;
    irr::f32 mSideslipFriction = 0.0f;
    irr::f32 mSideslipToThrust = 0.0f;

    //BumpDamage was not used at the end
    //at last in the Playstation version of the
    //game
    irr::f32 mBumpDamage = 0.0f;

    //flight model parameters
    irr::f32 mFriction = 0.0f;
    irr::f32 mFrictionLimit = 0.0f;
    irr::f32 mBounce = 0.0f;
    irr::f32 mMaximumZpos = 0.0f;

    VehicleBoosterStruct Booster;
    VehicleConditionsStruct Conditions;
    VehicleViewStruct View;

    irr::f32 mDeltaTimeFactor = 1.0f;

    uint16_t dbgTrackCurrWayPoint = 0;

    irr::f32 mDbgColl;

    irr::scene::IMeshSceneNode* mCraftNode = nullptr;

    irr::core::aabbox3d<irr::f32> mBoundingBox;

    //Returns true if vehicle is controlled by
    //computer player
    bool IsControlledByComputer();

    bool ShouldAmmoBarBlink();
    bool ShouldFuelBarBlink();
    bool ShouldShieldBarBlink();

    void SetName(char* playerName);

    //is called when the player collected a collectable item of the
    //level
    bool CollectedCollectable(Collectable* whichCollectable);

    void SetCurrClosestWayPointLink(std::pair <WayPointLinkInfoStruct*, irr::core::vector3df> newClosestWayPointLink);

    std::pair <WayPointLinkInfoStruct*, irr::core::vector3df> currClosestWayPointLink;
    std::vector< std::pair <WayPointLinkInfoStruct*, irr::core::vector3df> > currCloseWayPointLinks;

    //the following variable is only valid (and must only be used)
    //while currClosestWayPointLink is current != nullptr
    //Important note: This 3D coordinate is saved in the Irrlicht coordinate
    //system
    irr::core::vector3df projPlayerPositionClosestWayPointLink;

    //next checkpoints value we need to reach for
    //correct race progress
    irr::s32 nextCheckPointValue = 0;

    //stores the last crossed checkpoints; we need this
    //to differentiate first crossing of finish line at the start of the race
    //with the end of following laps
    irr::s32 lastCrossedCheckPointValue = 0;

    //the following variable is used for race position calculation
    irr::f32 remainingDistanceToNextCheckPoint = 0.0f;

    irr::u8 currRacePlayerPos = 0;
    irr::u8 overallPlayerNumber = 0;

    irr::u8 currLapNumber = 1;
    irr::u8 raceNumberLaps = 6;

    bool mHasFinishedRace = false;

    void CrossedCheckPoint(irr::s32 valueCrossedCheckPoint, irr::s32 numberOfCheckpoints);

    //Important note: This 3D Vector is stored in the Irrlicht
    //coordinate system!
    irr::core::vector3df IrrWorldDirVecForward;

    //pointer to a connected HUD
    //nullptr if player is currently
    //not linked to any HUD
    HUD* mHUD = nullptr;

    void SetMyHUD(HUD* pntrHUD);
    HUD* GetMyHUD();

    void SetNewState(irr::u32 newPlayerState);
    irr::u32 GetCurrentState();

    void StartPlayingWarningSound();
    void StopPlayingWarningSound();

private:
    uint32_t ControlOrigin = 1; //activates the human player
    uint16_t LastWayPoint = 0;
    uint16_t CurrentWaypoint = 0;

    irr::u8 mPlayerCurrentState;

    //variables which only I use in my project
    irr::f32 mAbsTimeIntegrator = 0.0f;

    irr::f32 mUpdateVehicleTimeIntegrator = 0.0f;

    //the mesh for the Irrlicht SceneNode model
    irr::scene::IAnimatedMesh* mCraftMesh = nullptr;

    void SetupFlightModelConstants();

    VehicleComputerPlayerStruct ComputerPlayer;
    void vehicle_setup_computer_character();

    void vehicle_control();
    void vehicle_control_from_player();
    uint8_t vehicle_control_from_autopilot();
    uint8_t vehicle_set_autopilot_on();
    uint8_t vehicle_set_autopilot_off();

    void vehicle_get_track_friction();
    void vehicle_calculate_angle();
    void vehicle_calculate_thrust(irr::core::vector3df& delta);
    void vehicle_calculate_momentum(irr::core::vector3df& delta);
    void vehicle_calculate_movement_delta(irr::core::vector3df& delta);
    void vehicle_move_altitude(irr::core::vector3df& delta);
    void vehicle_move_tilt(irr::core::vector3df& delta);
    void vehicle_move_roll(irr::core::vector3df& delta);
    void vehicle_sensor_point_projection(irr::core::vector3df& delta);
    void vehicle_sensor_point_process(VehicleSensorPointStruct& sensor, irr::core::vector3df& slope, int8_t terrain);
    void vehicle_colide_map(irr::core::vector3df& delta);
    void vehicle_colide_vectors(irr::core::vector3df& delta);
    void vehicle_terrain_effect(irr::core::vector3df delta);
    uint8_t vehicle_colide(std::vector<VVehicle*> &vehicleVec, irr::core::vector3df& delta);
    uint8_t vehicle_colide_final_check_sean(std::vector<VVehicle*> &vehicleVec, irr::core::vector3df& delta);
    void vehicle_move_mapwho(irr::core::vector3df& delta);
    void vehicle_set_camera();
    void vehicle_post_process();

    bool OrientedBBoxCollision(VVehicle* vehicle1, VVehicle* vehicle2,
                                        irr::core::vector3df& collNormal, irr::f32& depth);

    uint8_t vehicle_colide_my_attempt(std::vector<VVehicle*> &vehicleVec, irr::core::vector3df& delta);
    bool VehiclesCheckForCollision(VVehicle* vehicle1, VVehicle* vehicle2,
               irr::core::vector3df& collNormal, irr::f32& depth);

    //Below are my functions and Members I need
    //a local coordinate system point defined on the players craft
    irr::core::vector3d<irr::f32> IrrLocalCraftFrontPnt;
    irr::core::vector3d<irr::f32> IrrLocalCraftBackPnt;
    irr::core::vector3d<irr::f32> IrrLocalCraftLeftPnt;
    irr::core::vector3d<irr::f32> IrrLocalCraftRightPnt;
    irr::core::vector3d<irr::f32> IrrLocalCraftOrigin;
    irr::core::vector3d<irr::f32> IrrLocalCraftTriggerSensor;

    irr::core::vector3d<irr::f32> IrrWorldCraftFrontPnt;
    irr::core::vector3d<irr::f32> IrrWorldCraftBackPnt;
    irr::core::vector3d<irr::f32> IrrWorldCraftLeftPnt;
    irr::core::vector3d<irr::f32> IrrWorldCraftRightPnt;
    irr::core::vector3d<irr::f32> IrrWorldCraftOrigin;
    irr::core::vector3d<irr::f32> IrrWorldCraftTriggerSensor;

    sf::Sound* mWarningSoundSource = nullptr;

    void CalcCraftLocalFeatureCoordinates(irr::core::vector3d<irr::f32> NewPosition, irr::core::vector3d<irr::f32> NewFrontAt);

    void CheckForTriggerCraftRegion();
    void CheckForChargingStation();

    //is unequal to NULL if player craft is currently inside
    //a craft trigger area defined in the level during
    //the current player update
    MapTileRegionStruct* mCurrentCraftTriggerRegion = nullptr;

    //is unequal to NULL if player craft was inside
    //a craft trigger area defined in the level during
    //the last player update
    MapTileRegionStruct* mLastCraftTriggerRegion = nullptr;

    void UpdateHUDState();

    //Player states I defined myself
    //TODO 04.07.2026: This internal variables
    //have no effect right now on the vehicle
    //Either map to other variable
    //or use them somewhere
    bool mPlayerCanMove = false;
    bool mPlayerCanShoot = false;

    //variables to remember if during the last
    //gameloop this player did any charging
    bool mLastChargingFuel = false;
    bool mLastChargingShield = false;
    bool mLastChargingAmmo = false;

    //variables to know if we do currently
    //charging
    bool mCurrChargingFuel = false;
    bool mCurrChargingShield = false;
    bool mCurrChargingAmmo = false;

    bool mPlayerCurrentlyCharging = false;
    sf::Sound* mChargingSoundSource = nullptr;

    //variables to remember if certain banner texts
    //were already shown to the player once, to prevent
    //to trigger them continiously
    bool mLowFuelWarningAlreadyShown = false;
    bool mEmptyFuelWarningAlreadyShown = false;
    bool mLowAmmoWarningAlreadyShown = false;
    bool mLowShieldWarningAlreadyShown = false;

    bool mBlockAdditionalFuelFullMsg = true;
    bool mBlockAdditionalShieldFullMsg = true;
    bool mBlockAdditionalAmmoFullMsg = true;

    void FinishedLap();
    void FinishedRace();

    void UpdateCoordinates();

    void UpdateSceneNode();

    void processWeaponBooster();

    void ModelRotate(irr::scene::ISceneNode *node, irr::core::vector3df rot);
    void ModelYaw(irr::scene::ISceneNode *node, irr::f32 rot);
    void ModelPitch(irr::scene::ISceneNode *node, irr::f32 rot);
    void ModelRoll(irr::scene::ISceneNode *node, irr::f32 rot);
};

#endif // VVEHICLE_H

