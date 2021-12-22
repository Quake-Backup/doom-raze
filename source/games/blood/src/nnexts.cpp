//-------------------------------------------------------------------------
/*
Copyright (C) 2010-2019 EDuke32 developers and contributors
Copyright (C) 2019 Nuke.YKT
Copyright (C) NoOne

This file is part of NBlood.

NBlood is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
//-------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////
// This file provides modern features for mappers.
// For full documentation please visit http://cruo.bloodgame.ru/xxsystem
///////////////////////////////////////////////////////////////////
#include "ns.h"

#ifdef NOONE_EXTENSIONS
#include <random>
#include "blood.h"
#include "savegamehelp.h"

BEGIN_BLD_NS

inline int mulscale8(int a, int b) { return MulScale(a, b, 8); }

bool gAllowTrueRandom = false;
bool gEventRedirectsUsed = false;




short gEffectGenCallbacks[] = {
    
    kCallbackFXFlameLick,
    kCallbackFXFlareSpark,
    kCallbackFXFlareSparkLite,
    kCallbackFXZombieSpurt,
    kCallbackFXBloodSpurt,
    kCallbackFXArcSpark,
    kCallbackFXTeslaAlt,

};


TRPLAYERCTRL gPlayerCtrl[kMaxPlayers];

TRCONDITION gCondition[kMaxTrackingConditions];
int gTrackingCondsCount;

std::default_random_engine gStdRandom;

const VECTORINFO_EXTRA gVectorInfoExtra[] = {
    1207,1207,      1001,1001,      4001,4002,
    431,431,        1002,1002,      359,359,
    521,521,        513,513,        499,499,
    9012,9014,      1101,1101,      1207,1207,
    499,495,        495,496,        9013,499,
    1307,1308,      499,499,        499,499,
    499,499,        499,499,        351,351,
    0,0,            357,499
};

const MISSILEINFO_EXTRA gMissileInfoExtra[] = {
    1207, 1207,    false, false, false, false, false, true, false,     true,
    420, 420,      false, true, true, false, false, false, false,      true,
    471, 471,      false, false, false, false, false, false, true,     false,
    421, 421,      false, true, false, true, false, false, false,      false,
    1309, 351,     false, true, false, false, false, false, false,     true,
    480, 480,      false, true, false, true, false, false, false,      false,
    470, 470,      false, false, false, false, false, false, true,     true,
    489, 490,      false, false, false, false, false, true, false,     true,
    462, 351,      false, true, false, false, false, false, false,     true,
    1203, 172,     false, false, true, false, false, false, false,     true,
    0,0,           false, false, true, false, false, false, false,     true,
    1457, 249,     false, false, false, false, false, true, false,     true,
    480, 489,      false, true, false, true, false, false, false,      false,
    480, 489,      false, false, false, true, false, false, false,     false,
    480, 489,      false, false, false, true, false, false, false,     false,
    491, 491,      true, true, true, true, true, true, true,           true,
    520, 520,      false, false, false, false, false, true, false,     true,
    520, 520,      false, false, false, false, false, true, false,     true,
};

const THINGINFO_EXTRA gThingInfoExtra[] = {
    true,   true,   true,   false,  false,
    false,  false,  false,  false,  false,
    false,  false,  false,  false,  false,
    true,   false,  false,  true,   true,
    true,   true,   false,  false,  false,
    false,  false,  true,   true,   true,
    true,   true,   true,   true,   true,
    true,
};

const DUDEINFO_EXTRA gDudeInfoExtra[] = {
    
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 200
    { false,  false,  0, 9, 13, 13, 17, 14 },       // 201
    { false,  false,  0, 9, 13, 13, 17, 14 },       // 202
    { false,  true,   0, 8, 0, 8, -1, -1 },         // 203
    { false,  false,  0, 8, 0, 8, -1, -1 },         // 204
    { false,  true,   1, -1, -1, -1, -1, -1 },      // 205
    { true,   true,   0, 0, 0, 0, -1, -1 },         // 206
    { true,   false,  0, 0, 0, 0, -1, -1 },         // 207
    { true,   false,  1, -1, -1, -1, -1, -1 },      // 208
    { true,   false,  1, -1, -1, -1, -1, -1 },      // 209
    { true,   true,   0, 0, 0, 0, -1, -1 },         // 210
    { false,  true,   0, 8, 0, 8, -1, -1 },         // 211
    { false,  true,   0, 6, 0, 6, -1, -1 },         // 212
    { false,  true,   0, 7, 0, 7, -1, -1 },         // 213
    { false,  true,   0, 7, 0, 7, -1, -1 },         // 214
    { false,  true,   0, 7, 0, 7, -1, -1 },         // 215
    { false,  true,   0, 7, 0, 7, -1, -1 },         // 216
    { false,  true,   0, 9, 10, 10, -1, -1 },       // 217
    { false,  true,   0, 0, 0, 0, -1, -1 },         // 218
    { true,  false,   7, 7, 7, 7, -1, -1 },         // 219
    { false,  true,   0, 7, 0, 7, -1, -1 },         // 220
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 221
    { false,  true,   -1, -1, -1, -1, -1, -1 },     // 222
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 223
    { false,  true,   -1, -1, -1, -1, -1, -1 },     // 224
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 225
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 226
    { false,  false,  0, 7, 0, 7, -1, -1 },         // 227
    { false,  false,  0, 7, 0, 7, -1, -1 },         // 228
    { false,  false,  0, 8, 0, 8, -1, -1 },         // 229
    { false,  false,  0, 9, 13, 13, 17, 14 },       // 230
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 231
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 232
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 233
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 234
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 235
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 236
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 237
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 238
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 239
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 240
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 241
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 242
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 243
    { false,  true,   -1, -1, -1, -1, -1, -1 },     // 244
    { false,  true,   0, 6, 0, 6, -1, -1 },         // 245
    { false,  false,  0, 9, 13, 13, 17, 14 },       // 246
    { false,  false,  0, 9, 13, 13, 14, 14 },       // 247
    { false,  false,  0, 9, 13, 13, 14, 14 },       // 248
    { false,  false,  0, 9, 13, 13, 17, 14 },       // 249
    { false,  true,   0, 6, 8, 8, 10, 9 },          // 250
    { false,  true,   0, 8, 9, 9, 11, 10 },         // 251
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 252
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 253
    { false,  false,  0, 9, 17, 13, 17, 14 },       // 254
    { false,  false,  -1, -1, -1, -1, -1, -1 },     // 255

};


AISTATE genPatrolStates[] = {

    //-------------------------------------------------------------------------------

    { kAiStatePatrolWaitL, 0, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitL, 7, -1, 0, NULL, NULL, aiPatrolThink, NULL },

    { kAiStatePatrolMoveL, 9, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveL, 8, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveL, 0, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveL, 6, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveL, 7, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },

    { kAiStatePatrolTurnL, 9, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnL, 8, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnL, 0, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnL, 6, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnL, 7, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },

    //-------------------------------------------------------------------------------

    { kAiStatePatrolWaitW, 0, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitW, 10, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitW, 13, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitW, 17, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitW, 8, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitW, 9, -1, 0, NULL, NULL, aiPatrolThink, NULL },

    { kAiStatePatrolMoveW, 0, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveW, 10, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveW, 13, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveW, 8, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveW, 9, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveW, 7, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveW, 6, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },

    { kAiStatePatrolTurnW, 0, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnW, 10, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnW, 13, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnW, 8, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnW, 9, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnW, 7, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnW, 6, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },

    //-------------------------------------------------------------------------------

    { kAiStatePatrolWaitC, 17, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitC, 11, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitC, 10, -1, 0, NULL, NULL, aiPatrolThink, NULL },
    { kAiStatePatrolWaitC, 14, -1, 0, NULL, NULL, aiPatrolThink, NULL },

    { kAiStatePatrolMoveC, 14, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveC, 10, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },
    { kAiStatePatrolMoveC, 9, -1, 0, NULL, aiPatrolMove, aiPatrolThink, NULL },

    { kAiStatePatrolTurnC, 14, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnC, 10, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },
    { kAiStatePatrolTurnC, 9, -1, 0, aiPatrolRandGoalAng, aiPatrolTurn, aiPatrolThink, NULL },

    //-------------------------------------------------------------------------------

};

CONDITION_TYPE_NAMES gCondTypeNames[7] = {
    
    {kCondGameBase,     kCondGameMax,   "Game"},
    {kCondMixedBase,    kCondMixedMax,  "Mixed"},
    {kCondWallBase,     kCondWallMax,   "Wall"},
    {kCondSectorBase,   kCondSectorMax, "Sector"},
    {kCondPlayerBase,   kCondPlayerMax, "Player"},
    {kCondDudeBase,     kCondDudeMax,   "Enemy"},
    {kCondSpriteBase,   kCondSpriteMax, "Sprite"},

};

// for actor.cpp
//-------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static DBloodActor* nnExtSpawnDude(DBloodActor* sourceActor, DBloodActor* origin, int nType, int a3, int a4)
{
    DBloodActor* pDudeActor = nullptr;
    auto pSource = &sourceActor->s();
    auto pXSource = &sourceActor->x();
    auto pOrigin = &origin->s();

    if (nType < kDudeBase || nType >= kDudeMax || (pDudeActor = actSpawnSprite(origin, kStatDude)) == NULL)
        return NULL;

    spritetype* pDude = &pDudeActor->s();
    XSPRITE* pXDude = &pDudeActor->x();

    int angle = pOrigin->ang;
    int x, y, z = a4 + pOrigin->z;
    if (a3 < 0)
    {
        x = pOrigin->pos.X;
        y = pOrigin->y;
    } 
    else
    {
        x = pOrigin->pos.X + mulscale30r(Cos(angle), a3);
        y = pOrigin->y + mulscale30r(Sin(angle), a3);
    }

    vec3_t pos = { x, y, z };
    SetActor(pDudeActor, &pos);

    pDude->type = nType;
    pDude->ang = angle;

    pDude->cstat |= CSTAT_SPRITE_BLOOD_BIT1 | CSTAT_SPRITE_BLOCK_ALL;
    pDude->clipdist = getDudeInfo(nType)->clipdist;

    pXDude->respawn = 1;
    pXDude->health = getDudeInfo(nType)->startHealth << 4;

    if (fileSystem.FindResource(getDudeInfo(nType)->seqStartID, "SEQ"))
        seqSpawn(getDudeInfo(nType)->seqStartID, pDudeActor, -1);

    // add a way to inherit some values of spawner by dude.
    if (pSource->flags & kModernTypeFlag1) {

        //inherit pal?
        if (pDude->pal <= 0)
            pDude->pal = pSource->pal;

        // inherit spawn sprite trigger settings, so designer can count monsters.
        pXDude->txID = pXSource->txID;
        pXDude->command = pXSource->command;
        pXDude->triggerOn = pXSource->triggerOn;
        pXDude->triggerOff = pXSource->triggerOff;

        // inherit drop items
        pXDude->dropMsg = pXSource->dropMsg;

        // inherit dude flags
        pXDude->dudeDeaf = pXSource->dudeDeaf;
        pXDude->dudeGuard = pXSource->dudeGuard;
        pXDude->dudeAmbush = pXSource->dudeAmbush;
        pXDude->dudeFlag4 = pXSource->dudeFlag4;
        pXDude->unused1 = pXSource->unused1;

    }

    aiInitSprite(pDudeActor);

    gKillMgr.AddNewKill(1);

    bool burning = IsBurningDude(pDudeActor);
    if (burning) {
        pXDude->burnTime = 10;
        pDudeActor->SetTarget(nullptr);
    }

    if ((burning || (pSource->flags & kModernTypeFlag3)) && !pXDude->dudeFlag4)
        aiActivateDude(pDudeActor);

    return pDudeActor;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool nnExtIsImmune(DBloodActor* actor, int dmgType, int minScale) 
{
    auto pSprite = &actor->s();
    if (dmgType >= kDmgFall && dmgType < kDmgMax && actor->hasX() && actor->x().locked != 1) 
    {
        if (pSprite->type >= kThingBase && pSprite->type < kThingMax)
        {
            return (thingInfo[pSprite->type - kThingBase].dmgControl[dmgType] <= minScale);
        }
        else if (actor->IsDudeActor()) 
        {
            if (actor->IsPlayerActor()) return (gPlayer[pSprite->type - kDudePlayer1].damageControl[dmgType]);
            else if (pSprite->type == kDudeModernCustom) return (actor->genDudeExtra.dmgControl[dmgType] <= minScale);
            else return (getDudeInfo(pSprite->type)->damageVal[dmgType] <= minScale);
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool nnExtEraseModernStuff(DBloodActor* actor) 
{
    auto pSprite = &actor->s();
    auto pXSprite = &actor->x();

    bool erased = false;
    switch (pSprite->type) {
        // erase all modern types if the map is not extended
        case kModernSpriteDamager:
        case kModernCustomDudeSpawn:
        case kModernRandomTX:
        case kModernSequentialTX:
        case kModernSeqSpawner:
        case kModernObjPropertiesChanger:
        case kModernObjPicnumChanger:
        case kModernObjSizeChanger:
        case kModernDudeTargetChanger:
        case kModernSectorFXChanger:
        case kModernObjDataChanger:
        case kModernObjDataAccumulator:
        case kModernEffectSpawner:
        case kModernWindGenerator:
        case kModernPlayerControl:
        case kModernCondition:
        case kModernConditionFalse:
        case kModernSlopeChanger:
        case kModernStealthRegion:
            pSprite->type = kSpriteDecoration;
            erased = true;
            break;
        case kItemModernMapLevel:
        case kDudeModernCustom:
        case kDudeModernCustomBurning:
        case kModernThingTNTProx:
        case kModernThingEnemyLifeLeech:
            pSprite->type = kSpriteDecoration;
            ChangeActorStat(actor, kStatDecoration);
            erased = true;
            break;
        // also erase some modernized vanilla types which was not active
        case kMarkerWarpDest:
            if (pSprite->statnum == kStatMarker) break;
            pSprite->type = kSpriteDecoration;
            erased = true;
            break;
    }

    if (pXSprite->Sight) 
    {
        pXSprite->Sight = false; // it does not work in vanilla at all
        erased = true;
    }

    if (pXSprite->Proximity) 
    {
        // proximity works only for things and dudes in vanilla
        switch (pSprite->statnum) 
        {
            case kStatThing:
            case kStatDude:
                break;
            default:
                pXSprite->Proximity = false;
                erased = true;
                break;
        }
    }

    return erased;
}

//---------------------------------------------------------------------------
//

//
//---------------------------------------------------------------------------

void nnExtTriggerObject(EventObject& eob, int command)
{
    if (eob.isSector())
    {
        trTriggerSector(eob.sector(), command);
    }
    else if (eob.isWall())
    {
        trTriggerWall(eob.wall(), command);
    }
    else if (eob.isActor())
    {
        auto objActor = eob.actor();
        if (!objActor || !objActor->hasX()) return;
        trTriggerSprite(objActor, command);
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void nnExtResetGlobals() 
{
    gAllowTrueRandom = gEventRedirectsUsed = false;

    // reset counters
    gProxySpritesCount = gSightSpritesCount = gPhysSpritesCount = gImpactSpritesCount = 0;

    // fill arrays with negative values to avoid index 0 situation
    memset(gSightSpritesList, 0, sizeof(gSightSpritesList));   
    memset(gProxySpritesList, 0, sizeof(gProxySpritesList));
    memset(gPhysSpritesList, 0, sizeof(gPhysSpritesList));  
    memset(gImpactSpritesList, 0, sizeof(gImpactSpritesList));

    // reset tracking conditions, if any
    for (size_t i = 0; i < countof(gCondition); i++) 
    {
        TRCONDITION* pCond = &gCondition[i];
        for (unsigned k = 0; k < kMaxTracedObjects; k++)
        {
            pCond->obj[k].obj = EventObject(nullptr);
        }
        pCond->actor = nullptr;
        pCond->length = 0;
    }
    gTrackingCondsCount = 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void nnExtInitModernStuff(TArray<DBloodActor*>& actors) 
{
    nnExtResetGlobals();

    // use true random only for single player mode, otherwise use Blood's default one.
    if (gGameOptions.nGameType == 0 && !VanillaMode()) 
    {
        gStdRandom.seed(std::random_device()());

        // since true random is not working if compiled with old mingw versions, we should
        // check if it works in game and if not - switch to using in-game random function.
        for (int i = kMaxRandomizeRetries; i >= 0; i--) 
        {
            std::uniform_int_distribution<int> dist_a_b(0, 100);
            if (gAllowTrueRandom || i <= 0) break;
            else if (dist_a_b(gStdRandom) != 0)
                gAllowTrueRandom = true;
        }
    }
    
    for (auto actor : actors)
    {
        if (!actor->exists() || !actor->hasX()) continue;
        XSPRITE* pXSprite = &actor->x();
        spritetype* pSprite = &actor->s();
        
        switch (pSprite->type) {
            case kModernRandomTX:
            case kModernSequentialTX:
                if (pXSprite->command == kCmdLink) gEventRedirectsUsed = true;
                break;
            case kDudeModernCustom:
            case kDudeModernCustomBurning:
                getSpriteMassBySize(actor); // create mass cache
                break;
            case kModernCondition:
            case kModernConditionFalse:
                if (!pXSprite->rxID && pXSprite->data1 > kCondGameMax) condError(actor,"\nThe condition must have RX ID!\nSPRITE #%d", actor->GetIndex());
                else if (!pXSprite->txID && !pSprite->flags) 
                {
                    Printf(PRINT_HIGH, "The condition must have TX ID or hitag to be set: RX ID %d, SPRITE #%d", pXSprite->rxID, actor->GetIndex());
                }
                break;
        }

            // auto set going On and going Off if both are empty
            if (pXSprite->txID && !pXSprite->triggerOn && !pXSprite->triggerOff)
                pXSprite->triggerOn = pXSprite->triggerOff = true;
            
            // copy custom start health to avoid overwrite by kThingBloodChunks
            if (actor->IsDudeActor())
                pXSprite->sysData2 = pXSprite->data4;
            
            // check reserved statnums
            if (pSprite->statnum >= kStatModernBase && pSprite->statnum < kStatModernMax) 
            {
                bool sysStat = true;
                switch (pSprite->statnum) 
                {
                    case kStatModernStealthRegion:
                        sysStat = (pSprite->type != kModernStealthRegion);
                        break;
                    case kStatModernDudeTargetChanger:
                        sysStat = (pSprite->type != kModernDudeTargetChanger);
                        break;
                    case kStatModernCondition:
                        sysStat = (pSprite->type != kModernCondition && pSprite->type != kModernConditionFalse);
                        break;
                    case kStatModernEventRedirector:
                        sysStat = (pSprite->type != kModernRandomTX && pSprite->type != kModernSequentialTX);
                        break;
                    case kStatModernWindGen:
                        sysStat = (pSprite->type != kModernWindGenerator);
                        break;
                    case kStatModernPlayerLinker:
                    case kStatModernQavScene:
                        sysStat = (pSprite->type != kModernPlayerControl);
                        break;
                }

                if (sysStat)
                    I_Error("Sprite statnum %d on sprite #%d is in a range of reserved (%d - %d)!", pSprite->statnum, actor->GetIndex(), kStatModernBase, kStatModernMax);
            }

            switch (pSprite->type) 
            {
                case kModernRandomTX:
                case kModernSequentialTX:
                    if (pXSprite->command != kCmdLink) break;
                    // add statnum for faster redirects search
                    ChangeActorStat(actor, kStatModernEventRedirector);
                    break;
                case kModernWindGenerator:
                    pSprite->cstat &= ~CSTAT_SPRITE_BLOCK;
                    ChangeActorStat(actor, kStatModernWindGen);
                    break;
                case kModernDudeTargetChanger:
                case kModernObjDataAccumulator:
                case kModernRandom:
                case kModernRandom2:
                case kModernStealthRegion:
                    pSprite->cstat &= ~CSTAT_SPRITE_BLOCK;
                    pSprite->cstat |= CSTAT_SPRITE_INVISIBLE;
                    switch (pSprite->type) 
                    {
                        // stealth regions for patrolling enemies
                        case kModernStealthRegion:
                            ChangeActorStat(actor, kStatModernStealthRegion);
                            break;
                        // add statnum for faster dude searching
                        case kModernDudeTargetChanger:
                            ChangeActorStat(actor, kStatModernDudeTargetChanger);
                            if (pXSprite->busyTime <= 0) pXSprite->busyTime = 5;
                            pXSprite->command = kCmdLink;
                            break;
                            // remove kStatItem status from random item generators
                        case kModernRandom:
                        case kModernRandom2:
                            ChangeActorStat(actor, kStatDecoration);
                            pXSprite->sysData1 = pXSprite->command; // save the command so spawned item can inherit it
                            pXSprite->command  = kCmdLink;  // generator itself can't send commands
                            break;
                    }
                    break;
                case kModernThingTNTProx:
                    pXSprite->Proximity = true;
                    break;
                case kDudeModernCustom: 
                {
                    if (pXSprite->txID <= 0) break;
                    int found = 0;
                    BloodStatIterator it(kStatDude);
                    while (DBloodActor* iactor = it.Next())
                    {
                        XSPRITE* pXSpr = &iactor->x();
                        if (pXSpr->rxID != pXSprite->txID) continue;
                        else if (found) I_Error("\nCustom dude (TX ID %d):\nOnly one incarnation allowed per channel!", pXSprite->txID);
                        ChangeActorStat(iactor, kStatInactive);
                        found++;
                    }
                    break;
                }
                case kDudePodMother:
                case kDudeTentacleMother:
                    pXSprite->state = 1;
                    break;
                case kModernPlayerControl:
                    switch (pXSprite->command) 
                    {
                        case kCmdLink:
                        {
                            if (pXSprite->data1 < 1 || pXSprite->data1 > kMaxPlayers)
                                I_Error("\nPlayer Control (SPRITE #%d):\nPlayer out of a range (data1 = %d)", actor->GetIndex(), pXSprite->data1);

                            //if (numplayers < pXSprite->data1)
                            //I_Error("\nPlayer Control (SPRITE #%d):\n There is no player #%d", actor->GetIndex(), pXSprite->data1);

                            if (pXSprite->rxID && pXSprite->rxID != kChannelLevelStart)
                                I_Error("\nPlayer Control (SPRITE #%d) with Link command should have no RX ID!", actor->GetIndex());

                            if (pXSprite->txID && pXSprite->txID < kChannelUser)
                                I_Error("\nPlayer Control (SPRITE #%d):\nTX ID should be in range of %d and %d!", actor->GetIndex(), kChannelUser, kChannelMax);

                            // only one linker per player allowed
                        BloodStatIterator it(kStatModernPlayerLinker);
                        while (auto iactor = it.Next())
                            {
                            XSPRITE* pXCtrl = &iactor->x();
                                if (pXSprite->data1 == pXCtrl->data1)
                                I_Error("\nPlayer Control (SPRITE #%d):\nPlayer %d already linked with different player control sprite #%d!", actor->GetIndex(), pXSprite->data1, iactor->GetIndex());
                            }
                            pXSprite->sysData1 = -1;
                            pSprite->cstat &= ~CSTAT_SPRITE_BLOCK;
                            ChangeActorStat(actor, kStatModernPlayerLinker);
                            break;
                        }
                        case 67: // play qav animation
                            if (pXSprite->txID && !pXSprite->waitTime) pXSprite->waitTime = 1;
                            ChangeActorStat(actor, kStatModernQavScene);
                            break;
                    }
                    break;
                case kModernCondition:
                case kModernConditionFalse:
                    if (pXSprite->busyTime > 0) 
                    {
                        if (pXSprite->waitTime > 0) 
                        {
                            pXSprite->busyTime += ClipHigh(((pXSprite->waitTime * 120) / 10), 4095); pXSprite->waitTime = 0;
                        Printf(PRINT_HIGH, "Summing busyTime and waitTime for tracking condition #%d, RX ID %d. Result = %d ticks", actor->GetIndex(), pXSprite->rxID, pXSprite->busyTime);
                        }
                        pXSprite->busy = pXSprite->busyTime;
                    }
                    
                    if (pXSprite->waitTime && pXSprite->command >= kCmdNumberic)
                    condError(actor, "Delay is not available when using numberic commands (%d - %d)", kCmdNumberic, 255);

                    pXSprite->Decoupled = false; // must go through operateSprite always
                    pXSprite->Sight     = pXSprite->Impact  = pXSprite->Touch   = pXSprite->triggerOff     = false;
                    pXSprite->Proximity = pXSprite->Push    = pXSprite->Vector  = pXSprite->triggerOn      = false;
                    pXSprite->state = pXSprite->restState = 0;
                    
                    pXSprite->targetX = pXSprite->targetY = pXSprite->targetZ = pXSprite->sysData2 = -1;
                    actor->SetTarget(nullptr);
                    ChangeActorStat(actor, kStatModernCondition);
                    auto oldStat = pSprite->cstat; 
                    pSprite->cstat = CSTAT_SPRITE_ALIGNMENT_SLOPE;
                    
                    if (oldStat & CSTAT_SPRITE_BLOCK) 
                        pSprite->cstat |= CSTAT_SPRITE_BLOCK;
                    
                    if (oldStat & CSTAT_SPRITE_MOVE_FORWARD) pSprite->cstat |= CSTAT_SPRITE_MOVE_FORWARD;
                    else if (oldStat & CSTAT_SPRITE_MOVE_REVERSE) pSprite->cstat |= CSTAT_SPRITE_MOVE_REVERSE;

                    pSprite->cstat |= CSTAT_SPRITE_INVISIBLE;
                    break;
            }

            // the following trigger flags are senseless to have together
            if ((pXSprite->Touch && (pXSprite->Proximity || pXSprite->Sight) && pXSprite->DudeLockout)
                    || (pXSprite->Touch && pXSprite->Proximity && !pXSprite->Sight)) pXSprite->Touch = false;

            if (pXSprite->Proximity && pXSprite->Sight && pXSprite->DudeLockout)
                pXSprite->Proximity = false;
            
            // very quick fix for floor sprites with Touch trigger flag if their Z is equals sector floorz / ceilgz
            if (pSprite->insector() && pXSprite->Touch && (pSprite->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR)) {
                if (pSprite->z == pSprite->sector()->floorz) pSprite->z--;
                else if (pSprite->z == pSprite->sector()->ceilingz) pSprite->z++;
            }

        // make Proximity flag work not just for dudes and things...
        if (pXSprite->Proximity && gProxySpritesCount < kMaxSuperXSprites) 
        {
            switch (pSprite->statnum) 
            {
                case kStatFX:           case kStatExplosion:            case kStatItem:
                case kStatPurge:        case kStatSpares:               case kStatFlare:
                case kStatInactive:     case kStatFree:                 case kStatMarker:
                case kStatPathMarker:   case kStatThing:                case kStatDude:
                case kStatModernPlayerLinker:
                    break;
                default:
                    gProxySpritesList[gProxySpritesCount++] = actor;
                    if (gProxySpritesCount == kMaxSuperXSprites)
                        I_Error("Max (%d) *additional* Proximity sprites reached!", kMaxSuperXSprites);
                    break;
            }
        }

        // make Sight, Screen, Aim flags work not just for dudes and things...
        if ((pXSprite->Sight || pXSprite->unused3) && gSightSpritesCount < kMaxSuperXSprites) 
        {
            switch (pSprite->statnum) 
            {
                case kStatFX:           case kStatExplosion:            case kStatItem:
                case kStatPurge:        case kStatSpares:               case kStatFlare:
                case kStatInactive:     case kStatFree:                 case kStatMarker:
                case kStatPathMarker:   case kStatModernPlayerLinker:
                    break;
                default:
                    gSightSpritesList[gSightSpritesCount++] = actor;
                    if (gSightSpritesCount == kMaxSuperXSprites)
                        I_Error("Max (%d) Sight sprites reached!", kMaxSuperXSprites);
                    break;
            }
        }

        // make Impact flag work for sprites that affected by explosions...
        if (pXSprite->Impact && gImpactSpritesCount < kMaxSuperXSprites) 
        {
            switch (pSprite->statnum) 
            {
                case kStatFX:           case kStatExplosion:            case kStatItem:
                case kStatPurge:        case kStatSpares:               case kStatFlare:
                case kStatInactive:     case kStatFree:                 case kStatMarker:
                case kStatPathMarker:   case kStatModernPlayerLinker:
                    break;
                default:
                    gImpactSpritesList[gImpactSpritesCount++] = actor;
                    if (gImpactSpritesCount == kMaxSuperXSprites)
                        I_Error("Max (%d) *additional* Impact sprites reached!", kMaxSuperXSprites);
                    break;
            }
        }
    }

    // collect objects for tracking conditions
    BloodStatIterator it2(kStatModernCondition);
    while (auto iactor = it2.Next())
    {
        XSPRITE* pXSprite = &iactor->x();

        if (pXSprite->busyTime <= 0 || pXSprite->isTriggered) continue;
        else if (gTrackingCondsCount >= kMaxTrackingConditions)
            I_Error("\nMax (%d) tracking conditions reached!", kMaxTrackingConditions);
            
        int count = 0;
        TRCONDITION* pCond = &gCondition[gTrackingCondsCount];

        for (auto iactor2 : actors)
        {
            if (!iactor->exists() || !iactor2->hasX() || iactor2->x().txID != pXSprite->rxID || iactor2 == iactor)
                continue;

            XSPRITE* pXSpr = &iactor2->x();
            spritetype* pSpr = &iactor2->s();
            switch (pSpr->type) 
            {
                case kSwitchToggle: // exceptions
                case kSwitchOneWay: // exceptions
                    continue;
            }

            if (pSpr->type == kModernCondition || pSpr->type == kModernConditionFalse)
                condError(iactor, "Tracking condition always must be first in condition sequence!");

            if (count >= kMaxTracedObjects)
                condError(iactor, "Max(%d) objects to track reached for condition #%d, RXID: %d!");

            pCond->obj[count].obj = EventObject(iactor2);
            pCond->obj[count++].cmd = (uint8_t)pXSpr->command;
        }

        for (auto& sect: sector)
        {
            if (!sect.hasX() || sect.xs().txID != pXSprite->rxID) continue;
            else if (count >= kMaxTracedObjects)
                condError(iactor, "Max(%d) objects to track reached for condition #%d, RXID: %d!");

            pCond->obj[count].obj = EventObject(&sect);
            pCond->obj[count++].cmd = sect.xs().command;
        }

        for(auto& wal : wall)
        {
            if (!wal.hasX() || wal.xw().txID != pXSprite->rxID)
                continue;

            switch (wal.type) {
                case kSwitchToggle: // exceptions
                case kSwitchOneWay: // exceptions
                    continue;
            }

            if (count >= kMaxTracedObjects)
                condError(iactor, "Max(%d) objects to track reached for condition #%d, RXID: %d!");
                
            pCond->obj[count].obj = EventObject(&wal);
            pCond->obj[count++].cmd = wal.xw().command;
        }

        if (pXSprite->data1 > kCondGameMax && count == 0)
            Printf(PRINT_HIGH, "No objects to track found for condition #%d, RXID: %d!", iactor->GetIndex(), pXSprite->rxID);

        pCond->length = count;
        pCond->actor = iactor;
        gTrackingCondsCount++;

    }
}


// The following functions required for random event features
//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int nnExtRandom(int a, int b) 
{
    if (!gAllowTrueRandom) return Random(((b + 1) - a)) + a;
    // used for better randomness in single player
    std::uniform_int_distribution<int> dist_a_b(a, b);
    return dist_a_b(gStdRandom);
}

static int GetDataVal(DBloodActor* actor, int data) 
{
    if (!actor->hasX()) return -1;
    
    switch (data) {
        case 0: return actor->x().data1;
        case 1: return actor->x().data2;
        case 2: return actor->x().data3;
        case 3: return actor->x().data4;
    }

    return -1;
}

//---------------------------------------------------------------------------
//
// tries to get random data field of sprite
//
//---------------------------------------------------------------------------

static int randomGetDataValue(DBloodActor* actor, int randType) 
{
    if (actor == NULL || !actor->hasX()) return -1;
    auto pXSprite = &actor->x();
    int random = 0; int bad = 0; int maxRetries = kMaxRandomizeRetries;

    int rData[4];
    rData[0] = pXSprite->data1; rData[2] = pXSprite->data3;
    rData[1] = pXSprite->data2; rData[3] = pXSprite->data4;
    // randomize only in case if at least 2 data fields fits.
    for (int i = 0; i < 4; i++) 
    {
        switch (randType) {
        case kRandomizeItem:
            if (rData[i] >= kItemWeaponBase && rData[i] < kItemMax) break;
            else bad++;
            break;
        case kRandomizeDude:
            if (rData[i] >= kDudeBase && rData[i] < kDudeMax) break;
            else bad++;
            break;
        case kRandomizeTX:
            if (rData[i] > kChannelZero && rData[i] < kChannelUserMax) break;
            else bad++;
            break;
        default:
            bad++;
            break;
        }
    }

    if (bad < 3) 
    {
        // try randomize few times
        while (maxRetries > 0) 
        {
            random = nnExtRandom(0, 3);
            if (rData[random] > 0) return rData[random];
            else maxRetries--;
        }
    }

    return -1;
}

//---------------------------------------------------------------------------
//
// this function drops random item using random pickup generator(s)
//
//---------------------------------------------------------------------------

static DBloodActor* randomDropPickupObject(DBloodActor* sourceactor, int prevItem) 
{
    DBloodActor* spawned = nullptr; 
    int selected = -1; 
    int maxRetries = 9;
    if (sourceactor->hasX())
    {
        auto pSource = &sourceactor->s();
        XSPRITE* pXSource = &sourceactor->x();
        while ((selected = randomGetDataValue(sourceactor, kRandomizeItem)) == prevItem) if (maxRetries-- <= 0) break;
        if (selected > 0) 
        {
            spawned = actDropObject(sourceactor, selected);
            if (spawned) 
            {
                auto pSprite2 = &spawned->s();

                pXSource->dropMsg = uint8_t(pSprite2->type); // store dropped item type in dropMsg
                pSprite2->pos.X = pSource->pos.X;
                pSprite2->y = pSource->y;
                pSprite2->z = pSource->z;

                if ((pSource->flags & kModernTypeFlag1) && (pXSource->txID > 0 || (pXSource->txID != 3 && pXSource->lockMsg > 0))) 
                {
                    spawned->addX();
                    XSPRITE* pXSprite2 = &spawned->x();

                    // inherit spawn sprite trigger settings, so designer can send command when item picked up.
                    pXSprite2->txID = pXSource->txID;
                    pXSprite2->command = pXSource->sysData1;
                    pXSprite2->triggerOn = pXSource->triggerOn;
                    pXSprite2->triggerOff = pXSource->triggerOff;

                    pXSprite2->Pickup = true;

                }
            }
        }
    }
    return spawned;
}

//---------------------------------------------------------------------------
//
// this function spawns random dude using dudeSpawn
//
//---------------------------------------------------------------------------

DBloodActor* randomSpawnDude(DBloodActor* sourceactor, DBloodActor* origin, int a3, int a4)
{
    DBloodActor* spawned = NULL; int selected = -1;
    
    if ((selected = randomGetDataValue(sourceactor, kRandomizeDude)) > 0)
        spawned = nnExtSpawnDude(sourceactor, origin, selected, a3, 0);

    return spawned;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

static void windGenDoVerticalWind(int factor, sectortype* pSector) 
{
    int val, maxZ = 0, zdiff; bool maxZfound = false;
   
    // find maxz marker first
    BloodSectIterator it(pSector);
    while (auto actor = it.Next())
    {
        auto sp = &actor->s();
        if (sp->type == kMarkerOn && sp->statnum != kStatMarker) 
        {
            maxZ = sp->z;
            maxZfound = true;
            break;
        }
    }

    it.Reset(pSector);
    while (auto actor = it.Next())
    {
        auto pSpr = &actor->s();
        switch (pSpr->statnum) 
        {
            case kStatFree:
                continue;
            case kStatFX:
                if (actor->zvel) break;
                continue;
            case kStatThing:
            case kStatDude:
                if (pSpr->flags & kPhysGravity) break;
                continue;
            default:
                if (actor->hasX() && actor->x().physAttr & kPhysGravity) break;
                continue;
        }

        if (maxZfound && pSpr->z <= maxZ) 
        {
            zdiff = pSpr->z - maxZ;
            if (actor->zvel < 0) actor->zvel += MulScale(actor->zvel >> 4, zdiff, 16);
            continue;

        }

        val = -MulScale(factor * 64, 0x10000, 16);
        if (actor->zvel >= 0) actor->zvel += val;
        else actor->zvel = val;

        pSpr->z += actor->zvel >> 12;

    }

}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void nnExtProcessSuperSprites()
{
    // process tracking conditions
    if (gTrackingCondsCount > 0) 
    {
        for (int i = 0; i < gTrackingCondsCount; i++) 
        {
            TRCONDITION const* pCond = &gCondition[i];
            XSPRITE* pXCond = &pCond->actor->x();
            if (pXCond->locked || pXCond->isTriggered || ++pXCond->busy < pXCond->busyTime)
                continue;

            if (pXCond->data1 >= kCondGameBase && pXCond->data1 < kCondGameMax)
            {
                EVENT evn;
                evn.target = EventObject(pCond->actor);
                evn.cmd = (int8_t)pXCond->command;
                evn.funcID = kCallbackMax;
                useCondition(pCond->actor, evn);
            }
            else if (pCond->length > 0)
            {
                pXCond->busy = 0;
                for (unsigned k = 0; k < pCond->length; k++)
                {
                    EVENT evn;
                    evn.target = pCond->obj[k].obj;
                    evn.cmd    = pCond->obj[k].cmd;
                    evn.funcID = kCallbackMax;
                    useCondition(pCond->actor, evn);
                }
            }
            
        }
    }
    
    // process floor oriented kModernWindGenerator to create a vertical wind in the sectors
    BloodStatIterator it(kStatModernWindGen);
    while (auto windactor = it.Next())
    {

        spritetype* pWind = &windactor->s();
        if (!(pWind->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR) || pWind->statnum >= kMaxStatus || !windactor->hasX())
            continue;

        XSPRITE* pXWind = &windactor->x();
        if (!pXWind->state || pXWind->locked)
            continue;

        int j, rx;
        bool fWindAlways = (pWind->flags & kModernTypeFlag1);

        if (pXWind->txID) {

            rx = pXWind->txID;
            for (j = bucketHead[rx]; j < bucketHead[rx + 1]; j++)
            {
                if (!rxBucket[j].isSector()) continue;
                auto pSector = rxBucket[j].sector();
                XSECTOR* pXSector = &pSector->xs();

                if ((!pXSector->locked) && (fWindAlways || pXSector->windAlways || pXSector->busy))
                    windGenDoVerticalWind(pXWind->sysData2, pSector);
            }

            DBloodActor* pXRedir = nullptr; // check redirected TX buckets
            while ((pXRedir = evrListRedirectors(OBJ_SPRITE, nullptr, nullptr, windactor, pXRedir, &rx)) != nullptr)
            {
                for (j = bucketHead[rx]; j < bucketHead[rx + 1]; j++)
                {
                    if (!rxBucket[j].isSector()) continue;
                    auto pSector = rxBucket[j].sector();
                    XSECTOR* pXSector = &pSector->xs();

                    if ((!pXSector->locked) && (fWindAlways || pXSector->windAlways || pXSector->busy))
                        windGenDoVerticalWind(pXWind->sysData2, pSector);
                }
            }

        }
        else if (pWind->insector())
        {
            sectortype* pSect = pWind->sector();
            XSECTOR* pXSector = (pSect->hasX()) ? &pSect->xs() : nullptr;
            if ((fWindAlways) || (pXSector && !pXSector->locked && (pXSector->windAlways || pXSector->busy)))
                windGenDoVerticalWind(pXWind->sysData2, pWind->sector());
        }
    }

    // process additional proximity sprites
    if (gProxySpritesCount > 0)
    {
        for (int i = 0; i < gProxySpritesCount; i++)
        {
            DBloodActor* pProx = gProxySpritesList[i];
            if (!pProx || !pProx->hasX()) continue;

            auto const pProxSpr = &pProx->s();
            XSPRITE* pXProxSpr = &pProx->x();
            if (!pXProxSpr->Proximity || (!pXProxSpr->Interrutable && pXProxSpr->state != pXProxSpr->restState) || pXProxSpr->locked == 1
                || pXProxSpr->isTriggered) continue;  // don't process locked or triggered sprites

            int okDist = (pProx->IsDudeActor()) ? 96 : ClipLow(pProxSpr->clipdist * 3, 32);
            int x = pProxSpr->pos.X;
            int y = pProxSpr->y;
            int z = pProxSpr->z;	
            auto pSect = pProxSpr->sector();

            if (!pXProxSpr->DudeLockout)
            {
                BloodStatIterator it(kStatDude);
                while (auto affected = it.Next())
                {
                    if (!affected->hasX() || affected->x().health <= 0) continue;
                    else if (CheckProximity(affected, x, y, z, pSect, okDist))
                    {
                        trTriggerSprite(pProx, kCmdSpriteProximity);
                        break;
                    }
                }
            }
            else
            {
                for (int a = connecthead; a >= 0; a = connectpoint2[a])
                {
                    PLAYER* pPlayer = &gPlayer[a];
                    if (!pPlayer || !pPlayer->actor->hasX() || pPlayer->pXSprite->health <= 0)
                        continue;

                    if (pPlayer->pXSprite->health > 0 && CheckProximity(gPlayer->actor, x, y, z, pSect, okDist))
                    {
                        trTriggerSprite(pProx, kCmdSpriteProximity);
                        break;
                    }
                }
            }
        }
    }

    // process sight sprites (for players only)
    if (gSightSpritesCount > 0)
    {
        for (int i = 0; i < gSightSpritesCount; i++)
        {
            DBloodActor* pSight = gSightSpritesList[i];
            if (!pSight || !pSight->hasX()) continue;

            auto const pSightSpr = &pSight->s();
            XSPRITE* pXSightSpr = &pSight->x();

            if ((!pXSightSpr->Interrutable && pXSightSpr->state != pXSightSpr->restState) || pXSightSpr->locked == 1 ||
                pXSightSpr->isTriggered) continue; // don't process locked or triggered sprites

            // sprite is drawn for one of players
            if ((pXSightSpr->unused3 & kTriggerSpriteScreen) && (pSight->spr.cstat2 & CSTAT2_SPRITE_MAPPED))
            {
                trTriggerSprite(pSight, kCmdSpriteSight);
                pSight->spr.cstat2 &= ~CSTAT2_SPRITE_MAPPED;
                continue;
            }

            int x = pSightSpr->pos.X;	
            int y = pSightSpr->y;
            int z = pSightSpr->z; 
            auto pSightSect = pSightSpr->sector();
            int ztop2, zbot2;
            
            for (int a = connecthead; a >= 0; a = connectpoint2[a])
            {
                PLAYER* pPlayer = &gPlayer[a];
                if (!pPlayer || !pPlayer->actor->hasX() || pPlayer->pXSprite->health <= 0)
                    continue;

                spritetype* pPlaySprite = pPlayer->pSprite;
                GetSpriteExtents(pPlaySprite, &ztop2, &zbot2);
                if (cansee(x, y, z, pSightSect, pPlaySprite->pos.X, pPlaySprite->y, ztop2, pPlaySprite->sector()))
                {
                    if (pXSightSpr->Sight)
                    {
                    trTriggerSprite(pSight, kCmdSpriteSight);
                        break;
                    }

                    if (pXSightSpr->unused3 & kTriggerSpriteAim)
                    {
                        bool vector = (pSightSpr->cstat & CSTAT_SPRITE_BLOCK_HITSCAN);
                        if (!vector)
                            pSightSpr->cstat |= CSTAT_SPRITE_BLOCK_HITSCAN;

                        HitScan(pPlayer->actor, pPlayer->zWeapon, pPlayer->aim.dx, pPlayer->aim.dy, pPlayer->aim.dz, CLIPMASK0 | CLIPMASK1, 0);

                        if (!vector)
                            pSightSpr->cstat &= ~CSTAT_SPRITE_BLOCK_HITSCAN;

                        if (gHitInfo.actor() == pSight)
                        {
                            trTriggerSprite(gHitInfo.actor(), kCmdSpriteSight);
                            break;
                        }
                    }
                }
            }
        }
    }

    // process Debris sprites for movement
    if (gPhysSpritesCount > 0)
    {
        for (int i = 0; i < gPhysSpritesCount; i++)
        {
            DBloodActor* debrisactor = gPhysSpritesList[i];
            if (debrisactor == nullptr || !debrisactor->hasX()) continue;
            auto const pDebris = &debrisactor->s();

            if (pDebris->statnum == kStatFree || (pDebris->flags & kHitagFree) != 0)
            {
                gPhysSpritesList[i] = nullptr;
                continue;
            }

            XSPRITE* pXDebris = &debrisactor->x();
            if (!(pXDebris->physAttr & kPhysMove) && !(pXDebris->physAttr & kPhysGravity))
            {
                gPhysSpritesList[i] = nullptr;
                continue;
            }

            XSECTOR* pXSector = (pDebris->sector()->hasX()) ? &pDebris->sector()->xs() : nullptr;
            viewBackupSpriteLoc(debrisactor);

            bool uwater = false;
            int mass = debrisactor->spriteMass.mass;
            int airVel = debrisactor->spriteMass.airVel;

            int top, bottom;
            GetActorExtents(debrisactor, &top, &bottom);
            
            if (pXSector != nullptr)
            {
                if ((uwater = pXSector->Underwater) != 0) airVel <<= 6;
                if (pXSector->panVel != 0 && getflorzofslopeptr(pDebris->sector(), pDebris->pos.X, pDebris->y) <= bottom)
                {
                    int angle = pXSector->panAngle; int speed = 0;
                    if (pXSector->panAlways || pXSector->state || pXSector->busy)
                    {
                        speed = pXSector->panVel << 9;
                        if (!pXSector->panAlways && pXSector->busy)
                            speed = MulScale(speed, pXSector->busy, 16);
                    }
                    if (pDebris->sector()->floorstat & CSTAT_SECTOR_ALIGN)
                        angle = (angle + GetWallAngle(pDebris->sector()->firstWall()) + 512) & 2047;
                    int dx = MulScale(speed, Cos(angle), 30);
                    int dy = MulScale(speed, Sin(angle), 30);
                    debrisactor->xvel += dx;
                    debrisactor->yvel += dy;
                }
            }

            actAirDrag(debrisactor, airVel);

            if (pXDebris->physAttr & kPhysDebrisTouch) 
            {
                PLAYER* pPlayer = NULL;
                for (int a = connecthead; a != -1; a = connectpoint2[a]) 
                {
                    pPlayer = &gPlayer[a];
                    DBloodActor* pact = pPlayer->actor;

                    if (pact && pact->hit.hit.type == kHitSprite && pact->hit.hit.actor() == debrisactor) 
                    {
                        int nSpeed = approxDist(pact->xvel, pact->yvel);
                            nSpeed = ClipLow(nSpeed - MulScale(nSpeed, mass, 6), 0x9000 - (mass << 3));

                            debrisactor->xvel += MulScale(nSpeed, Cos(pPlayer->pSprite->ang), 30);
                            debrisactor->yvel += MulScale(nSpeed, Sin(pPlayer->pSprite->ang), 30);

                        debrisactor->hit.hit.setSprite(pPlayer->actor);
                    }
                }
            }
            
            if (pXDebris->physAttr & kPhysGravity) pXDebris->physAttr |= kPhysFalling;
            if ((pXDebris->physAttr & kPhysFalling) || debrisactor->xvel || debrisactor->yvel || debrisactor->zvel || pDebris->sector()->velFloor || pDebris->sector()->velCeil)
            debrisMove(i);

            if (debrisactor->xvel || debrisactor->yvel)
                pXDebris->goalAng = getangle(debrisactor->xvel, debrisactor->yvel) & 2047;

            int ang = pDebris->ang & 2047;
            if ((uwater = spriteIsUnderwater(debrisactor)) == false) evKillActor(debrisactor, kCallbackEnemeyBubble);
            else if (Chance(0x1000 - mass)) 
            {
                if (debrisactor->zvel > 0x100) debrisBubble(debrisactor);
                if (ang == pXDebris->goalAng) 
                {
                   pXDebris->goalAng = (pDebris->ang + Random3(kAng60)) & 2047;
                    debrisBubble(debrisactor);
        }
    }

            int angStep = ClipLow(mulscale8(1, ((abs(debrisactor->xvel) + abs(debrisactor->yvel)) >> 5)), (uwater) ? 1 : 0);
            if (ang < pXDebris->goalAng) pDebris->ang = ClipHigh(ang + angStep, pXDebris->goalAng);
            else if (ang > pXDebris->goalAng) pDebris->ang = ClipLow(ang - angStep, pXDebris->goalAng);

            auto pSector = pDebris->sector();
            int cz = getceilzofslopeptr(pSector, pDebris->pos.X, pDebris->y);
            int fz = getflorzofslopeptr(pSector, pDebris->pos.X, pDebris->y);

            GetActorExtents(debrisactor, &top, &bottom);
            if (fz >= bottom && pSector->lowerLink == nullptr && !(pSector->ceilingstat & CSTAT_SECTOR_SKY)) pDebris->z += ClipLow(cz - top, 0);
            if (cz <= top && pSector->upperLink == nullptr && !(pSector->floorstat & CSTAT_SECTOR_SKY)) pDebris->z += ClipHigh(fz - bottom, 0);
        }
    }
}

//---------------------------------------------------------------------------
//
// this function plays sound predefined in missile info
//
//---------------------------------------------------------------------------

void sfxPlayMissileSound(DBloodActor* actor, int missileId) 
{
    const MISSILEINFO_EXTRA* pMissType = &gMissileInfoExtra[missileId - kMissileBase];
    sfxPlay3DSound(actor, Chance(0x5000) ? pMissType->fireSound[0] : pMissType->fireSound[1], -1, 0);
}

//---------------------------------------------------------------------------
//
// this function plays sound predefined in vector info
//
//---------------------------------------------------------------------------

void sfxPlayVectorSound(DBloodActor* actor, int vectorId) 
{
    const VECTORINFO_EXTRA* pVectorData = &gVectorInfoExtra[vectorId];
    sfxPlay3DSound(actor, Chance(0x5000) ? pVectorData->fireSound[0] : pVectorData->fireSound[1], -1, 0);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

int getSpriteMassBySize(DBloodActor* actor)
{
    auto pSprite = &actor->s();
    int mass = 0; int seqId = -1; int clipDist = pSprite->clipdist;
    if (!actor->hasX())
    {
        I_Error("getSpriteMassBySize: pSprite->hasX == false");
    }
    else if (actor->IsDudeActor()) 
    {
        switch (pSprite->type) 
        {
        case kDudePodMother: // fake dude, no seq
            break;
        case kDudeModernCustom:
        case kDudeModernCustomBurning:
            seqId = actor->x().data2;
            clipDist = actor->genDudeExtra.initVals[2];
            break;
        default:
            seqId = getDudeInfo(pSprite->type)->seqStartID;
            break;
        }
    } 
    else  
    {
        seqId = seqGetID(actor);
    }

    SPRITEMASS* cached = &actor->spriteMass;
    if (((seqId >= 0 && seqId == cached->seqId) || pSprite->picnum == cached->picnum) && pSprite->xrepeat == cached->xrepeat &&
        pSprite->yrepeat == cached->yrepeat && clipDist == cached->clipdist) 
    {
        return cached->mass;
    }

    int picnum = pSprite->picnum;
    int massDiv = 30; 
    int addMul = 2; 
    int subMul = 2;

    if (seqId >= 0) 
    {
        auto pSeq = getSequence(seqId);
        if (pSeq)
        {
            picnum = seqGetTile(&pSeq->frames[0]);
        } 
        else
            picnum = pSprite->picnum;
    }

    clipDist = ClipLow(pSprite->clipdist, 1);
    int x = tileWidth(picnum);        
    int y = tileHeight(picnum);
    int xrepeat = pSprite->xrepeat; 	
    int yrepeat = pSprite->yrepeat;

    // take surface type into account
    switch (tileGetSurfType(pSprite->picnum)) 
    {
        case 1:  massDiv = 16; break; // stone
        case 2:  massDiv = 18; break; // metal
        case 3:  massDiv = 21; break; // wood
        case 4:  massDiv = 25; break; // flesh
        case 5:  massDiv = 28; break; // water
        case 6:  massDiv = 26; break; // dirt
        case 7:  massDiv = 27; break; // clay
        case 8:  massDiv = 35; break; // snow
        case 9:  massDiv = 22; break; // ice
        case 10: massDiv = 37; break; // leaves
        case 11: massDiv = 33; break; // cloth
        case 12: massDiv = 36; break; // plant
        case 13: massDiv = 24; break; // goo
        case 14: massDiv = 23; break; // lava
    }

    mass = ((x + y) * (clipDist / 2)) / massDiv;

    if (xrepeat > 64) mass += ((xrepeat - 64) * addMul);
    else if (xrepeat < 64 && mass > 0) 
    {
        for (int i = 64 - xrepeat; i > 0; i--) 
        {
            if ((mass -= subMul) <= 100 && subMul-- <= 1) 
            {
                mass -= i;
                break;
            }
        }
    }

    if (yrepeat > 64) mass += ((yrepeat - 64) * addMul);
    else if (yrepeat < 64 && mass > 0) 
    {
        for (int i = 64 - yrepeat; i > 0; i--) 
        {
            if ((mass -= subMul) <= 100 && subMul-- <= 1) 
            {
                mass -= i;
                break;
            }
        }
    }

    if (mass <= 0) cached->mass = 1 + Random(10);
    else cached->mass = ClipRange(mass, 1, 65535);

    cached->airVel = ClipRange(400 - cached->mass, 32, 400);
    cached->fraction = ClipRange(60000 - (cached->mass << 7), 8192, 60000);

    cached->xrepeat = pSprite->xrepeat;             
    cached->yrepeat = pSprite->yrepeat;
    cached->picnum = pSprite->picnum;              
    cached->seqId = seqId;
    cached->clipdist = pSprite->clipdist;

    return cached->mass;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

static int debrisGetIndex(DBloodActor* actor) 
{
    if (!actor->hasX() || actor->x().physAttr == 0)
        return -1;

    for (int i = 0; i < gPhysSpritesCount; i++) 
    {
        if (gPhysSpritesList[i] != actor) continue;
        return i;
    }

    return -1;
}

int debrisGetFreeIndex(void) 
{
    for (int i = 0; i < kMaxSuperXSprites; i++) 
    {
        if (gPhysSpritesList[i] == nullptr) return i;
        auto spr = &gPhysSpritesList[i]->s();
        if (spr->statnum == kStatFree) return i;

        else if ((spr->flags & kHitagFree) || !gPhysSpritesList[i]->hasX()) return i;
        else if (gPhysSpritesList[i]->x().physAttr == 0) return i;
    }

    return -1;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void debrisConcuss(DBloodActor* owneractor, int listIndex, int x, int y, int z, int dmg)
{
    DBloodActor* actor = gPhysSpritesList[listIndex];
    if (actor != nullptr && actor->hasX())
    {
        spritetype* pSprite = &actor->s();
        int dx = pSprite->pos.X - x; int dy = pSprite->y - y; int dz = (pSprite->z - z) >> 4;
        dmg = scale(0x40000, dmg, 0x40000 + dx * dx + dy * dy + dz * dz);
        bool thing = (pSprite->type >= kThingBase && pSprite->type < kThingMax);
        int size = (tileWidth(pSprite->picnum) * pSprite->xrepeat * tileHeight(pSprite->picnum) * pSprite->yrepeat) >> 1;
        if (actor->x().physAttr & kPhysDebrisExplode) 
        {
            if (actor->spriteMass.mass > 0) 
            {
                int t = scale(dmg, size, actor->spriteMass.mass);

                actor->xvel += MulScale(t, dx, 16);
                actor->yvel += MulScale(t, dy, 16);
                actor->zvel += MulScale(t, dz, 16);
            }

            if (thing)
                pSprite->statnum = kStatThing; // temporary change statnum property
        }

        actDamageSprite(owneractor, actor, kDamageExplode, dmg);
        
        if (thing)
            pSprite->statnum = kStatDecoration; // return statnum property back
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void debrisBubble(DBloodActor* actor)
{
    spritetype* pSprite = &actor->s();
 
    int top, bottom;
    GetSpriteExtents(pSprite, &top, &bottom);
    for (unsigned int i = 0; i < 1 + Random(5); i++) {
        
        int nDist = (pSprite->xrepeat * (tileWidth(pSprite->picnum) >> 1)) >> 2;
        int nAngle = Random(2048);
        int x = pSprite->pos.X + MulScale(nDist, Cos(nAngle), 30);
        int y = pSprite->y + MulScale(nDist, Sin(nAngle), 30);
        int z = bottom - Random(bottom - top);
        auto pFX = gFX.fxSpawnActor((FX_ID)(FX_23 + Random(3)), pSprite->sector(), x, y, z, 0);
        if (pFX) {
            pFX->xvel = actor->xvel + Random2(0x1aaaa);
            pFX->yvel = actor->yvel + Random2(0x1aaaa);
            pFX->zvel = actor->zvel + Random2(0x1aaaa);
        }

    }
    
    if (Chance(0x2000))
        evPostActor(actor, 0, kCallbackEnemeyBubble);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void debrisMove(int listIndex)
{
    DBloodActor* actor = gPhysSpritesList[listIndex];
    XSPRITE* pXSprite = &actor->x();
    spritetype* pSprite = &actor->s();   
    auto pSector = pSprite->sector();

    if (!actor->hasX() || !pSector)
    {
        gPhysSpritesList[listIndex] = nullptr;
        return;
    }

    int top, bottom;
    GetActorExtents(actor, &top, &bottom);

    Collision moveHit;
    moveHit.setNone();
    int floorDist = (bottom - pSprite->z) >> 2;
    int ceilDist = (pSprite->z - top) >> 2;
    int clipDist = pSprite->clipdist << 2;
    int mass = actor->spriteMass.mass;

    bool uwater = false;
    int tmpFraction = actor->spriteMass.fraction;
    if (pSector->hasX() && pSector->xs().Underwater) 
    {
        tmpFraction >>= 1;
        uwater = true;
    }

    if (actor->xvel || actor->yvel) 
    {

        auto oldcstat = pSprite->cstat;
        pSprite->cstat &= ~(CSTAT_SPRITE_BLOCK | CSTAT_SPRITE_BLOCK_HITSCAN);

        ClipMove(pSprite->pos, &pSector, actor->xvel >> 12,
            actor->yvel >> 12, clipDist, ceilDist, floorDist, CLIPMASK0, moveHit);
        actor->hit.hit = moveHit;

        pSprite->cstat = oldcstat;
        if (pSprite->sector() != pSector) 
        {
            if (!pSector) return;
            else ChangeActorSect(actor, pSector);
        }

        if (pSector->type >= kSectorPath && pSector->type <= kSectorRotate) 
        {
            auto pSector2 = pSector;
            if (pushmove(&pSprite->pos, &pSector2, clipDist, ceilDist, floorDist, CLIPMASK0) != -1)
                pSector = pSector2;
        }

        if (actor->hit.hit.type == kHitWall)
        {
            moveHit = actor->hit.hit;
            actWallBounceVector(&actor->xvel, &actor->yvel, moveHit.hitWall, tmpFraction);
        }

    } 
    else if (!FindSector(pSprite->pos.X, pSprite->y, pSprite->z, &pSector)) 
    {
        return;
    }

    if (pSprite->sector() != pSector)
    {
        assert(pSector);
        ChangeActorSect(actor, pSector);
        pSector = pSprite->sector();
    }

    if (pSector->hasX())
        uwater = pSector->xs().Underwater;

    if (actor->zvel)
        pSprite->z += actor->zvel >> 8;

    int ceilZ, floorZ;
    Collision ceilColl, floorColl;
    GetZRange(actor, &ceilZ, &ceilColl, &floorZ, &floorColl, clipDist, CLIPMASK0, PARALLAXCLIP_CEILING | PARALLAXCLIP_FLOOR);
    GetSpriteExtents(pSprite, &top, &bottom);

    if ((pXSprite->physAttr & kPhysDebrisSwim) && uwater) 
    {
        int vc = 0;
        int cz = getceilzofslopeptr(pSector, pSprite->pos.X, pSprite->y);
        int fz = getflorzofslopeptr(pSector, pSprite->pos.X, pSprite->y);
        int div = ClipLow(bottom - top, 1);

        if (pSector->lowerLink) cz += (cz < 0) ? 0x500 : -0x500;
        if (top > cz && (!(pXSprite->physAttr & kPhysDebrisFloat) || fz <= bottom << 2))
            actor->zvel -= DivScale((bottom - ceilZ) >> 6, mass, 8);

        if (fz < bottom)
            vc = 58254 + ((bottom - fz) * -80099) / div;

        if (vc) 
        {
            pSprite->z += ((vc << 2) >> 1) >> 8;
            actor->zvel += vc;
        }

    }
    else if ((pXSprite->physAttr & kPhysGravity) && bottom < floorZ) 
    {
        pSprite->z += 455;
        actor->zvel += 58254;

    }

    int i;
    if ((i = CheckLink(actor)) != 0)
    {
        GetZRange(actor, &ceilZ, &ceilColl, &floorZ, &floorColl, clipDist, CLIPMASK0, PARALLAXCLIP_CEILING | PARALLAXCLIP_FLOOR);
        if (!(pSprite->cstat & CSTAT_SPRITE_INVISIBLE))
        {
            switch (i)
            {
            case kMarkerUpWater:
            case kMarkerUpGoo:
                int pitch = (150000 - (actor->spriteMass.mass << 9)) + Random3(8192);
                sfxPlay3DSoundCP(actor, 720, -1, 0, pitch, 75 - Random(40));
                if (!spriteIsUnderwater(actor))
                {
                    evKillActor(actor, kCallbackEnemeyBubble);
                }
                else
                {
                        evPostActor(actor, 0, kCallbackEnemeyBubble);
                    for (int i = 2; i <= 5; i++)
                    {
                            if (Chance(0x5000 * i))
                                evPostActor(actor, Random(5), kCallbackEnemeyBubble);
                    }
                }
                break;
            }
        }
    }

    GetActorExtents(actor, &top, &bottom);

    if (floorZ <= bottom) {

        actor->hit.florhit = floorColl;
        int v30 = actor->zvel - pSprite->sector()->velFloor;

        if (v30 > 0) 
        {
            pXSprite->physAttr |= kPhysFalling;
            actFloorBounceVector(&actor->xvel, &actor->yvel, &v30, pSprite->sector(), tmpFraction);
            actor->zvel = v30;

            if (abs(actor->zvel) < 0x10000)
            {
                actor->zvel = pSprite->sector()->velFloor;
                pXSprite->physAttr &= ~kPhysFalling;
            }

            moveHit = floorColl;
            DBloodActor* pFX = NULL, *pFX2 = NULL;
            switch (tileGetSurfType(floorColl)) 
            {
            case kSurfLava:
                if ((pFX = gFX.fxSpawnActor(FX_10, pSprite->sector(), pSprite->pos.X, pSprite->y, floorZ, 0)) == NULL) break;
                for (i = 0; i < 7; i++) 
                {
                    if ((pFX2 = gFX.fxSpawnActor(FX_14, pFX->spr.sector(), pFX->spr.pos.X, pFX->spr.y, pFX->spr.z, 0)) == NULL) continue;
                    pFX2->xvel = Random2(0x6aaaa);
                    pFX2->yvel = Random2(0x6aaaa);
                    pFX2->zvel = -(int)Random(0xd5555);
                }
                break;
            case kSurfWater:
                gFX.fxSpawnActor(FX_9, pSprite->sector(), pSprite->pos.X, pSprite->y, floorZ, 0);
                break;
            }

        }
        else if (actor->zvel == 0) 
        {
            pXSprite->physAttr &= ~kPhysFalling;
        }
    }
    else 
    {
        actor->hit.florhit.setNone();
        if (pXSprite->physAttr & kPhysGravity)
            pXSprite->physAttr |= kPhysFalling;
    }

    if (top <= ceilZ) 
    {
        actor->hit.ceilhit = moveHit = ceilColl;
        pSprite->z += ClipLow(ceilZ - top, 0);
        if (actor->zvel <= 0 && (pXSprite->physAttr & kPhysFalling))
            actor->zvel = MulScale(-actor->zvel, 0x2000, 16);

    }
    else 
    {
        actor->hit.ceilhit.setNone();
        GetActorExtents(actor, &top, &bottom);
    }

    if (moveHit.type != kHitNone && pXSprite->Impact && !pXSprite->locked && !pXSprite->isTriggered && (pXSprite->state == pXSprite->restState || pXSprite->Interrutable)) {
        if (pSprite->type >= kThingBase && pSprite->type < kThingMax)
            ChangeActorStat(actor, kStatThing);

        trTriggerSprite(actor, kCmdToggle);
    }

    if (!actor->xvel && !actor->yvel) return;
    else if (floorColl.type == kHitSprite)
    {

        if ((floorColl.actor()->spr.cstat & CSTAT_SPRITE_ALIGNMENT_MASK) == 0)
        {
            actor->xvel += MulScale(4, pSprite->pos.X - floorColl.actor()->spr.pos.X, 2);
            actor->yvel += MulScale(4, pSprite->y - floorColl.actor()->spr.y, 2);
            return;
        }
    }

    pXSprite->height = ClipLow(floorZ - bottom, 0) >> 8;
    if (uwater || pXSprite->height >= 0x100)
        return;

    int nDrag = 0x2a00;
    if (pXSprite->height > 0)
        nDrag -= scale(nDrag, pXSprite->height, 0x100);

    actor->xvel -= mulscale16r(actor->xvel, nDrag);
    actor->yvel -= mulscale16r(actor->yvel, nDrag);
    if (approxDist(actor->xvel, actor->yvel) < 0x1000)
        actor->xvel = actor->yvel = 0;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool ceilIsTooLow(DBloodActor* actor) 
{
    if (actor != nullptr) 
    {
        sectortype* pSector = actor->spr.sector();
        int a = pSector->ceilingz - pSector->floorz;
        int top, bottom;
        GetActorExtents(actor, &top, &bottom);
        int b = top - bottom;
        if (a > b) return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiSetGenIdleState(DBloodActor* actor)
{
    switch (actor->spr.type) 
    {
    case kDudeModernCustom:
    case kDudeModernCustomBurning:
        aiGenDudeNewState(actor, &genIdle);
        break;
    default:
        aiNewState(actor, &genIdle);
        break;
    }
}

//---------------------------------------------------------------------------
//
// this function stops wind on all TX sectors affected by WindGen after it goes off state.
//
//---------------------------------------------------------------------------

void windGenStopWindOnSectors(DBloodActor* sourceactor) 
{
    spritetype* pSource = &sourceactor->s();
    auto pXSource = &sourceactor->x();
    if (pXSource->txID <= 0 && pSource->sector()->hasX())
    {
        pSource->sector()->xs().windVel = 0;
        return;
    }

    for (int i = bucketHead[pXSource->txID]; i < bucketHead[pXSource->txID + 1]; i++) 
    {
        if (!rxBucket[i].isSector()) continue;
        auto pSector = rxBucket[i].sector();
        XSECTOR* pXSector = &pSector->xs();

        if ((pXSector->state == 1 && !pXSector->windAlways)
            || ((pSource->flags & kModernTypeFlag1) && !(pSource->flags & kModernTypeFlag2))) 
        {
                pXSector->windVel = 0;
        }
    }
    
    // check redirected TX buckets
    int rx = -1; 
    DBloodActor* pXRedir = nullptr;
    while ((pXRedir = evrListRedirectors(OBJ_SPRITE, nullptr, nullptr, sourceactor, pXRedir, &rx)) != nullptr)
    {
        for (int i = bucketHead[rx]; i < bucketHead[rx + 1]; i++) 
        {
            if (!rxBucket[i].isSector()) continue;
            auto pSector = rxBucket[i].sector();
            XSECTOR* pXSector = &pSector->xs();

            if ((pXSector->state == 1 && !pXSector->windAlways) || (pSource->flags & kModernTypeFlag2))
                pXSector->windVel = 0;
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlStartScene(DBloodActor* sourceactor, PLAYER* pPlayer, bool force) 
{
    auto pXSource = &sourceactor->x();
    TRPLAYERCTRL* pCtrl = &gPlayerCtrl[pPlayer->nPlayer];

    if (pCtrl->qavScene.initiator != nullptr && !force) return;
    
    QAV* pQav = playerQavSceneLoad(pXSource->data2);
    if (pQav != nullptr) 
    {
        // save current weapon
        pXSource->dropMsg = pPlayer->curWeapon;

        auto initiator = pCtrl->qavScene.initiator;
        if (initiator != nullptr && initiator != sourceactor && initiator->hasX())
            pXSource->dropMsg = initiator->x().dropMsg;

        if (initiator == nullptr)
            WeaponLower(pPlayer);

        pXSource->sysData1 = ClipLow((pQav->duration * pXSource->waitTime) / 4, 0); // how many times animation should be played

        pCtrl->qavScene.initiator = sourceactor;
        pCtrl->qavScene.qavResrc = pQav;
        pCtrl->qavScene.dummy = -1;

        //pCtrl->qavScene.qavResrc->Preload();

        pPlayer->sceneQav = pXSource->data2;
        pPlayer->weaponTimer = pCtrl->qavScene.qavResrc->duration;
        pPlayer->qavCallback = (pXSource->data3 > 0) ? ClipRange(pXSource->data3 - 1, 0, 32) : -1;
        pPlayer->qavLoop = false;
        pPlayer->qavLastTick = I_GetTime(pCtrl->qavScene.qavResrc->ticrate);
        pPlayer->qavTimer = pCtrl->qavScene.qavResrc->duration;
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlStopScene(PLAYER* pPlayer)
{
    TRPLAYERCTRL* pCtrl = &gPlayerCtrl[pPlayer->nPlayer];
    auto initiator = pCtrl->qavScene.initiator;
    XSPRITE* pXSource = nullptr;
    if (initiator->hasX())
    {
        pXSource = &initiator->x();
        pXSource->sysData1 = 0;
    }

    if (pCtrl->qavScene.initiator != nullptr) 
    {
        pCtrl->qavScene.initiator = nullptr;
        pCtrl->qavScene.qavResrc = nullptr;
        pPlayer->sceneQav = -1;

        // restore weapon
        if (pPlayer->pXSprite->health > 0) 
        {
            int oldWeapon = (pXSource && pXSource->dropMsg != 0) ? pXSource->dropMsg : 1;
            pPlayer->newWeapon = pPlayer->curWeapon = oldWeapon;
            WeaponRaise(pPlayer);
        }
    }

}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlLink(DBloodActor* sourceactor, PLAYER* pPlayer, bool checkCondition) 
{
    auto pXSource = &sourceactor->x();
    // save player's sprite index to let the tracking condition know it after savegame loading...
    sourceactor->prevmarker             = pPlayer->actor;
    
    pPlayer->pXSprite->txID             = pXSource->txID;
    pPlayer->pXSprite->command          = kCmdToggle;
    pPlayer->pXSprite->triggerOn        = pXSource->triggerOn;
    pPlayer->pXSprite->triggerOff       = pXSource->triggerOff;
    pPlayer->pXSprite->busyTime         = pXSource->busyTime;
    pPlayer->pXSprite->waitTime         = pXSource->waitTime;
    pPlayer->pXSprite->restState        = pXSource->restState;

    pPlayer->pXSprite->Push             = pXSource->Push;
    pPlayer->pXSprite->Impact           = pXSource->Impact;
    pPlayer->pXSprite->Vector           = pXSource->Vector;
    pPlayer->pXSprite->Touch            = pXSource->Touch;
    pPlayer->pXSprite->Sight            = pXSource->Sight;
    pPlayer->pXSprite->Proximity        = pXSource->Proximity;

    pPlayer->pXSprite->Decoupled        = pXSource->Decoupled;
    pPlayer->pXSprite->Interrutable     = pXSource->Interrutable;
    pPlayer->pXSprite->DudeLockout      = pXSource->DudeLockout;

    pPlayer->pXSprite->data1            = pXSource->data1;
    pPlayer->pXSprite->data2            = pXSource->data2;
    pPlayer->pXSprite->data3            = pXSource->data3;
    pPlayer->pXSprite->data4            = pXSource->data4;

    pPlayer->pXSprite->key              = pXSource->key;
    pPlayer->pXSprite->dropMsg          = pXSource->dropMsg;

    // let's check if there is tracking condition expecting objects with this TX id
    if (checkCondition && pXSource->txID >= kChannelUser) 
    {
        for (int i = 0; i < gTrackingCondsCount; i++) 
        {
            TRCONDITION* pCond = &gCondition[i];
            if (pCond->actor->x().rxID != pXSource->txID)
                continue;
                
            // search for player control sprite and replace it with actual player sprite
            for (unsigned k = 0; k < pCond->length; k++) 
            {
                if (!pCond->obj[k].obj.isActor() || pCond->obj[k].obj.actor() != sourceactor) continue;
                pCond->obj[k].obj = EventObject(pPlayer->actor);
                pCond->obj[k].cmd = (uint8_t)pPlayer->pXSprite->command;
                break;
            }
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlSetRace(int value, PLAYER* pPlayer) 
{
    playerSetRace(pPlayer, value);
    switch (pPlayer->lifeMode) 
    {
        case kModeHuman:
        case kModeBeast:
            playerSizeReset(pPlayer);
            break;
        case kModeHumanShrink:
            playerSizeShrink(pPlayer, 2);
            break;
        case kModeHumanGrown:
            playerSizeGrow(pPlayer, 2);
            break;
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlSetMoveSpeed(int value, PLAYER* pPlayer) 
{
    int speed = ClipRange(value, 0, 500);
    for (int i = 0; i < kModeMax; i++) 
    {
        for (int a = 0; a < kPostureMax; a++) 
        {
            POSTURE* curPosture = &pPlayer->pPosture[i][a]; POSTURE* defPosture = &gPostureDefaults[i][a];
            curPosture->frontAccel = (defPosture->frontAccel * speed) / kPercFull;
            curPosture->sideAccel = (defPosture->sideAccel * speed) / kPercFull;
            curPosture->backAccel = (defPosture->backAccel * speed) / kPercFull;
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlSetJumpHeight(int value, PLAYER* pPlayer) 
{
    int jump = ClipRange(value, 0, 500);
    for (int i = 0; i < kModeMax; i++) 
    {
        POSTURE* curPosture = &pPlayer->pPosture[i][kPostureStand]; POSTURE* defPosture = &gPostureDefaults[i][kPostureStand];
        curPosture->normalJumpZ = (defPosture->normalJumpZ * jump) / kPercFull;
        curPosture->pwupJumpZ = (defPosture->pwupJumpZ * jump) / kPercFull;
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlSetScreenEffect(int value, int timeval, PLAYER* pPlayer) 
{
    int eff = ClipLow(value, 0); 
    int time = (eff > 0) ? timeval : 0;
    switch (eff) {
        case 0: // clear all
        case 1: // tilting
            pPlayer->tiltEffect = ClipRange(time, 0, 220);
            if (eff) break;
            [[fallthrough]];
        case 2: // pain
            pPlayer->painEffect = ClipRange(time, 0, 2048);
            if (eff) break;
            [[fallthrough]];
        case 3: // blind
            pPlayer->blindEffect = ClipRange(time, 0, 2048);
            if (eff) break;
            [[fallthrough]];
        case 4: // pickup
            pPlayer->pickupEffect = ClipRange(time, 0, 2048);
            if (eff) break;
            [[fallthrough]];
        case 5: // quakeEffect
            pPlayer->quakeEffect = ClipRange(time, 0, 2048);
            if (eff) break;
            [[fallthrough]];
        case 6: // visibility
            pPlayer->visibility = ClipRange(time, 0, 2048);
            if (eff) break;
            [[fallthrough]];
        case 7: // delirium
            pPlayer->pwUpTime[kPwUpDeliriumShroom] = ClipHigh(time << 1, 432000);
            break;
    }

}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlSetLookAngle(int value, PLAYER* pPlayer)
{
    double const upAngle = 289; 
    double const downAngle = -347;
    double const lookStepUp = 4.0 * upAngle / 60.0;
    double const lookStepDown = -4.0 * downAngle / 60.0;
    double const look = value << 5;
    double adjustment;

    if (look > 0) 
    {
        adjustment = min(MulScaleF(lookStepUp, look, 8), upAngle);
    }
    else if (look < 0) 
    {
        adjustment = -max(MulScaleF(lookStepDown, abs(look), 8), downAngle);
    }
    else 
    {
        adjustment = 0;
    }

    pPlayer->horizon.settarget(100. * tan(adjustment * pi::pi() / 1024.));
    pPlayer->horizon.lockinput();
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlEraseStuff(int value, PLAYER* pPlayer)
{
    
    switch (value) 
    {
        case 0: // erase all
            [[fallthrough]];
        case 1: // erase weapons
            WeaponLower(pPlayer);

            for (int i = 0; i < 14; i++) {
                pPlayer->hasWeapon[i] = false;
                // also erase ammo
                if (i < 12) pPlayer->ammoCount[i] = 0;
            }

            pPlayer->hasWeapon[1] = true;
            pPlayer->curWeapon = kWeapNone;
            pPlayer->nextWeapon = kWeapPitchFork;

            WeaponRaise(pPlayer);
            if (value) break;
            [[fallthrough]];
        case 2: // erase all armor
            for (int i = 0; i < 3; i++) pPlayer->armor[i] = 0;
            if (value) break;
            [[fallthrough]];
        case 3: // erase all pack items
            for (int i = 0; i < 5; i++) {
                pPlayer->packSlots[i].isActive = false;
                pPlayer->packSlots[i].curAmount = 0;
            }
            pPlayer->packItemId = -1;
            if (value) break;
            [[fallthrough]];
        case 4: // erase all keys
            for (int i = 0; i < 8; i++) pPlayer->hasKey[i] = false;
            if (value) break;
            [[fallthrough]];
        case 5: // erase powerups
            for (int i = 0; i < kMaxPowerUps; i++) pPlayer->pwUpTime[i] = 0;
            break;
    }

}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlGiveStuff(int data2, int weapon, int data4, PLAYER* pPlayer, TRPLAYERCTRL* pCtrl) 
{
    switch (data2) 
    {
        case 1: // give N weapon and default ammo for it
        case 2: // give just N ammo for selected weapon
            if (weapon <= 0 || weapon > 13) 
            {
                Printf(PRINT_HIGH, "Weapon #%d is out of a weapons range!", weapon);
                break;
            } else if (data2 == 2 && data4 == 0) 
            {
                Printf(PRINT_HIGH, "Zero ammo for weapon #%d is specified!", weapon);
                break;
            }
            switch (weapon) 
            {
                case kWeapProximity: // remote bomb 
                case kWeapRemote: // prox bomb
                    pPlayer->hasWeapon[weapon] = true;
                    weapon--;
                    pPlayer->ammoCount[weapon] = ClipHigh(pPlayer->ammoCount[weapon] + ((data2 == 2) ? data4 : 1), gAmmoInfo[weapon].max);
                    weapon++;
                    break;
                default:
                    for (int i = 0; i < 11; i++) 
                    {
                        if (gWeaponItemData[i].type != weapon) continue;

                        const WEAPONITEMDATA* pWeaponData = &gWeaponItemData[i]; 
                        int nAmmoType = pWeaponData->ammoType;
                        switch (data2) {
                            case 1:
                                pPlayer->hasWeapon[weapon] = true;
                                if (pPlayer->ammoCount[nAmmoType] >= pWeaponData->count) break;
                                pPlayer->ammoCount[nAmmoType] = ClipHigh(pPlayer->ammoCount[nAmmoType] + pWeaponData->count, gAmmoInfo[nAmmoType].max);
                                break;
                            case 2:
                                pPlayer->ammoCount[nAmmoType] = ClipHigh(pPlayer->ammoCount[nAmmoType] + data4, gAmmoInfo[nAmmoType].max);
                                break;
                        }
                        break;
                    }
                    break;
            }
            if (pPlayer->hasWeapon[weapon] && data4 == 0) // switch on it
            {
                pPlayer->nextWeapon = kWeapNone;

                if (pPlayer->sceneQav >= 0 && pCtrl->qavScene.initiator && pCtrl->qavScene.initiator->hasX())
                {
                    pCtrl->qavScene.initiator->x().dropMsg = weapon;
                } 
                else if (pPlayer->curWeapon != weapon) 
                {
                    pPlayer->newWeapon = weapon;
                    WeaponRaise(pPlayer);
                }
            }
            break;
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlUsePackItem(int data2, int data3, int data4, PLAYER* pPlayer, int evCmd)
{
    unsigned int invItem = data2 - 1;
    switch (evCmd) 
    {
        case kCmdOn:
            if (!pPlayer->packSlots[invItem].isActive) packUseItem(pPlayer, invItem);
            break;
        case kCmdOff:
            if (pPlayer->packSlots[invItem].isActive) packUseItem(pPlayer, invItem);
            break;
        default:
            packUseItem(pPlayer, invItem);
            break;
    }

    switch (data4) 
    {
        case 2: // both
        case 0: // switch on it
            if (pPlayer->packSlots[invItem].curAmount > 0) pPlayer->packItemId = invItem;
            if (!data4) break;
            [[fallthrough]];
        case 1: // force remove after use
            pPlayer->packSlots[invItem].isActive = false;
            pPlayer->packSlots[invItem].curAmount = 0;
            break;
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void trPlayerCtrlUsePowerup(DBloodActor* sourceactor, PLAYER* pPlayer, int evCmd)
{

    bool relative = (sourceactor->spr.flags & kModernTypeFlag1);

    int nPower = (kMinAllowedPowerup + sourceactor->x().data2) - 1;
    int nTime = ClipRange(abs(sourceactor->x().data3) * 100, -gPowerUpInfo[nPower].maxTime, gPowerUpInfo[nPower].maxTime);
    if (sourceactor->x().data3 < 0)
        nTime = -nTime;

    if (pPlayer->pwUpTime[nPower]) 
    {
       if (!relative && nTime <= 0)
           powerupDeactivate(pPlayer, nPower);
    }

    if (nTime != 0) 
    {
        if (pPlayer->pwUpTime[nPower] <= 0)
            powerupActivate(pPlayer, nPower);  // MUST activate first for powerups like kPwUpDeathMask
        
        // ...so we able to change time amount
        if (relative) pPlayer->pwUpTime[nPower] += nTime;
        else pPlayer->pwUpTime[nPower] = nTime;
    }

    if (pPlayer->pwUpTime[nPower] <= 0)
        powerupDeactivate(pPlayer, nPower);

    return;

}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void useObjResizer(DBloodActor* sourceactor, int targType, sectortype* targSect, walltype* targWall, DBloodActor* targetactor) 
{
    auto pXSource = &sourceactor->x();
    switch (targType) 
    {
    // for sectors
    case OBJ_SECTOR:
        if (!targSect) return;
        if (valueIsBetween(pXSource->data1, -1, 32767))
            targSect->floorxpan_ = (float)ClipRange(pXSource->data1, 0, 255);

        if (valueIsBetween(pXSource->data2, -1, 32767))
            targSect->floorypan_ = (float)ClipRange(pXSource->data2, 0, 255);

        if (valueIsBetween(pXSource->data3, -1, 32767))
            targSect->ceilingxpan_ = (float)ClipRange(pXSource->data3, 0, 255);

        if (valueIsBetween(pXSource->data4, -1, 65535))
            targSect->ceilingypan_ = (float)ClipRange(pXSource->data4, 0, 255);
        break;
    // for sprites
    case OBJ_SPRITE: 
    {
        bool fit = false;
        auto pTarget = &targetactor->s();
        // resize by seq scaling
        if (sourceactor->spr.flags & kModernTypeFlag1) 
        {
            if (valueIsBetween(pXSource->data1, -255, 32767))
            {
                int mulDiv = (valueIsBetween(pXSource->data2, 0, 257)) ? pXSource->data2 : 256;
                if (pXSource->data1 > 0) targetactor->x().scale = mulDiv * ClipHigh(pXSource->data1, 25);
                else if (pXSource->data1 < 0) targetactor->x().scale = mulDiv / ClipHigh(abs(pXSource->data1), 25);
                else targetactor->x().scale = 0;
                fit = true;
                }

        // resize by repeats
        }
        else 
        {
            if (valueIsBetween(pXSource->data1, -1, 32767)) 
            {
                pTarget->xrepeat = ClipRange(pXSource->data1, 0, 255);
                fit = true;
            }
            
            if (valueIsBetween(pXSource->data2, -1, 32767)) 
            {
                pTarget->yrepeat = ClipRange(pXSource->data2, 0, 255);
                fit = true;
            }
        }

        if (fit && (pTarget->type == kDudeModernCustom || pTarget->type == kDudeModernCustomBurning))
        {
            // request properties update for custom dude
            
            targetactor->genDudeExtra.updReq[kGenDudePropertySpriteSize] = true;
            targetactor->genDudeExtra.updReq[kGenDudePropertyAttack] = true;
            targetactor->genDudeExtra.updReq[kGenDudePropertyMass] = true;
            targetactor->genDudeExtra.updReq[kGenDudePropertyDmgScale] = true;
            evPostActor(targetactor, kGenDudeUpdTimeRate, kCallbackGenDudeUpdate);

        }

        if (valueIsBetween(pXSource->data3, -1, 32767))
            pTarget->xoffset = ClipRange(pXSource->data3, 0, 255);

        if (valueIsBetween(pXSource->data4, -1, 65535))
            pTarget->yoffset = ClipRange(pXSource->data4, 0, 255);
        break;
    }
    case OBJ_WALL:
        if (!targWall) return;
        if (valueIsBetween(pXSource->data1, -1, 32767))
            targWall->xrepeat = ClipRange(pXSource->data1, 0, 255);

        if (valueIsBetween(pXSource->data2, -1, 32767))
            targWall->yrepeat = ClipRange(pXSource->data2, 0, 255);

        if (valueIsBetween(pXSource->data3, -1, 32767))
            targWall->xpan_ = (float)ClipRange(pXSource->data3, 0, 255);

        if (valueIsBetween(pXSource->data4, -1, 65535))
            targWall->ypan_ = (float)ClipRange(pXSource->data4, 0, 255);
        break;
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void usePropertiesChanger(DBloodActor* sourceactor, int objType, sectortype* pSector, walltype* pWall, DBloodActor* targetactor)
{
    auto pXSource = &sourceactor->x();
    spritetype* pSource = &sourceactor->s();

    switch (objType) 
    {
        case OBJ_WALL: 
        {
            if (!pWall) return;

            // data3 = set wall hitag
            if (valueIsBetween(pXSource->data3, -1, 32767)) 
            {
                if ((pSource->flags & kModernTypeFlag1)) pWall->hitag |= pXSource->data3;
                else pWall->hitag = pXSource->data3;
            }

            // data4 = set wall cstat
            if (valueIsBetween(pXSource->data4, -1, 65535)) 
            {
                auto old = pWall->cstat;

                // set new cstat
                if ((pSource->flags & kModernTypeFlag1)) pWall->cstat |= EWallFlags::FromInt(pXSource->data4); // relative
                else pWall->cstat = EWallFlags::FromInt(pXSource->data4); // absolute

                // and hanlde exceptions
                pWall->cstat |= old & (CSTAT_WALL_BOTTOM_SWAP | CSTAT_WALL_ALIGN_BOTTOM | CSTAT_WALL_1WAY);
                pWall->cstat = (pWall->cstat & ~CSTAT_WALL_MOVE_MASK) | (old & CSTAT_WALL_MOVE_MASK);
#if 0
                // old code for reference. This does not look right.
                if (old & 0xc000) {

                    if (!(pWall->cstat & 0xc000))
                        pWall->cstat |= 0xc000; // kWallMoveMask

                    if ((old & 0x0) && !(pWall->cstat & 0x0)) pWall->cstat |= 0x0; // kWallMoveNone
                    else if ((old & 0x4000) && !(pWall->cstat & 0x4000)) pWall->cstat |= 0x4000; // kWallMoveForward
                    else if ((old & 0x8000) && !(pWall->cstat & CSTAT_SPRITE_INVISIBLE)) pWall->cstat |= CSTAT_SPRITE_INVISIBLE; // kWallMoveBackward

                }
#endif
            }
        }
        break;
        case OBJ_SPRITE: 
        {
            spritetype* pSprite = &targetactor->s(); 
            XSPRITE* pXSprite = &targetactor->x();
            bool thing2debris = false;
            int old = -1;

            // data3 = set sprite hitag
            if (valueIsBetween(pXSource->data3, -1, 32767)) 
            {
                old = pSprite->flags;

                // set new hitag
                if ((pSource->flags & kModernTypeFlag1)) pSprite->flags = pSource->flags |= pXSource->data3; // relative
                else pSprite->flags = pXSource->data3;  // absolute

                // and handle exceptions
                if ((old & kHitagFree) && !(pSprite->flags & kHitagFree)) pSprite->flags |= kHitagFree;
                if ((old & kHitagRespawn) && !(pSprite->flags & kHitagRespawn)) pSprite->flags |= kHitagRespawn;

                // prepare things for different (debris) physics.
                if (pSprite->statnum == kStatThing && debrisGetFreeIndex() >= 0) thing2debris = true;

            }

            // data2 = sprite physics settings
            if (valueIsBetween(pXSource->data2, -1, 32767) || thing2debris) 
            {
                switch (pSprite->statnum) 
                {
                case kStatDude: // dudes already treating in game
                case kStatFree:
                case kStatMarker:
                case kStatPathMarker:
                    break;
                default:
                    // store physics attributes in xsprite to avoid setting hitag for modern types!
                    int flags = (pXSprite->physAttr != 0) ? pXSprite->physAttr : 0;
                    int oldFlags = flags;

                    if (thing2debris) 
                    {
                        // converting thing to debris
                        if ((pSprite->flags & kPhysMove) != 0) flags |= kPhysMove;
                        else flags &= ~kPhysMove;

                        if ((pSprite->flags & kPhysGravity) != 0) flags |= (kPhysGravity | kPhysFalling);
                        else flags &= ~(kPhysGravity | kPhysFalling);

                        pSprite->flags &= ~(kPhysMove | kPhysGravity | kPhysFalling);
                        targetactor->xvel = targetactor->yvel = targetactor->zvel = 0;
                        pXSprite->restState = pXSprite->state;

                    } 
                    else
                    {
                        // WTF is this?!?
                        char digits[6] = {};
                        snprintf(digits, 6, "%d", pXSource->data2);
                        for (unsigned int i = 0; i < sizeof(digits); i++)
                            digits[i] = (digits[i] >= 48 && digits[i] <= 57) ? (digits[i] - 57) + 9 : 0;

                        // first digit of data2: set main physics attributes
                        switch (digits[0])
                        {
                            case 0:
                                flags &= ~kPhysMove;
                                flags &= ~(kPhysGravity | kPhysFalling);
                                break;
                            case 1:
                                flags |= kPhysMove;
                                flags &= ~(kPhysGravity | kPhysFalling);
                                break;
                            case 2:
                                flags &= ~kPhysMove;
                                flags |= (kPhysGravity | kPhysFalling);
                                break;
                            case 3:
                                flags |= kPhysMove;
                                flags |= (kPhysGravity | kPhysFalling);
                                break;
                        }

                        // second digit of data2: touch physics flags
                        switch (digits[1])
                        {
                            case 0:
                                flags &= ~kPhysDebrisTouch;
                                break;
                            case 1:
                                flags |= kPhysDebrisTouch;
                                break;
                        }

                        // third digit of data2: weapon physics flags
                        switch (digits[2])
                        {
                            case 0:
                                flags &= ~kPhysDebrisVector;
                                flags &= ~kPhysDebrisExplode;
                                break;
                            case 1:
                                flags |= kPhysDebrisVector;
                                flags &= ~kPhysDebrisExplode;
                                break;
                            case 2:
                                flags &= ~kPhysDebrisVector;
                                flags |= kPhysDebrisExplode;
                                break;
                            case 3:
                                flags |= kPhysDebrisVector;
                                flags |= kPhysDebrisExplode;
                                break;
                        }

                        // fourth digit of data2: swimming / flying physics flags
                        switch (digits[3])
                        {
                            case 0:
                                flags &= ~kPhysDebrisSwim;
                                flags &= ~kPhysDebrisFly;
                                flags &= ~kPhysDebrisFloat;
                                break;
                            case 1:
                                flags |= kPhysDebrisSwim;
                                flags &= ~kPhysDebrisFly;
                                flags &= ~kPhysDebrisFloat;
                                break;
                            case 2:
                                flags |= kPhysDebrisSwim;
                                flags |= kPhysDebrisFloat;
                                flags &= ~kPhysDebrisFly;
                                break;
                            case 3:
                                flags |= kPhysDebrisFly;
                                flags &= ~kPhysDebrisSwim;
                                flags &= ~kPhysDebrisFloat;
                                break;
                            case 4:
                                flags |= kPhysDebrisFly;
                                flags |= kPhysDebrisFloat;
                                flags &= ~kPhysDebrisSwim;
                                break;
                            case 5:
                                flags |= kPhysDebrisSwim;
                                flags |= kPhysDebrisFly;
                                flags &= ~kPhysDebrisFloat;
                                break;
                            case 6:
                                flags |= kPhysDebrisSwim;
                                flags |= kPhysDebrisFly;
                                flags |= kPhysDebrisFloat;
                                break;
                    }
                    }

                    int nIndex = debrisGetIndex(targetactor); // check if there is no sprite in list

                    // adding physics sprite in list
                    if ((flags & kPhysGravity) != 0 || (flags & kPhysMove) != 0) 
                    {

                        if (oldFlags == 0)
                            targetactor->xvel = targetactor->yvel = targetactor->zvel = 0;

                        if (nIndex != -1) 
                        {
                            pXSprite->physAttr = flags; // just update physics attributes
                        } 
                        else if ((nIndex = debrisGetFreeIndex()) < 0) 
                        {
                            viewSetSystemMessage("Max (%d) Physics affected sprites reached!", kMaxSuperXSprites);
                        } 
                        else 
                        {
                            pXSprite->physAttr = flags; // update physics attributes

                            // allow things to became debris, so they use different physics...
                            if (pSprite->statnum == kStatThing) ChangeActorStat(targetactor, 0);

                            // set random goal ang for swimming so they start turning
                            if ((flags & kPhysDebrisSwim) && !targetactor->xvel && !targetactor->yvel && !targetactor->zvel)
                                pXSprite->goalAng = (pSprite->ang + Random3(kAng45)) & 2047;
                            
                            if (pXSprite->physAttr & kPhysDebrisVector)
                                pSprite->cstat |= CSTAT_SPRITE_BLOCK_HITSCAN;

                            gPhysSpritesList[nIndex] = targetactor;
                            if (nIndex >= gPhysSpritesCount) gPhysSpritesCount++;
                            getSpriteMassBySize(targetactor); // create physics cache

                        }

                    // removing physics from sprite in list (don't remove sprite from list)
                    }
                    else if (nIndex != -1) 
                    {

                        pXSprite->physAttr = flags;
                        targetactor->xvel = targetactor->yvel = targetactor->zvel = 0;
                        if (pSprite->lotag >= kThingBase && pSprite->lotag < kThingMax)
                            ChangeActorStat(targetactor, kStatThing);  // if it was a thing - restore statnum
                    }
                    break;
                }
            }

            // data4 = sprite cstat
            if (valueIsBetween(pXSource->data4, -1, 65535)) 
            {
                auto old = pSprite->cstat;

                // set new cstat
                if ((pSource->flags & kModernTypeFlag1)) pSprite->cstat |= ESpriteFlags::FromInt(pXSource->data4); // relative
                else pSprite->cstat = ESpriteFlags::FromInt(pXSource->data4 & 0xffff); // absolute

                // and handle exceptions
                if ((old & CSTAT_SPRITE_BLOOD_BIT1)) pSprite->cstat |= CSTAT_SPRITE_BLOOD_BIT1; //kSpritePushable
                if ((old & CSTAT_SPRITE_YCENTER)) pSprite->cstat |= CSTAT_SPRITE_YCENTER;

                pSprite->cstat |= (old & CSTAT_SPRITE_MOVE_MASK);
#if 0
                // looks very broken.
                if (old & 0x6000) 
                {
                    if (!(pSprite->cstat & 0x6000))
                        pSprite->cstat |= 0x6000; // kSpriteMoveMask

                    if ((old & 0x0) && !(pSprite->cstat & 0x0)) pSprite->cstat |= 0x0; // kSpriteMoveNone
                    else if ((old & 0x2000) && !(pSprite->cstat & 0x2000)) pSprite->cstat |= 0x2000; // kSpriteMoveForward, kSpriteMoveFloor
                    else if ((old & 0x4000) && !(pSprite->cstat & 0x4000)) pSprite->cstat |= 0x4000; // kSpriteMoveReverse, kSpriteMoveCeiling
                }
#endif
            }
        }
        break;
        case OBJ_SECTOR: 
        {
            if (!pSector) return;
            XSECTOR* pXSector = &pSector->xs();

            // data1 = sector underwater status and depth level
            if (pXSource->data1 >= 0 && pXSource->data1 < 2) {
                
                pXSector->Underwater = (pXSource->data1) ? true : false;

                
                spritetype* pUpper = NULL; XSPRITE* pXUpper = NULL;
                    
                auto aLower = barrier_cast<DBloodActor*>(pSector->lowerLink);
                spritetype* pLower = nullptr;
                XSPRITE* pXLower = nullptr;
                if (aLower)
                {
                    pLower = &aLower->s();
                    pXLower = &aLower->x();

                    // must be sure we found exact same upper link
                    for (auto& sec: sector)
                    {
                        auto aUpper = barrier_cast<DBloodActor*>(sec.upperLink);
                        if (aUpper == nullptr || aUpper->x().data1 != pXLower->data1) continue;
                        pUpper = &aUpper->s();
                        pXUpper = &aUpper->x();
                        break;
                    }
                }
                
                // treat sectors that have links, so warp can detect underwater status properly
                if (pLower) 
                {
                    if (pXSector->Underwater) 
                    {
                        switch (pLower->type) 
                        {
                            case kMarkerLowStack:
                            case kMarkerLowLink:
                                pXLower->sysData1 = pLower->type;
                                pLower->type = kMarkerLowWater;
                                break;
                            default:
                                if (pSector->ceilingpicnum < 4080 || pSector->ceilingpicnum > 4095) pXLower->sysData1 = kMarkerLowLink;
                                else pXLower->sysData1 = kMarkerLowStack;
                                break;
                        }
                    }
                    else if (pXLower->sysData1 > 0) pLower->type = pXLower->sysData1;
                    else if (pSector->ceilingpicnum < 4080 || pSector->ceilingpicnum > 4095) pLower->type = kMarkerLowLink;
                    else pLower->type = kMarkerLowStack;
                }

                if (pUpper) 
                {
                    if (pXSector->Underwater) 
                    {
                        switch (pUpper->type) 
                        {
                            case kMarkerUpStack:
                            case kMarkerUpLink:
                                pXUpper->sysData1 = pUpper->type;
                                pUpper->type = kMarkerUpWater;
                                break;
                            default:
                                if (pSector->floorpicnum < 4080 || pSector->floorpicnum > 4095) pXUpper->sysData1 = kMarkerUpLink;
                                else pXUpper->sysData1 = kMarkerUpStack;
                                break;
                        }
                    }
                    else if (pXUpper->sysData1 > 0) pUpper->type = pXUpper->sysData1;
                    else if (pSector->floorpicnum < 4080 || pSector->floorpicnum > 4095) pUpper->type = kMarkerUpLink;
                    else pUpper->type = kMarkerUpStack;
                }

                // search for dudes in this sector and change their underwater status
                BloodSectIterator it(pSector);
                while (auto iactor = it.Next())
                {
                    spritetype* pSpr = &iactor->s();
                    if (pSpr->statnum != kStatDude || !iactor->IsDudeActor() || ! iactor->hasX())
                        continue;

                    PLAYER* pPlayer = getPlayerById(pSpr->type);
                    if (pXSector->Underwater) 
                    {
                        if (pLower)
                            iactor->x().medium = (pLower->type == kMarkerUpGoo) ? kMediumGoo : kMediumWater;

                        if (pPlayer) 
                        {
                            int waterPal = kMediumWater;
                            if (pLower) 
                            {
                                if (pXLower->data2 > 0) waterPal = pXLower->data2;
                                else if (pLower->type == kMarkerUpGoo) waterPal = kMediumGoo;
                            }

                            pPlayer->nWaterPal = waterPal;
                            pPlayer->posture = kPostureSwim;
                            pPlayer->pXSprite->burnTime = 0;
                        }

                    }
                    else 
                    {
                        iactor->x().medium = kMediumNormal;
                        if (pPlayer)
                        {
                            pPlayer->posture = (!(pPlayer->input.actions & SB_CROUCH)) ? kPostureStand : kPostureCrouch;
                            pPlayer->nWaterPal = 0;
                        }

                    }
                }
            }
            else if (pXSource->data1 > 9) pXSector->Depth = 7;
            else if (pXSource->data1 > 1) pXSector->Depth = pXSource->data1 - 2;


            // data2 = sector visibility
            if (valueIsBetween(pXSource->data2, -1, 32767))
                pSector->visibility = ClipRange(pXSource->data2, 0 , 234);

            // data3 = sector ceil cstat
            if (valueIsBetween(pXSource->data3, -1, 32767)) {
                if ((pSource->flags & kModernTypeFlag1)) pSector->ceilingstat |= ESectorFlags::FromInt(pXSource->data3);
                else pSector->ceilingstat = ESectorFlags::FromInt(pXSource->data3);
            }

            // data4 = sector floor cstat
            if (valueIsBetween(pXSource->data4, -1, 65535)) {
                if ((pSource->flags & kModernTypeFlag1)) pSector->floorstat |= ESectorFlags::FromInt(pXSource->data4);
                else pSector->floorstat = ESectorFlags::FromInt(pXSource->data4);
            }
        }
        break;
        // no TX id
        case -1:
            // data2 = global visibility
            if (valueIsBetween(pXSource->data2, -1, 32767))
                gVisibility = ClipRange(pXSource->data2, 0, 4096);
        break;
    }

}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void useTeleportTarget(DBloodActor* sourceactor, DBloodActor* actor) 
{
    auto pSprite = &actor->s();
    auto pSource = &sourceactor->s();
    auto pXSource = &sourceactor->x();

    PLAYER* pPlayer = getPlayerById(pSprite->type);
    XSECTOR* pXSector = (pSource->sector()->hasX()) ? &pSource->sector()->xs() : nullptr;
    bool isDude = (!pPlayer && actor->IsDudeActor());

    if (pSprite->sector() != pSource->sector())
        ChangeActorSect(actor, pSource->sector());

    pSprite->pos.X = pSource->pos.X; pSprite->y = pSource->y;
    int zTop, zBot; 
    GetActorExtents(sourceactor, &zTop, &zBot);
    pSprite->z = zBot;

    clampSprite(actor, 0x01);

    if (pSource->flags & kModernTypeFlag1) // force telefrag
        TeleFrag(actor, pSource->sector());

    if (pSprite->flags & kPhysGravity)
        pSprite->flags |= kPhysFalling;

    if (pXSector) 
    {

        if (pXSector->Enter && (pPlayer || (isDude && !pXSector->dudeLockout)))
            trTriggerSector(pSource->sector(), kCmdSectorEnter);

        if (pXSector->Underwater) 
        {
            auto aLink = barrier_cast<DBloodActor*>(pSource->sector()->lowerLink);
            spritetype* pLink = nullptr;
            if (aLink) 
            {
                // must be sure we found exact same upper link
                for(auto& sec: sector)
                {
                    auto aUpper = barrier_cast<DBloodActor*>(sec.upperLink);
                    if (aUpper == nullptr || aUpper->x().data1 != aLink->x().data1) continue;
                    pLink = &aLink->s();
                    break;
                }
            }

            if (pLink)
                actor->x().medium = (pLink->type == kMarkerUpGoo) ? kMediumGoo : kMediumWater;

            if (pPlayer) 
            {
                int waterPal = kMediumWater;
                if (pLink) 
                {
                    if (aLink->x().data2 > 0) waterPal = aLink->x().data2;
                    else if (pLink->type == kMarkerUpGoo) waterPal = kMediumGoo;
                }

                pPlayer->nWaterPal = waterPal;
                pPlayer->posture = kPostureSwim;
                pPlayer->pXSprite->burnTime = 0;
            }
        } 
        else 
        {
            actor->x().medium = kMediumNormal;
            if (pPlayer) 
            {
                pPlayer->posture = (!(pPlayer->input.actions & SB_CROUCH)) ? kPostureStand : kPostureCrouch;
                pPlayer->nWaterPal = 0;
            }

        }
    }

    if (pSprite->statnum == kStatDude && actor->IsDudeActor() && !actor->IsPlayerActor()) 
    {
        XSPRITE* pXDude = &actor->x();
        int x = pXDude->targetX;
        int y = pXDude->targetY; 
        int z = pXDude->targetZ;
        auto target = actor->GetTarget();
        
        aiInitSprite(actor);

        if (target != nullptr) 
        {
            pXDude->targetX = x; 
            pXDude->targetY = y; 
            pXDude->targetZ = z;
            actor->SetTarget(target);
            aiActivateDude(actor);
        }
    }

    if (pXSource->data2 == 1) 
    {
        if (pPlayer) 
        {
            pPlayer->angle.settarget(pSource->ang);
            pPlayer->angle.lockinput();
        }
        else if (isDude) pXSource->goalAng = pSprite->ang = pSource->ang;
        else pSprite->ang = pSource->ang;
    }

    if (pXSource->data3 == 1)
        actor->xvel = actor->yvel = actor->zvel = 0;

    viewBackupSpriteLoc(actor);

    if (pXSource->data4 > 0)
        sfxPlay3DSound(sourceactor, pXSource->data4, -1, 0);

    if (pPlayer) 
    {
        playerResetInertia(pPlayer);
        if (pXSource->data2 == 1)
            pPlayer->zViewVel = pPlayer->zWeaponVel = 0;
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void useEffectGen(DBloodActor* sourceactor, DBloodActor* actor)
{
    if (!actor) actor = sourceactor;
    auto pSprite = &actor->s();
    auto pSource = &sourceactor->s();
    auto pXSource = &sourceactor->x();

    int fxId = (pXSource->data3 <= 0) ? pXSource->data2 : pXSource->data2 + Random(pXSource->data3 + 1);


    if (!actor->hasX()) return;
    else if (fxId >= kEffectGenCallbackBase) 
    {
        int length = sizeof(gEffectGenCallbacks) / sizeof(gEffectGenCallbacks[0]);
        if (fxId < kEffectGenCallbackBase + length) 
        {
            fxId = gEffectGenCallbacks[fxId - kEffectGenCallbackBase];
            evKillActor(actor, (CALLBACK_ID)fxId);
            evPostActor(actor, 0, (CALLBACK_ID)fxId);
        }
        
    } 
    else if (valueIsBetween(fxId, 0, kFXMax)) 
    {
        int pos, top, bottom; 
        GetActorExtents(actor, &top, &bottom);
        DBloodActor* pEffect = nullptr;

        // select where exactly effect should be spawned
        switch (pXSource->data4) 
        {
            case 1:
                pos = bottom;
                break;
            case 2: // middle
                pos = pSprite->z + (tileHeight(pSprite->picnum) / 2 + tileTopOffset(pSprite->picnum));
                break;
            case 3:
            case 4:
                if (!pSprite->insector()) pos = top;
                else pos = (pXSource->data4 == 3) ? pSprite->sector()->floorz : pSprite->sector()->ceilingz;
                break;
            default:
                pos = top;
                break;
        }

        if ((pEffect = gFX.fxSpawnActor((FX_ID)fxId, pSprite->sector(), pSprite->pos.X, pSprite->y, pos, 0)) != NULL)
        {
            auto pEffectSpr = &pEffect->s();
            pEffect->SetOwner(sourceactor);

            if (pSource->flags & kModernTypeFlag1) 
            {
                pEffectSpr->pal = pSource->pal;
                pEffectSpr->xoffset = pSource->xoffset;
                pEffectSpr->yoffset = pSource->yoffset;
                pEffectSpr->xrepeat = pSource->xrepeat;
                pEffectSpr->yrepeat = pSource->yrepeat;
                pEffectSpr->shade = pSource->shade;
            }

            if (pSource->flags & kModernTypeFlag2) 
            {
                pEffectSpr->cstat = pSource->cstat;
                if (pEffectSpr->cstat & CSTAT_SPRITE_INVISIBLE)
                    pEffectSpr->cstat &= ~CSTAT_SPRITE_INVISIBLE;
            }

            if (pEffectSpr->cstat & CSTAT_SPRITE_ONE_SIDE)
                pEffectSpr->cstat &= ~CSTAT_SPRITE_ONE_SIDE;
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void useSectorWindGen(DBloodActor* sourceactor, sectortype* pSector) 
{
    auto pSource = &sourceactor->s();
    auto pXSource = &sourceactor->x();

    XSECTOR* pXSector = nullptr;
    
    if (pSector != nullptr) 
    {
        pXSector = &pSector->xs();
    }
    else if (pSource->sector()->hasX())
    {
        pSector = pSource->sector();
        pXSector = &pSector->xs();
    }
    else 
    {
        pSector = pSource->sector();
        pSector->allocX();
        pXSector = &pSector->xs();
        pXSector->windAlways = 1;
    }

    int windVel = ClipRange(pXSource->data2, 0, 32767);
    if ((pXSource->data1 & 0x0001))
        windVel = nnExtRandom(0, windVel);
    
    // process vertical wind in nnExtProcessSuperSprites();
    if ((pSource->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR)) 
    {
        pXSource->sysData2 = windVel << 1;
        return;
    }

    pXSector->windVel = windVel;
    if ((pSource->flags & kModernTypeFlag1))
        pXSector->panAlways = pXSector->windAlways = 1;

    int ang = pSource->ang;
    if (pXSource->data4 <= 0) 
    {
        if ((pXSource->data1 & 0x0002)) 
        {
            while (pSource->ang == ang)
                pSource->ang = nnExtRandom(-kAng360, kAng360) & 2047;
        }
    }
    else if (pSource->cstat & CSTAT_SPRITE_MOVE_FORWARD) pSource->ang += pXSource->data4;
    else if (pSource->cstat & CSTAT_SPRITE_MOVE_REVERSE) pSource->ang -= pXSource->data4;
    else if (pXSource->sysData1 == 0) 
    {
        if ((ang += pXSource->data4) >= kAng180) pXSource->sysData1 = 1;
        pSource->ang = ClipHigh(ang, kAng180);
    }
    else 
    {
        if ((ang -= pXSource->data4) <= -kAng180) pXSource->sysData1 = 0;
        pSource->ang = ClipLow(ang, -kAng180);
    }

    pXSector->windAng = pSource->ang;

    if (pXSource->data3 > 0 && pXSource->data3 < 4) 
    {
        switch (pXSource->data3) 
        {
            case 1:
                pXSector->panFloor = true;
                pXSector->panCeiling = false;
                break;
            case 2:
                pXSector->panFloor = false;
                pXSector->panCeiling = true;
                break;
            case 3:
                pXSector->panFloor = true;
                pXSector->panCeiling = true;
                break;
        }
        if (pXSector->panCeiling)
        {
            StartInterpolation(pSector, Interp_Sect_CeilingPanX);
            StartInterpolation(pSector, Interp_Sect_CeilingPanY);
        }
        if (pXSector->panFloor)
        {
            StartInterpolation(pSector, Interp_Sect_FloorPanX);
            StartInterpolation(pSector, Interp_Sect_FloorPanY);
        }

        int oldPan = pXSector->panVel;
        pXSector->panAngle = pXSector->windAng;
        pXSector->panVel = pXSector->windVel;

        // add to panList if panVel was set to 0 previously
        if (oldPan == 0 && pXSector->panVel != 0) 
        {
            if (!panList.Contains(pSector))
                panList.Push(pSector);
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void useSpriteDamager(DBloodActor* sourceactor, int objType, sectortype* targSect, DBloodActor* targetactor)
{
    auto pSource = &sourceactor->s();
    auto pXSource = &sourceactor->x();

    sectortype* pSector = pSource->sector();

    int top, bottom;
    bool floor, ceil, wall, enter;

    switch (objType) 
    {
        case OBJ_SPRITE:
            damageSprites(sourceactor, targetactor);
            break;
        case OBJ_SECTOR:
        {
            GetActorExtents(sourceactor, &top, &bottom);
            floor = (bottom >= pSector->floorz);    
            ceil = (top <= pSector->ceilingz);
            wall = (pSource->cstat & CSTAT_SPRITE_ALIGNMENT_WALL);        
            enter = (!floor && !ceil && !wall);
            BloodSectIterator it(targSect);
            while (auto iactor = it.Next())
            {
                auto& hit = iactor->hit;

                if (!iactor->IsDudeActor() || !iactor->hasX())
                    continue;
                else if (enter)
                    damageSprites(sourceactor, iactor);
                else if (floor && hit.florhit.type == kHitSector && hit.florhit.hitSector == targSect)
                    damageSprites(sourceactor, iactor);
                else if (ceil && hit.ceilhit.type == kHitSector && hit.ceilhit.hitSector == targSect)
                    damageSprites(sourceactor, iactor);
                else if (wall && hit.hit.type == kHitWall && hit.hit.hitWall->sectorp() == targSect)
                    damageSprites(sourceactor, iactor);
            }
            break;
        }
        case -1:
        {
            BloodStatIterator it(kStatDude);
            while (auto iactor = it.Next())
            {
                if (iactor->spr.statnum != kStatDude) continue;
                switch (pXSource->data1) 
                {
                    case 667:
                    if (iactor->IsPlayerActor()) continue;
                    damageSprites(sourceactor, iactor);
                        break;
                    case 668:
                    if (iactor->IsPlayerActor()) continue;
                    damageSprites(sourceactor, iactor);
                        break;
                    default:
                    damageSprites(sourceactor, iactor);
                        break;
                }
            }
            break;
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------


void damageSprites(DBloodActor* sourceactor, DBloodActor* actor) 
{
    spritetype* pSource = &sourceactor->s();
    if (!actor->IsDudeActor() || !actor->hasX() || actor->x().health <= 0 || sourceactor->x().data3 < 0)
        return;
    

    int health = 0;
    auto pSprite = &actor->s();
    auto pXSprite = &actor->x();
    auto pXSource = &sourceactor->x();

    PLAYER* pPlayer = getPlayerById(pSprite->type);
    int dmgType = (pXSource->data2 >= kDmgFall) ? ClipHigh(pXSource->data2, kDmgElectric) : -1;
    int dmg = pXSprite->health << 4; 
    int armor[3];

    bool godMode = (pPlayer && ((dmgType >= 0 && pPlayer->damageControl[dmgType]) || powerupCheck(pPlayer, kPwUpDeathMask) || pPlayer->godMode)); // kneeling

    if (godMode || pXSprite->locked) return;
    else if (pXSource->data3) 
    {
        if (pSource->flags & kModernTypeFlag1) dmg = ClipHigh(pXSource->data3 << 1, 65535);
        else if (pXSprite->sysData2 > 0) dmg = (ClipHigh(pXSprite->sysData2 << 4, 65535) * pXSource->data3) / kPercFull;
        else dmg = ((getDudeInfo(pSprite->type)->startHealth << 4) * pXSource->data3) / kPercFull;

        health = pXSprite->health - dmg;
    }

    if (dmgType >= kDmgFall) 
            {
        if (dmg < (int)pXSprite->health << 4)
                {
            if (!nnExtIsImmune(actor, dmgType, 0))
            {
                if (pPlayer)
                {
                    playerDamageArmor(pPlayer, (DAMAGE_TYPE)dmgType, dmg);
                    for (int i = 0; i < 3; armor[i] = pPlayer->armor[i], pPlayer->armor[i] = 0, i++);
                    actDamageSprite(sourceactor, actor, (DAMAGE_TYPE)dmgType, dmg);
                    for (int i = 0; i < 3; pPlayer->armor[i] = armor[i], i++);

                }
                else
                {
                actDamageSprite(sourceactor, actor, (DAMAGE_TYPE)dmgType, dmg);
                }
            }
            else
            {
                //Printf(PRINT_HIGH, "Dude type %d is immune to damage type %d!", pSprite->type, dmgType);
        }
        }
        else if (!pPlayer) actKillDude(sourceactor, actor, (DAMAGE_TYPE)dmgType, dmg);
        else playerDamageSprite(sourceactor, pPlayer, (DAMAGE_TYPE)dmgType, dmg);
    }
    else if ((pXSprite->health = ClipLow(health, 1)) > 16);
    else if (!pPlayer) actKillDude(sourceactor, actor, kDamageBullet, dmg);
    else playerDamageSprite(sourceactor, pPlayer, kDamageBullet, dmg);

    if (pXSprite->health > 0) 
    {
        if (!(pSource->flags & kModernTypeFlag8))
            pXSprite->health = health;
        
        bool showEffects = !(pSource->flags & kModernTypeFlag2); // show it by default
        bool forceRecoil =  (pSource->flags & kModernTypeFlag4);
        
        if (showEffects) 
        {
            switch (dmgType) 
            {
                case kDmgBurn:
                    if (pXSprite->burnTime > 0) break;
                    actBurnSprite(sourceactor, actor, ClipLow(dmg >> 1, 128));
                    evKillActor(actor, kCallbackFXFlameLick);
                    evPostActor(actor, 0, kCallbackFXFlameLick); // show flames
                    break;
                case kDmgElectric:
                    forceRecoil = true; // show tesla recoil animation
                    break;
                case kDmgBullet:
                    evKillActor(actor, kCallbackFXBloodSpurt);
                    for (int i = 1; i < 6; i++) 
                    {
                        if (Chance(0x16000 >> i))
                            fxSpawnBlood(actor, dmg << 4);
                    }
                    break;
                case kDmgChoke:
                    if (!pPlayer || !Chance(0x2000)) break;
                    else pPlayer->blindEffect += dmg << 2;
            }
        }

        if (forceRecoil && !pPlayer) 
        {
            pXSprite->data3 = 32767;
            actor->dudeExtra.teslaHit = (dmgType == kDmgElectric) ? 1 : 0;
            if (pXSprite->aiState->stateType != kAiStateRecoil)
                RecoilDude(actor);
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void useSeqSpawnerGen(DBloodActor* sourceactor, int objType, sectortype* pSector, walltype* pWall, DBloodActor* iactor) 
{
    auto pXSource = &sourceactor->x();
    if (pXSource->data2 > 0 && !getSequence(pXSource->data2)) 
    {
        Printf(PRINT_HIGH, "Missing sequence #%d", pXSource->data2);
        return;
    }

    spritetype* pSource = &sourceactor->s();

    switch (objType) 
    {
        case OBJ_SECTOR:
        {            
            if (pXSource->data2 <= 0) 
            {
                if (pXSource->data3 == 3 || pXSource->data3 == 1)
                    seqKill(SS_FLOOR, pSector);
                if (pXSource->data3 == 3 || pXSource->data3 == 2)
                    seqKill(SS_CEILING, pSector);
            }
            else 
            {
                if (pXSource->data3 == 3 || pXSource->data3 == 1)
                    seqSpawn(pXSource->data2, SS_FLOOR, pSector, -1);
                if (pXSource->data3 == 3 || pXSource->data3 == 2)
                    seqSpawn(pXSource->data2, SS_CEILING, pSector, -1);
            }
            return;
        }
        case OBJ_WALL:
        {
            if (pXSource->data2 <= 0)
            {
                if (pXSource->data3 == 3 || pXSource->data3 == 1)
                    seqKill(SS_WALL, pWall);
                if ((pXSource->data3 == 3 || pXSource->data3 == 2) && (pWall->cstat & CSTAT_WALL_MASKED))
                    seqKill(SS_MASKED, pWall);
            }
            else
            {
                if (pXSource->data3 == 3 || pXSource->data3 == 1)
                    seqSpawn(pXSource->data2, SS_WALL, pWall, -1);

                if (pXSource->data3 == 3 || pXSource->data3 == 2) {

                    if (!pWall->twoSided()) {
                        if (pXSource->data3 == 3)
                            seqSpawn(pXSource->data2, SS_WALL, pWall, -1);

                    }
                    else {
                        if (!(pWall->cstat & CSTAT_WALL_MASKED))
                            pWall->cstat |= CSTAT_WALL_MASKED;

                        seqSpawn(pXSource->data2, SS_MASKED, pWall, -1);
                    }
                }

                if (pXSource->data4 > 0)
                {
                    int cx, cy, cz;
                    cx = (pWall->pos.X + pWall->point2Wall()->pos.X) >> 1;
                    cy = (pWall->pos.Y + pWall->point2Wall()->pos.Y) >> 1;
                    auto pMySector = pWall->sectorp();
                    int32_t ceilZ, floorZ;
                    getzsofslopeptr(pSector, cx, cy, &ceilZ, &floorZ);
                    int32_t ceilZ2, floorZ2;
                    getzsofslopeptr(pWall->nextSector(), cx, cy, &ceilZ2, &floorZ2);
                    ceilZ = ClipLow(ceilZ, ceilZ2);
                    floorZ = ClipHigh(floorZ, floorZ2);
                    cz = (ceilZ + floorZ) >> 1;

                    sfxPlay3DSound(cx, cy, cz, pXSource->data4, pSector);
                }
            }
            return;
        }
        case OBJ_SPRITE:
        {
            auto pSprite = &iactor->s();
            if (pXSource->data2 <= 0) seqKill(iactor);
            else if (pSprite->insector())
            {
                if (pXSource->data3 > 0)
                {
                    auto spawned = InsertSprite(pSprite->sector(), kStatDecoration);
                    auto pSpawned = &spawned->s();
                    int top, bottom; GetActorExtents(spawned, &top, &bottom);
                    pSpawned->pos.X = pSprite->pos.X;
                    pSpawned->y = pSprite->y;
                    switch (pXSource->data3) 
                    {                    
					default:
                        pSpawned->z = pSprite->z;
                        break;
                    case 2:
                        pSpawned->z = bottom;
                        break;
                    case 3:
                        pSpawned->z = top;
                        break;
                    case 4:
                        pSpawned->z = pSprite->z + tileHeight(pSprite->picnum) / 2 + tileTopOffset(pSprite->picnum);
                        break;
                    case 5:
                    case 6:
                        if (!pSprite->insector()) pSpawned->z = top;
                        else pSpawned->z = (pXSource->data3 == 5) ? spawned->sector()->floorz : spawned->sector()->ceilingz;
                        break;
                    }

                    if (spawned != nullptr)
                    {

                        spawned->addX();
                        seqSpawn(pXSource->data2, spawned, -1);
                        if (pSource->flags & kModernTypeFlag1) 
                        {
                            pSpawned->pal = pSource->pal;
                            pSpawned->shade = pSource->shade;
                            pSpawned->xrepeat = pSource->xrepeat;
                            pSpawned->yrepeat = pSource->yrepeat;
                            pSpawned->xoffset = pSource->xoffset;
                            pSpawned->yoffset = pSource->yoffset;
                        }

                        if (pSource->flags & kModernTypeFlag2) 
                        {
                            pSpawned->cstat |= pSource->cstat;
                        }

                        // should be: the more is seqs, the shorter is timer
                        evPostActor(spawned, 1000, kCallbackRemove);
                    }
                }
                else 
                {
                    seqSpawn(pXSource->data2, iactor, -1);
                }

                if (pXSource->data4 > 0)
                    sfxPlay3DSound(iactor, pXSource->data4, -1, 0);
            }
            return;
    }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void condPush(DBloodActor* actor, const EventObject& iactor) { actor->condition[0] = iactor; }
void condPush(DBloodActor* actor, DBloodActor* iactor) { actor->condition[0] = EventObject(iactor); }
void condPush(DBloodActor* actor, walltype* iactor) { actor->condition[0] = EventObject(iactor); }
void condPush(DBloodActor* actor, sectortype* iactor) { actor->condition[0] = EventObject(iactor); }
EventObject condGet(DBloodActor* actor) { return actor->condition[0]; }

void condBackup(DBloodActor* actor)
{
    actor->condition[1] = actor->condition[0];
}

void condRestore(DBloodActor* actor)
{
    actor->condition[0] = actor->condition[1];
}

// normal comparison
bool condCmp(int val, int arg1, int arg2, int comOp) 
{
    if (comOp & 0x2000) return (comOp & CSTAT_SPRITE_BLOCK) ? (val > arg1) : (val >= arg1); // blue sprite
    else if (comOp & 0x4000) return (comOp & CSTAT_SPRITE_BLOCK) ? (val < arg1) : (val <= arg1); // green sprite
    else if (comOp & CSTAT_SPRITE_BLOCK) 
    {
        if (arg1 > arg2) I_Error("Value of argument #1 (%d) must be less than value of argument #2 (%d)", arg1, arg2);
        return (val >= arg1 && val <= arg2);
    }
    else return (val == arg1);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void condError(DBloodActor* aCond, const char* pzFormat, ...)
{
    char buffer[256]; 
    char buffer2[512]; 
    FString condType = "Unknown";
    auto pXCond = &aCond->x();
    for (int i = 0; i < 7; i++) 
    {
        if (pXCond->data1 < gCondTypeNames[i].rng1 || pXCond->data1 >= gCondTypeNames[i].rng2) continue;
        condType = gCondTypeNames[i].name;
        condType.ToUpper();
        break;
    }
    
    snprintf(buffer, 256, "\n\n%s CONDITION RX: %d, TX: %d, SPRITE: #%d RETURNS:\n", condType.GetChars(), pXCond->rxID, pXCond->txID, aCond->GetIndex());
    va_list args;
    va_start(args, pzFormat);
    vsnprintf(buffer2, 512, pzFormat, args);
    I_Error("%s%s", buffer, buffer2);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool condCheckGame(DBloodActor* aCond, const EVENT& event, int cmpOp, bool PUSH)
{
    auto pXCond = &aCond->x();

    //int var = -1;
    int cond = pXCond->data1 - kCondGameBase; 
    int arg1 = pXCond->data2;
    int arg2 = pXCond->data3; 
    int arg3 = pXCond->data4;

    switch (cond) {
        case 1:  return condCmp(gFrameCount / (kTicsPerSec * 60), arg1, arg2, cmpOp);            // compare level minutes
        case 2:  return condCmp((gFrameCount / kTicsPerSec) % 60, arg1, arg2, cmpOp);            // compare level seconds
        case 3:  return condCmp(((gFrameCount % kTicsPerSec) * 33) / 10, arg1, arg2, cmpOp);     // compare level mseconds
        case 4:  return condCmp(gFrameCount, arg1, arg2, cmpOp);                                 // compare level time (unsafe)
        case 5:  return condCmp(gKillMgr.Kills, arg1, arg2, cmpOp);                             // compare current global kills counter
        case 6:  return condCmp(gKillMgr.TotalKills, arg1, arg2, cmpOp);                        // compare total global kills counter
        case 7:  return condCmp(gSecretMgr.Founds, arg1, arg2, cmpOp);                          // compare how many secrets found
        case 8:  return condCmp(gSecretMgr.Total, arg1, arg2, cmpOp);                           // compare total secrets
        /*----------------------------------------------------------------------------------------------------------------------------------*/
        case 20: return condCmp(gVisibility, arg1, arg2, cmpOp);                                // compare global visibility value
        /*----------------------------------------------------------------------------------------------------------------------------------*/
        case 30: return Chance((0x10000 * arg3) / kPercFull);                                   // check chance
        case 31: return condCmp(nnExtRandom(arg1, arg2), arg1, arg2, cmpOp);
        /*----------------------------------------------------------------------------------------------------------------------------------*/
        case 47: return condCmp(gStatCount[ClipRange(arg3, 0, kMaxStatus)], arg1, arg2, cmpOp); // compare counter of specific statnum sprites
        case 48: return condCmp(Numsprites, arg1, arg2, cmpOp);                                 // compare counter of total sprites
    }

    condError(aCond, "Unexpected condition id (%d)!", cond);
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool condCheckMixed(DBloodActor* aCond, const EVENT& event, int cmpOp, bool PUSH)
{
    auto pXCond = &aCond->x();

    //int var = -1;
    int cond = pXCond->data1 - kCondMixedBase; int arg1 = pXCond->data2;
    int arg2 = pXCond->data3; int arg3 = pXCond->data4;
    
    auto eob = condGet(aCond);

    switch (cond) 
    {
        case 0:  return (eob.isSector());
        case 5:  return (eob.isWall());
        case 10: return (eob.isActor() && eob.actor());
        case 15: // x-index is fine?
            if (eob.isWall()) return eob.wall()->hasX();
            if (eob.isSector()) return eob.sector()->hasX();
            if (eob.isActor()) return eob.actor() && eob.actor()->hasX();
            break;
        case 20: // type in a range?
            if (eob.isWall()) return condCmp(eob.wall()->type, arg1, arg2, cmpOp);
            if (eob.isSector())  return condCmp(eob.sector()->type, arg1, arg2, cmpOp);
            if (eob.isActor()) return eob.actor() && condCmp(eob.actor()->spr.type, arg1, arg2, cmpOp);
            break;
        case 24:
        case 25: case 26: case 27:
        case 28: case 29: case 30:
        case 31: case 32: case 33:
                if (eob.isWall())
            {
                    walltype* pObj = eob.wall();
                    switch (cond) 
                    {
                        case 24: return condCmp(surfType[pObj->picnum], arg1, arg2, cmpOp);
                        case 25: return condCmp(pObj->picnum, arg1, arg2, cmpOp);
                        case 26: return condCmp(pObj->pal, arg1, arg2, cmpOp);
                        case 27: return condCmp(pObj->shade, arg1, arg2, cmpOp);
                        case 28: return (pObj->cstat & EWallFlags::FromInt(arg1));
                        case 29: return (pObj->hitag & arg1);
                        case 30: return condCmp(pObj->xrepeat, arg1, arg2, cmpOp);
                        case 31: return condCmp(pObj->xpan(), arg1, arg2, cmpOp);
                        case 32: return condCmp(pObj->yrepeat, arg1, arg2, cmpOp);
                        case 33: return condCmp(pObj->ypan(), arg1, arg2, cmpOp);
                    }
                }
                else if (eob.isActor())
                {
                    if (!eob.actor()) break;
                    spritetype* pObj = &eob.actor()->s();
                    switch (cond) 
                    {
                        case 24: return condCmp(surfType[pObj->picnum], arg1, arg2, cmpOp);
                        case 25: return condCmp(pObj->picnum, arg1, arg2, cmpOp);
                        case 26: return condCmp(pObj->pal, arg1, arg2, cmpOp);
                        case 27: return condCmp(pObj->shade, arg1, arg2, cmpOp);
                        case 28: return (pObj->cstat & ESpriteFlags::FromInt(arg1));
                        case 29: return (pObj->flags & arg1);
                        case 30: return condCmp(pObj->xrepeat, arg1, arg2, cmpOp);
                        case 31: return condCmp(pObj->xoffset, arg1, arg2, cmpOp);
                        case 32: return condCmp(pObj->yrepeat, arg1, arg2, cmpOp);
                        case 33: return condCmp(pObj->yoffset, arg1, arg2, cmpOp);
                    }
                }
                else if (eob.sector())
                {
                    sectortype* pObj = eob.sector();
                    switch (cond) 
                    {
                        case 24:
                            switch (arg3) 
                            {
                                default: return (condCmp(surfType[pObj->floorpicnum], arg1, arg2, cmpOp) || condCmp(surfType[pObj->ceilingpicnum], arg1, arg2, cmpOp));
                                case 1: return condCmp(surfType[pObj->floorpicnum], arg1, arg2, cmpOp);
                                case 2: return condCmp(surfType[pObj->ceilingpicnum], arg1, arg2, cmpOp);
                            }
                            break;
                        case 25:
                            switch (arg3) 
                            {
                                default: return (condCmp(pObj->floorpicnum, arg1, arg2, cmpOp) || condCmp(pObj->ceilingpicnum, arg1, arg2, cmpOp));
                                case 1:  return condCmp(pObj->floorpicnum, arg1, arg2, cmpOp);
                                case 2:  return condCmp(pObj->ceilingpicnum, arg1, arg2, cmpOp);
                            }
                            break;
                        case 26: 
                            switch (arg3) 
                            {
                                default: return (condCmp(pObj->floorpal, arg1, arg2, cmpOp) || condCmp(pObj->ceilingpal, arg1, arg2, cmpOp));
                                case 1:  return condCmp(pObj->floorpal, arg1, arg2, cmpOp);
                                case 2:  return condCmp(pObj->ceilingpal, arg1, arg2, cmpOp);
                            }
                            break;
                        case 27:
                            switch (arg3) 
                            {
                                default: return (condCmp(pObj->floorshade, arg1, arg2, cmpOp) || condCmp(pObj->ceilingshade, arg1, arg2, cmpOp));
                                case 1:  return condCmp(pObj->floorshade, arg1, arg2, cmpOp);
                                case 2:  return condCmp(pObj->ceilingshade, arg1, arg2, cmpOp);
                            }
                            break;
                        case 28:
                        {
                            auto a = ESectorFlags::FromInt(arg1);
                            switch (arg3) 
                            {
                                default: return ((pObj->floorstat & a) || (pObj->ceilingstat & a));
                                case 1:  return (pObj->floorstat & a);
                                case 2:  return (pObj->ceilingstat & a);
                            }
                            break;
                        }
                        case 29: return (pObj->hitag & arg1);
                        case 30: return condCmp(pObj->floorxpan(), arg1, arg2, cmpOp);
                        case 31: return condCmp(pObj->ceilingxpan(), arg1, arg2, cmpOp);
                        case 32: return condCmp(pObj->floorypan(), arg1, arg2, cmpOp);
                        case 33: return condCmp(pObj->ceilingypan(), arg1, arg2, cmpOp);
                    }
                }
            break;
        case 41:  case 42:  case 43:
        case 44:  case 50:  case 51:
        case 52:  case 53:  case 54:
        case 55:  case 56:  case 57:
        case 58:  case 59:  case 70:
        case 71:
                if (eob.isWall())
            {
                    auto pObj = eob.wall();
                    if (!pObj->hasX())
                        return condCmp(0, arg1, arg2, cmpOp);
                    
                    XWALL* pXObj =  &pObj->xw();
                    switch (cond) 
                    {
                        case 41: return condCmp(pXObj->data, arg1, arg2, cmpOp);
                        case 50: return condCmp(pXObj->rxID, arg1, arg2, cmpOp);
                        case 51: return condCmp(pXObj->txID, arg1, arg2, cmpOp);
                        case 52: return pXObj->locked;
                        case 53: return pXObj->triggerOn;
                        case 54: return pXObj->triggerOff;
                        case 55: return pXObj->triggerOnce;
                        case 56: return pXObj->isTriggered;
                        case 57: return pXObj->state;
                        case 58: return condCmp((kPercFull * pXObj->busy) / 65536, arg1, arg2, cmpOp);
                        case 59: return pXObj->dudeLockout;
                        case 70:
                            switch (arg3) 
                            {
                                default: return (condCmp(seqGetID(SS_WALL, pObj), arg1, arg2, cmpOp) || condCmp(seqGetID(SS_MASKED, pObj), arg1, arg2, cmpOp));
                                case 1:  return condCmp(seqGetID(SS_WALL, pObj), arg1, arg2, cmpOp);
                                case 2:  return condCmp(seqGetID(SS_MASKED, pObj), arg1, arg2, cmpOp);
                            }
                            break;
                        case 71:
                            switch (arg3) 
                            {
                                default: return (condCmp(seqGetStatus(SS_WALL, pObj), arg1, arg2, cmpOp) || condCmp(seqGetStatus(SS_MASKED, pObj), arg1, arg2, cmpOp));
                                case 1:  return condCmp(seqGetStatus(SS_WALL, pObj), arg1, arg2, cmpOp);
                                case 2:  return condCmp(seqGetStatus(SS_MASKED, pObj), arg1, arg2, cmpOp);
                            }
                            break;
                    }
                }
                else if (eob.isActor())
                {
                    auto objActor = eob.actor();
                    if (!objActor) break;
                    if (!objActor->hasX())
                        return condCmp(0, arg1, arg2, cmpOp);
                    
                    XSPRITE* pXObj = &objActor->x();
                    switch (cond) 
                    {
                        case 41: case 42:
                        case 43: case 44:
                            return condCmp(getDataFieldOfObject(eob, 1 + cond - 41), arg1, arg2, cmpOp);
                        case 50: return condCmp(pXObj->rxID, arg1, arg2, cmpOp);
                        case 51: return condCmp(pXObj->txID, arg1, arg2, cmpOp);
                        case 52: return pXObj->locked;
                        case 53: return pXObj->triggerOn;
                        case 54: return pXObj->triggerOff;
                        case 55: return pXObj->triggerOnce;
                        case 56: return pXObj->isTriggered;
                        case 57: return pXObj->state;
                        case 58: return condCmp((kPercFull * pXObj->busy) / 65536, arg1, arg2, cmpOp);
                        case 59: return pXObj->DudeLockout;
                        case 70: return condCmp(seqGetID(objActor), arg1, arg2, cmpOp);
                        case 71: return condCmp(seqGetStatus(objActor), arg1, arg2, cmpOp);
                    }
                }
                else if (eob.isSector())
                {
                    auto pObj = eob.sector();
                    if (!pObj->hasX())
                        return condCmp(0, arg1, arg2, cmpOp);
                    
                    XSECTOR* pXObj = &pObj->xs();
                    switch (cond) {
                        case 41: return condCmp(pXObj->data, arg1, arg2, cmpOp);
                        case 50: return condCmp(pXObj->rxID, arg1, arg2, cmpOp);
                        case 51: return condCmp(pXObj->txID, arg1, arg2, cmpOp);
                        case 52: return pXObj->locked;
                        case 53: return pXObj->triggerOn;
                        case 54: return pXObj->triggerOff;
                        case 55: return pXObj->triggerOnce;
                        case 56: return pXObj->isTriggered;
                        case 57: return pXObj->state;
                        case 58: return condCmp((kPercFull * pXObj->busy) / 65536, arg1, arg2, cmpOp);
                        case 59: return pXObj->dudeLockout;
                        case 70:
                            // wall???
                            switch (arg3) 
                            {
                                default: return (condCmp(seqGetID(SS_CEILING, pObj), arg1, arg2, cmpOp) || condCmp(seqGetID(SS_FLOOR, pObj), arg1, arg2, cmpOp));
                                case 1:  return condCmp(seqGetID(SS_CEILING, pObj), arg1, arg2, cmpOp);
                                case 2:  return condCmp(seqGetID(SS_FLOOR, pObj), arg1, arg2, cmpOp);
                            }
                            break;
                        case 71:
                            switch (arg3) 
                            {
                                default: return (condCmp(seqGetStatus(SS_CEILING, pObj), arg1, arg2, cmpOp) || condCmp(seqGetStatus(SS_FLOOR, pObj), arg1, arg2, cmpOp));
                                case 1:  return condCmp(seqGetStatus(SS_CEILING, pObj), arg1, arg2, cmpOp);
                                case 2:  return condCmp(seqGetStatus(SS_FLOOR, pObj), arg1, arg2, cmpOp);
                            }
                            break;
                    }
                }
            break;
        case 99: return condCmp(event.cmd, arg1, arg2, cmpOp);  // this codition received specified command?
    }

    condError(aCond, "Unexpected condition id (%d)!", cond);
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool condCheckSector(DBloodActor* aCond, int cmpOp, bool PUSH) 
{
    auto pXCond = &aCond->x();

    int cond = pXCond->data1 - kCondSectorBase; 
    int arg1 = pXCond->data2;
    int arg2 = pXCond->data3; //int arg3 = pXCond->data4;
    
    auto eob = condGet(aCond);

    if (!eob.isSector())
        condError(aCond, "Sector expected, got %s", eob.description().GetChars());

    sectortype* pSect = eob.sector();
    XSECTOR* pXSect = pSect->hasX()? &pSect->xs() : nullptr;

    if (cond < (kCondRange >> 1)) 
    {
        switch (cond) 
        {
        default: break;
        case 0: return condCmp(pSect->visibility, arg1, arg2, cmpOp);
        case 5: return condCmp(pSect->floorheinum, arg1, arg2, cmpOp);
        case 6: return condCmp(pSect->ceilingheinum, arg1, arg2, cmpOp);
        case 10: // required sprite type is in current sector?
            BloodSectIterator it(pSect);
            while (auto iactor = it.Next())
            {
                if (!condCmp(iactor->spr.type, arg1, arg2, cmpOp)) continue;
                else if (PUSH) condPush(aCond, iactor);
                return true;
            }
            return false;
        }
    } 
    else if (pXSect) 
    {
        switch (cond) 
        {
            default: break;
            case 50: return pXSect->Underwater;
            case 51: return condCmp(pXSect->Depth, arg1, arg2, cmpOp);
            case 55: // compare floor height (in %)
            case 56: { // compare ceil height (in %)
                int h = 0; int curH = 0;
                switch (pSect->type) 
                {
                case kSectorZMotion:
                case kSectorRotate:
                case kSectorSlide:
                    if (cond == 60) 
                    {
                        h = ClipLow(abs(pXSect->onFloorZ - pXSect->offFloorZ), 1);
                        curH = abs(pSect->floorz - pXSect->offFloorZ);
                    } 
                    else 
                    {
                        h = ClipLow(abs(pXSect->onCeilZ - pXSect->offCeilZ), 1);
                        curH = abs(pSect->ceilingz - pXSect->offCeilZ);
                    }
                    return condCmp((kPercFull * curH) / h, arg1, arg2, cmpOp);
                default:
                    condError(aCond, "Usupported sector type %d", pSect->type);
                    return false;
                }
            }
            case 57: // this sector in movement?
                return !pXSect->unused1;
        }
    } 
    else 
    {
        switch (cond) 
        {
            default: return false;
            case 55:
            case 56:
                return condCmp(0, arg1, arg2, cmpOp);
        }
    }
    
    condError(aCond, "Unexpected condition id (%d)!", cond);
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool condCheckWall(DBloodActor* aCond, int cmpOp, bool PUSH) 
{
    auto pXCond = &aCond->x();

    int var = -1;
    int cond = pXCond->data1 - kCondWallBase; int arg1 = pXCond->data2;
    int arg2 = pXCond->data3; //int arg3 = pXCond->data4;

    auto eob = condGet(aCond);
    if (!eob.isWall())
        condError(aCond, "Wall expected, got %s", eob.description().GetChars());
        
    walltype* pWall = eob.wall();
    
    if (cond < (kCondRange >> 1)) 
    {
        switch (cond) 
        {
            default: break;
            case 0:
                return condCmp(pWall->overpicnum, arg1, arg2, cmpOp);
            case 5:
                if (PUSH) condPush(aCond, pWall->sectorp());
                return true;
            case 10: // this wall is a mirror?                          // must be as constants here
                return (pWall->type != kWallStack && condCmp(pWall->picnum, 4080, (4080 + 16) - 1, 0));
            case 15:
                if (!pWall->twoSided()) return false;
                else if (PUSH) condPush(aCond, pWall->nextSector());
                return true;
            case 20:
                if (!pWall->twoSided()) return false;
                else if (PUSH) condPush(aCond, pWall->nextWall());
                return true;
            case 25: // next wall belongs to sector? (Note: This was 'sector of next wall' which is same as case 15 because we do not allow bad links!)
                if (!pWall->twoSided()) return false;
                else if (PUSH) condPush(aCond, pWall->nextSector());
                return true;
        }
    }

    condError(aCond, "Unexpected condition id (%d)!", cond);
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool condCheckPlayer(DBloodActor* aCond, int cmpOp, bool PUSH) 
{
    auto pXCond = &aCond->x();

    int var = -1;
    PLAYER* pPlayer = NULL;
    int cond = pXCond->data1 - kCondPlayerBase; 
    int arg1 = pXCond->data2;
    int arg2 = pXCond->data3; 
    int arg3 = pXCond->data4;

    auto eob = condGet(aCond);

    if (!eob.isActor() || !eob.actor())
        condError(aCond, "Sprite expected, got %s", eob.description().GetChars());

    auto objActor = eob.actor();
    for (int i = 0; i < kMaxPlayers; i++) 
    {
        if (objActor != gPlayer[i].actor) continue;
        pPlayer = &gPlayer[i];
        break;
    }
    
    if (!pPlayer) 
    {
        condError(aCond, "Player expected, got %s", eob.description().GetChars());
        return false;
    }

    switch (cond) {
        case 0: // check if this player is connected
            if (!condCmp(pPlayer->nPlayer + 1, arg1, arg2, cmpOp) || pPlayer->actor == nullptr) return false;
            else if (PUSH) condPush(aCond, pPlayer->actor);
            return (pPlayer->nPlayer >= 0);
        case 1: return condCmp((gGameOptions.nGameType != 3) ? 0 : pPlayer->teamId + 1, arg1, arg2, cmpOp); // compare team
        case 2: return (arg1 > 0 && arg1 < 8 && pPlayer->hasKey[arg1 - 1]);
        case 3: return (arg1 > 0 && arg1 < 15 && pPlayer->hasWeapon[arg1 - 1]);
        case 4: return condCmp(pPlayer->curWeapon, arg1, arg2, cmpOp);
        case 5: return (arg1 > 0 && arg1 < 6 && condCmp(pPlayer->packSlots[arg1 - 1].curAmount, arg2, arg3, cmpOp));
        case 6: return (arg1 > 0 && arg1 < 6 && pPlayer->packSlots[arg1 - 1].isActive);
        case 7: return condCmp(pPlayer->packItemId + 1, arg1, arg2, cmpOp);
        case 8: // check for powerup amount in seconds
            if (arg3 > 0 && arg3 <= (kMaxAllowedPowerup - (kMinAllowedPowerup << 1) + 1)) {
                var = (kMinAllowedPowerup + arg3) - 1; // allowable powerups
                return condCmp(pPlayer->pwUpTime[var] / 100, arg1, arg2, cmpOp);
            }
            condError(aCond, "Unexpected powerup #%d", arg3);
            return false;
        case 9:
            if (!pPlayer->fragger) return false;
            else if (PUSH) condPush(aCond, pPlayer->fragger);
            return true;
        case 10: // check keys pressed
            switch (arg1) {
            case 1:  return (pPlayer->input.fvel > 0);            // forward
            case 2:  return (pPlayer->input.fvel < 0);            // backward
            case 3:  return (pPlayer->input.svel > 0);             // left
            case 4:  return (pPlayer->input.svel < 0);             // right
            case 5:  return !!(pPlayer->input.actions & SB_JUMP);       // jump
            case 6:  return !!(pPlayer->input.actions & SB_CROUCH);     // crouch
            case 7:  return !!(pPlayer->input.actions & SB_FIRE);      // normal fire weapon
            case 8:  return !!(pPlayer->input.actions & SB_ALTFIRE);     // alt fire weapon
            case 9:  return !!(pPlayer->input.actions & SB_OPEN);        // use
            default:
                condError(aCond, "Specify a correct key!");
                break;
            }
            return false;
        case 11: return (pPlayer->isRunning);
        case 12: return (pPlayer->fallScream); // falling in abyss?
        case 13: return condCmp(pPlayer->lifeMode + 1, arg1, arg2, cmpOp);
        case 14: return condCmp(pPlayer->posture + 1, arg1, arg2, cmpOp);
        case 46: return condCmp(pPlayer->sceneQav, arg1, arg2, cmpOp);
        case 47: return (pPlayer->godMode || powerupCheck(pPlayer, kPwUpDeathMask));
        case 48: return isShrinked(pPlayer->actor);
        case 49: return isGrown(pPlayer->actor);
    }

    condError(aCond, "Unexpected condition #%d!", cond);
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool condCheckDude(DBloodActor* aCond, int cmpOp, bool PUSH) 
{
    auto pXCond = &aCond->x();

    int var = -1;
    int cond = pXCond->data1 - kCondDudeBase; int arg1 = pXCond->data2;
    int arg2 = pXCond->data3; int arg3 = pXCond->data4;
    
    auto eob = condGet(aCond);

    if (!eob.isActor() || !eob.actor())
        condError(aCond, "Sprite expected, got %s", eob.description().GetChars());

    auto objActor = eob.actor();

    spritetype* pSpr = &objActor->s();
    if (!objActor->hasX() || pSpr->type == kThingBloodChunks)
        condError(aCond, "Sprite #%d is dead!", objActor->GetIndex());
    
    if (!objActor->IsDudeActor() || objActor->IsPlayerActor())
        condError(aCond, "Object #%d is not an enemy!", objActor->GetIndex());

    XSPRITE* pXSpr = &objActor->x();
    auto targ = objActor->GetTarget();
    switch (cond)
    {
        default: break;
        case 0: // dude have any targets?
            if (!targ) return false;
            else if (!targ->IsDudeActor() && targ->spr.type != kMarkerPath) return false;
            else if (PUSH) condPush(aCond, targ);
            return true;
        case 1: return aiFightDudeIsAffected(objActor); // dude affected by ai fight?
        case 2: // distance to the target in a range?
        case 3: // is the target visible?
        case 4: // is the target visible with periphery?
        {

            if (!targ)
                condError(aCond, "Dude #%d has no target!", objActor->GetIndex());

            spritetype* pTrgt = &targ->s();
            DUDEINFO* pInfo = getDudeInfo(pSpr->type);
            int eyeAboveZ = pInfo->eyeHeight * pSpr->yrepeat << 2;
            int dx = pTrgt->pos.X - pSpr->pos.X; 
            int dy = pTrgt->y - pSpr->y;

            switch (cond) 
            {
            case 2:
                    var = condCmp(approxDist(dx, dy), arg1 * 512, arg2 * 512, cmpOp);
                    break;
                case 3:
                case 4:
                    var = cansee(pSpr->pos.X, pSpr->y, pSpr->z, pSpr->sector(), pTrgt->pos.X, pTrgt->y, pTrgt->z - eyeAboveZ, pTrgt->sector());
                if (cond == 4 && var > 0) 
                {
                        var = ((1024 + getangle(dx, dy) - pSpr->ang) & 2047) - 1024;
                        var = (abs(var) < ((arg1 <= 0) ? pInfo->periphery : ClipHigh(arg1, 2048)));
                    }
                    break;
            }

            if (var <= 0) return false;
            else if (PUSH) condPush(aCond, targ);
            return true;

        }
        case 5: return pXSpr->dudeFlag4;
        case 6: return pXSpr->dudeDeaf;
        case 7: return pXSpr->dudeGuard;
        case 8: return pXSpr->dudeAmbush;
        case 9: return (pXSpr->unused1 & kDudeFlagStealth);
        case 10: // check if the marker is busy with another dude
        case 11: // check if the marker is reached
                if (!pXSpr->dudeFlag4 || !targ || targ->spr.type != kMarkerPath) return false;
            switch (cond) {
                case 10:
                {
                    auto var = aiPatrolMarkerBusy(objActor, targ);
                    if (!var) return false;
                    else if (PUSH) condPush(aCond, var);
                    break;
                }
                case 11:
                    if (!aiPatrolMarkerReached(objActor)) return false;
                    else if (PUSH) condPush(aCond, targ);
                    break;
            }
            return true;
        case 12: // compare spot progress value in %
            if (!pXSpr->dudeFlag4 || !targ || targ->spr.type != kMarkerPath) var = 0;
            else if (!(pXSpr->unused1 & kDudeFlagStealth) || pXSpr->data3 < 0 || pXSpr->data3 > kMaxPatrolSpotValue) var = 0;
            else var = (kPercFull * pXSpr->data3) / kMaxPatrolSpotValue;
            return condCmp(var, arg1, arg2, cmpOp);
        case 15: return getDudeInfo(pSpr->type)->lockOut; // dude allowed to interact with objects?
        case 16: return condCmp(pXSpr->aiState->stateType, arg1, arg2, cmpOp);
        case 17: return condCmp(pXSpr->stateTimer, arg1, arg2, cmpOp);
        case 20: // kDudeModernCustom conditions
        case 21:
        case 22:
        case 23:
        case 24:
            switch (pSpr->type) 
            {
            case kDudeModernCustom:
            case kDudeModernCustomBurning:
                switch (cond) {
                    case 20: // life leech is thrown?
                    {
                        DBloodActor* act = objActor->genDudeExtra.pLifeLeech;
                        if (!act) return false;
                        else if (PUSH) condPush(aCond, act);
                        return true;
                    }

                    case 21: // life leech is destroyed?
                    {
                        DBloodActor* act = objActor->genDudeExtra.pLifeLeech;
                        if (!act) return false;
                        if (objActor->GetSpecialOwner()) return true;
                        else if (PUSH) condPush(aCond, act);
                        return false;
                    }

                    case 22: // are required amount of dudes is summoned?
                        return condCmp(objActor->genDudeExtra.slaveCount, arg1, arg2, cmpOp);

                    case 23: // check if dude can...
                        switch (arg3) 
                        {
                            case 1: return objActor->genDudeExtra.canAttack;
                            case 2: return objActor->genDudeExtra.canBurn;
                            case 3: return objActor->genDudeExtra.canDuck;
                            case 4: return objActor->genDudeExtra.canElectrocute;
                            case 5: return objActor->genDudeExtra.canFly;
                            case 6: return objActor->genDudeExtra.canRecoil;
                            case 7: return objActor->genDudeExtra.canSwim;
                            case 8: return objActor->genDudeExtra.canWalk;
                            default: condError(aCond, "Invalid argument %d", arg3); break;
                        }
                        break;
                    case 24: // compare weapon dispersion
                        return condCmp(objActor->genDudeExtra.baseDispersion, arg1, arg2, cmpOp);
                }
                break;
            default:
                condError(aCond, "Dude #%d is not a Custom Dude!", objActor->GetIndex());
                return false;
            }
    }

    condError(aCond, "Unexpected condition #%d!", cond);
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool condCheckSprite(DBloodActor* aCond, int cmpOp, bool PUSH)
{
    auto pXCond = &aCond->x();

    int var = -1, var2 = -1, var3 = -1; PLAYER* pPlayer = NULL; bool retn = false;
    int cond = pXCond->data1 - kCondSpriteBase; int arg1 = pXCond->data2;
    int arg2 = pXCond->data3; int arg3 = pXCond->data4;
    
    auto eob = condGet(aCond);

    if (!eob.isActor() || !eob.actor())
        condError(aCond, "Sprite expected, got %s", eob.description().GetChars());

    auto objActor = eob.actor();

    spritetype* pSpr = &objActor->s();
    XSPRITE* pXSpr = objActor->hasX()? &objActor->x() : nullptr;
    
    if (cond < (kCondRange >> 1)) 
    {
        switch (cond) 
        {
            default: break;
            case 0: return condCmp((pSpr->ang & 2047), arg1, arg2, cmpOp);
            case 5: return condCmp(pSpr->statnum, arg1, arg2, cmpOp);
            case 6: return ((pSpr->flags & kHitagRespawn) || pSpr->statnum == kStatRespawn);
        case 7: return condCmp(spriteGetSlope(pSpr), arg1, arg2, cmpOp);
            case 10: return condCmp(pSpr->clipdist, arg1, arg2, cmpOp);
            case 15:
            if (!objActor->GetOwner()) return false;
            else if (PUSH) condPush(aCond, objActor->GetOwner());
                return true;
            case 20: // stays in a sector?
                if (!pSpr->insector()) return false;
            else if (PUSH) condPush(aCond, pSpr->sector());
                return true;
            case 25:
            switch (arg1)
            {
            case 0: return (objActor->xvel || objActor->yvel || objActor->zvel);
            case 1: return (objActor->xvel);
            case 2: return (objActor->yvel);
            case 3: return (objActor->zvel);
                }
                break;
            case 30:
                if (!spriteIsUnderwater(objActor) && !spriteIsUnderwater(objActor, true)) return false;
            else if (PUSH) condPush(aCond, pSpr->sector());
                return true;
        case 31:
            if (arg1 == -1)
            {
                for (var = 0; var < kDmgMax; var++)
                {
                        if (!nnExtIsImmune(objActor, arg1, 0))
                            return false;
                    }
                    return true;
                }
                return nnExtIsImmune(objActor, arg1, 0);

            case 35: // hitscan: ceil?
            case 36: // hitscan: floor?
            case 37: // hitscan: wall?
            case 38: // hitscan: sprite?
            switch (arg1)
            {
                    case  0: arg1 = CLIPMASK0 | CLIPMASK1; break;
                    case  1: arg1 = CLIPMASK0; break;
                    case  2: arg1 = CLIPMASK1; break;
                }

                if ((pPlayer = getPlayerById(pSpr->type)) != NULL)
                    var = HitScan(objActor, pPlayer->zWeapon, pPlayer->aim.dx, pPlayer->aim.dy, pPlayer->aim.dz, arg1, arg3 << 1);
            else if (objActor->IsDudeActor())
                var = HitScan(objActor, pSpr->z, bcos(pSpr->ang), bsin(pSpr->ang), (!objActor->hasX()) ? 0 : objActor->dudeSlope, arg1, arg3 << 1);
            else if ((var2 & CSTAT_SPRITE_ALIGNMENT_MASK) == CSTAT_SPRITE_ALIGNMENT_FLOOR)
            {
                    var3 = (var2 & 0x0008) ? 0x10000 << 1 : -(0x10000 << 1);
                    var = HitScan(objActor, pSpr->z, Cos(pSpr->ang) >> 16, Sin(pSpr->ang) >> 16, var3, arg1, arg3 << 1);
            }
            else
            {
                    var = HitScan(objActor, pSpr->z, bcos(pSpr->ang), bsin(pSpr->ang), 0, arg1, arg3 << 1);
                }

            if (var >= 0)
            {
                switch (cond)
                {
                        case 35: retn = (var == 1); break;
                        case 36: retn = (var == 2); break;
                        case 37: retn = (var == 0 || var == 4); break;
                        case 38: retn = (var == 3); break;
                    }

                    if (!PUSH) return retn;
                switch (var)
                {
                case 0: case 4: condPush(aCond, gHitInfo.hitWall);    break;
                case 1: case 2: condPush(aCond, gHitInfo.hitSector);    break;
                case 3:         condPush(aCond, gHitInfo.actor());   break;
                    }

                }
                return retn;

            case 45: // this sprite is a target of some dude?
            BloodStatIterator it(kStatDude);
            while (auto iactor = it.Next())
                {
                if (objActor == iactor) continue;

                if (iactor->IsDudeActor() && iactor->hasX())
                {
                    XSPRITE* pXDude = &iactor->x();
                    if (pXDude->health <= 0 || iactor->GetTarget() != objActor) continue;
                    else if (PUSH) condPush(aCond, iactor);
                        return true;
                    }
                }
                return false;
        }
    } 
    else if (pXSpr) 
    {
        switch (cond) 
        {
            default: break;
            case 50: // compare hp (in %)
                if (objActor->IsDudeActor()) var = (pXSpr->sysData2 > 0) ? ClipRange(pXSpr->sysData2 << 4, 1, 65535) : getDudeInfo(pSpr->type)->startHealth << 4;
                else if (pSpr->type == kThingBloodChunks) return condCmp(0, arg1, arg2, cmpOp);
                else if (pSpr->type >= kThingBase && pSpr->type < kThingMax) var = thingInfo[pSpr->type - kThingBase].startHealth << 4;
                return condCmp((kPercFull * pXSpr->health) / ClipLow(var, 1), arg1, arg2, cmpOp);
            case 55: // touching ceil of sector?
                if (objActor->hit.ceilhit.type != kHitSector) return false;
                else if (PUSH) condPush(aCond, objActor->hit.ceilhit.hitSector);
                return true;
            case 56: // touching floor of sector?
                if (objActor->hit.florhit.type != kHitSector) return false;
                else if (PUSH) condPush(aCond, objActor->hit.florhit.hitSector);
                return true;
            case 57: // touching walls of sector?
                if (objActor->hit.hit.type != kHitWall) return false;
                else if (PUSH) condPush(aCond, objActor->hit.hit.hitWall);
                return true;
            case 58: // touching another sprite?
            {
                DBloodActor* actorvar = nullptr;
                // Caution: The hit pointers here may be stale, so be careful with them.
                switch (arg3) 
                {
                    case 0:
                    case 1:
                    if (objActor->hit.florhit.type == kHitSprite) actorvar = objActor->hit.florhit.safeActor();
                        if (arg3 || var >= 0) break;
                    [[fallthrough]];
                    case 2:
                    if (objActor->hit.hit.type == kHitSprite) actorvar = objActor->hit.hit.safeActor();
                        if (arg3 || var >= 0) break;
                    [[fallthrough]];
                    case 3:
                    if (objActor->hit.ceilhit.type == kHitSprite) actorvar = objActor->hit.ceilhit.safeActor();
                        break;
                }
                if (actorvar == nullptr) 
                { 
                    // check if something is touching this sprite
                    BloodSpriteIterator it;
                    while (auto iactor = it.Next())
                    {
                        if (iactor->spr.flags & kHitagRespawn) continue;
                        auto& hit = iactor->hit;
                        switch (arg3) 
                        {
                            case 0:
                            case 1:
                            if (hit.ceilhit.type == kHitSprite && hit.ceilhit.safeActor() == objActor) actorvar = iactor;
                            if (arg3 || actorvar) break;
                            [[fallthrough]];
                            case 2:
                            if (hit.hit.type == kHitSprite && hit.hit.safeActor() == objActor) actorvar = iactor;
                            if (arg3 || actorvar) break;
                            [[fallthrough]];
                            case 3:
                            if (hit.florhit.type == kHitSprite && hit.florhit.safeActor() == objActor) actorvar = iactor;
                                break;
                        }
                    }
                }
                if (actorvar == nullptr) return false;
                else if (PUSH) condPush(aCond, actorvar);
                return true;
            }

            case 65: // compare burn time (in %)
                var = (objActor->IsDudeActor()) ? 2400 : 1200;
                if (!condCmp((kPercFull * pXSpr->burnTime) / var, arg1, arg2, cmpOp)) return false;
                else if (PUSH && objActor->GetBurnSource()) condPush(aCond, objActor->GetBurnSource());
                return true;

            case 66: // any flares stuck in this sprite?
            {
                BloodStatIterator it(kStatFlare);
                while (auto flareactor = it.Next())
                {
                    if (!flareactor->hasX() || (flareactor->spr.flags & kHitagFree))
                        continue;

                    if (flareactor->GetTarget() != objActor) continue;
                    else if (PUSH) condPush(aCond, flareactor);
                    return true;
                }
                return false;
            }
            case 70:
                return condCmp(getSpriteMassBySize(objActor), arg1, arg2, cmpOp); // mass of the sprite in a range?
        }
    }
    else 
{
        switch (cond) 
        {
            default: return false;
            case 50:
            case 65:
            case 70:
                return condCmp(0, arg1, arg2, cmpOp);
        }
    }

    condError(aCond, "Unexpected condition id (%d)!", cond);
    return false;
}

//---------------------------------------------------------------------------
//
// this updates index of object in all conditions
// only used when spawning players
//
//---------------------------------------------------------------------------

void condUpdateObjectIndex(DBloodActor* oldActor, DBloodActor* newActor) 
{
    // update index in tracking conditions first
    for (int i = 0; i < gTrackingCondsCount; i++) 
    {
        TRCONDITION* pCond = &gCondition[i];
        for (unsigned k = 0; k < pCond->length; k++) 
        {
            if (!pCond->obj[k].obj.isActor() || pCond->obj[k].obj.actor() != oldActor) continue;
            pCond->obj[k].obj = EventObject(newActor);
            break;
        }
    }

    // puke...
    auto oldSerial = EventObject(oldActor);
    auto newSerial = EventObject(newActor);

    // then update serials
    BloodStatIterator it(kStatModernCondition);
    while (auto iActor = it.Next())
    {
        if (iActor->condition[0] == oldSerial) iActor->condition[0] = newSerial;
        if (iActor->condition[1] == oldSerial) iActor->condition[1] = newSerial;

    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool modernTypeSetSpriteState(DBloodActor* actor, int nState) 
{
    auto pXSprite = &actor->x();

    if ((pXSprite->busy & 0xffff) == 0 && pXSprite->state == nState)
        return false;

    pXSprite->busy  = IntToFixed(nState);
    pXSprite->state = nState;
    
    evKillActor(actor);
    if (pXSprite->restState != nState && pXSprite->waitTime > 0)
        evPostActor(actor, (pXSprite->waitTime * 120) / 10, pXSprite->restState ? kCmdOn : kCmdOff);

    if (pXSprite->txID != 0 && ((pXSprite->triggerOn && pXSprite->state) || (pXSprite->triggerOff && !pXSprite->state)))
        modernTypeSendCommand(actor, pXSprite->txID, (COMMAND_ID)pXSprite->command);

    return true;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void modernTypeSendCommand(DBloodActor* actor, int destChannel, COMMAND_ID command) 
{
    switch (command) 
    {
    case kCmdLink:
        evSendActor(actor, destChannel, kCmdModernUse); // just send command to change properties
        return;
    case kCmdUnlock:
        evSendActor(actor, destChannel, command); // send normal command first
        evSendActor(actor, destChannel, kCmdModernUse);  // then send command to change properties
        return;
    default:
        evSendActor(actor, destChannel, kCmdModernUse); // send first command to change properties
        evSendActor(actor, destChannel, command); // then send normal command
        return;
    }
}

//---------------------------------------------------------------------------
//
// this function used by various new modern types.
//
//---------------------------------------------------------------------------

void modernTypeTrigger(int destObjType, sectortype* destSect, walltype* destWall, DBloodActor* destactor, EVENT& event) 
{
    if (!event.isActor()) return;
    auto pActor = event.getActor();
    if (!pActor || !pActor->hasX()) return;
    spritetype* pSource = &pActor->s();
    XSPRITE* pXSource = &pActor->x();

    switch (destObjType) {
        case OBJ_SECTOR:
            if (!destSect || !destSect->hasX()) return;
            break;
        case OBJ_WALL:
            if (!destWall || !destWall->hasX()) return;
            break;
        case OBJ_SPRITE:
        {
            if (!destactor) return;
            auto pSpr = &destactor->s();
            if (pSpr->flags & kHitagFree) return;

            // allow redirect events received from some modern types.
            // example: it allows to spawn FX effect if event was received from kModernEffectGen
            // on many TX channels instead of just one.
            switch (pSpr->type) 
            {
                case kModernRandomTX:
                case kModernSequentialTX:
                XSPRITE* pXSpr = &destactor->x();
                    if (pXSpr->command != kCmdLink || pXSpr->locked) break; // no redirect mode detected
                switch (pSpr->type) 
                {
                        case kModernRandomTX:
                    useRandomTx(destactor, (COMMAND_ID)pXSource->command, false); // set random TX id
                            break;
                        case kModernSequentialTX:
                    if (pSpr->flags & kModernTypeFlag1) 
                    {
                        seqTxSendCmdAll(destactor, pActor, (COMMAND_ID)pXSource->command, true);
                                return;
                            }
                    useSequentialTx(destactor, (COMMAND_ID)pXSource->command, false); // set next TX id
                            break;
                    }
                    if (pXSpr->txID <= 0 || pXSpr->txID >= kChannelUserMax) return;
                    modernTypeSendCommand(pActor, pXSpr->txID, (COMMAND_ID)pXSource->command);
                    return;
            }
            break;
        }
        default:
            return;
    }

    switch (pSource->type) 
    {
        // allows teleport any sprite from any location to the source destination
        case kMarkerWarpDest:
            if (destObjType != OBJ_SPRITE) break;
            useTeleportTarget(pActor, destactor);
            break;
        // changes slope of sprite or sector
        case kModernSlopeChanger:
            switch (destObjType) 
            {
                case OBJ_SPRITE:
                case OBJ_SECTOR:
                    useSlopeChanger(pActor, destObjType, destSect, destactor);
                    break;
            }
            break;
        case kModernSpriteDamager:
        // damages xsprite via TX ID or xsprites in a sector
            switch (destObjType) 
            {
                case OBJ_SPRITE:
                case OBJ_SECTOR:
                    useSpriteDamager(pActor, destObjType, destSect, destactor);
            break;
            }
            break;
        // can spawn any effect passed in data2 on it's or txID sprite
        case kModernEffectSpawner:
            if (destObjType != OBJ_SPRITE) break;
            useEffectGen(pActor, destactor);
            break;
        // takes data2 as SEQ ID and spawns it on it's or TX ID object
        case kModernSeqSpawner:
            useSeqSpawnerGen(pActor, destObjType, destSect, destWall, destactor);
            break;
        // creates wind on TX ID sector
        case kModernWindGenerator:
            if (destObjType != OBJ_SECTOR || pXSource->data2 < 0) break;
            useSectorWindGen(pActor, destSect);
            break;
        // size and pan changer of sprite/wall/sector via TX ID
        case kModernObjSizeChanger:
            useObjResizer(pActor, destObjType, destSect, destWall, destactor);
            break;
        // iterate data filed value of destination object
        case kModernObjDataAccumulator:
            useIncDecGen(pActor, destObjType, destSect, destWall, destactor);
            break;
        // change data field value of destination object
        case kModernObjDataChanger:
            useDataChanger(pActor, destObjType, destSect, destWall, destactor);
            break;
        // change sector lighting dynamically
        case kModernSectorFXChanger:
            if (destObjType != OBJ_SECTOR) break;
            useSectorLightChanger(pActor, destSect);
            break;
        // change target of dudes and make it fight
        case kModernDudeTargetChanger:
            if (destObjType != OBJ_SPRITE) break;
            useTargetChanger(pActor, destactor);
            break;
        // change picture and palette of TX ID object
        case kModernObjPicnumChanger:
            usePictureChanger(pActor, destObjType, destSect, destWall, destactor);
            break;
        // change various properties
        case kModernObjPropertiesChanger:
            usePropertiesChanger(pActor, destObjType, destSect, destWall, destactor);
            break;
        // updated vanilla sound gen that now allows to play sounds on TX ID sprites
        case kGenModernSound:
            if (destObjType != OBJ_SPRITE) break;
            useSoundGen(pActor, destactor);
            break;
        // updated ecto skull gen that allows to fire missile from TX ID sprites
        case kGenModernMissileUniversal:
            if (destObjType != OBJ_SPRITE) break;
            useUniMissileGen(pActor, destactor);
            break;
        // spawn enemies on TX ID sprites
        case kMarkerDudeSpawn:
            if (destObjType != OBJ_SPRITE) break;
            useDudeSpawn(pActor, destactor);
            break;
         // spawn custom dude on TX ID sprites
        case kModernCustomDudeSpawn:
            if (destObjType != OBJ_SPRITE) break;
            useCustomDudeSpawn(pActor, destactor);
            break;
    }
}

//---------------------------------------------------------------------------
//
// the following functions required for kModernDudeTargetChanger
//
//---------------------------------------------------------------------------

DBloodActor* aiFightGetTargetInRange(DBloodActor* actor, int minDist, int maxDist, int data, int teamMode) 
{
    auto pSprite = &actor->s();
    XSPRITE* pXSprite = &actor->x();

    DUDEINFO* pDudeInfo = getDudeInfo(pSprite->type);
    
    BloodStatIterator it(kStatDude);
    while (auto targactor = it.Next())
    {
        if (!aiFightDudeCanSeeTarget(actor, pDudeInfo, targactor)) continue;

        auto pXTarget = &targactor->x();

        int dist = aiFightGetTargetDist(actor, pDudeInfo, targactor);
        if (dist < minDist || dist > maxDist) continue;
        else if (actor->GetTarget() == targactor) return targactor;
        else if (!targactor->IsDudeActor() || targactor == actor || targactor->IsPlayerActor()) continue;
        else if (IsBurningDude(targactor) || !IsKillableDude(targactor) || targactor->GetOwner() == actor) continue;
        else if ((teamMode == 1 && pXSprite->rxID == pXTarget->rxID) || aiFightMatesHaveSameTarget(actor, targactor, 1)) continue;
        else if (data == 666 || pXTarget->data1 == data) 
        {
            if (actor->GetTarget())
            {
                int fineDist1 = aiFightGetFineTargetDist(actor, actor->GetTarget());
                int fineDist2 = aiFightGetFineTargetDist(actor, targactor);
                if (fineDist1 < fineDist2)
                    continue;
            }
            return targactor;
        }
    }
    return nullptr;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

DBloodActor* aiFightTargetIsPlayer(DBloodActor* actor)
{
    auto targ = actor->GetTarget();
    if (targ && targ->IsPlayerActor()) return targ;
    return nullptr;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

DBloodActor* aiFightGetMateTargets(DBloodActor* actor)
{
    auto pXSprite = &actor->x();
    int rx = pXSprite->rxID; 

    for (int i = bucketHead[rx]; i < bucketHead[rx + 1]; i++) 
    {
        if (rxBucket[i].isActor()) 
        {
            auto mate = rxBucket[i].actor();
            if (!mate || !mate->hasX() || mate == actor || !mate->IsDudeActor())
                continue;

            if (mate->GetTarget())
            {
                if (!mate->GetTarget()->IsPlayerActor())
                    return mate->GetTarget();
            }
        }
    }
    return nullptr;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool aiFightMatesHaveSameTarget(DBloodActor* leaderactor, DBloodActor* targetactor, int allow) 
{
    auto pXLeader = &leaderactor->x();
    int rx = pXLeader->rxID; 

    for (int i = bucketHead[rx]; i < bucketHead[rx + 1]; i++) 
    {
        if (!rxBucket[i].isActor()) continue;
        auto mate = rxBucket[i].actor();

        if (!mate || !mate->hasX() || mate == leaderactor || !mate->IsDudeActor())
            continue;

        if (mate->GetTarget() == targetactor && allow-- <= 0)
            return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool aiFightDudeCanSeeTarget(DBloodActor* dudeactor, DUDEINFO* pDudeInfo, DBloodActor* targetactor) 
{
    auto pDude = &dudeactor->s();
    auto pTarget = &targetactor->s();

    int dx = pTarget->pos.X - pDude->pos.X; int dy = pTarget->y - pDude->y;

    // check target
    if (approxDist(dx, dy) < pDudeInfo->seeDist) 
    {
        int eyeAboveZ = pDudeInfo->eyeHeight * pDude->yrepeat << 2;

        // is there a line of sight to the target?
        if (cansee(pDude->pos.X, pDude->y, pDude->z, pDude->sector(), pTarget->pos.X, pTarget->y, pTarget->z - eyeAboveZ, pTarget->sector()))
        {
            /*int nAngle = getangle(dx, dy);
            int losAngle = ((1024 + nAngle - pDude->ang) & 2047) - 1024;

            // is the target visible?
            if (abs(losAngle) < 2048) // 360 deg periphery here*/
            return true;
        }

    }

    return false;

}

//---------------------------------------------------------------------------
//
// this function required if monsters in genIdle ai state. It wakes up monsters
// when kModernDudeTargetChanger goes to off state, so they won't ignore the world.
//
//---------------------------------------------------------------------------

void aiFightActivateDudes(int rx) 
{
    for (int i = bucketHead[rx]; i < bucketHead[rx + 1]; i++) 
    {
        if (!rxBucket[i].isActor()) continue;
        auto dudeactor = rxBucket[i].actor();

        if (!dudeactor || !dudeactor->hasX() || !dudeactor->IsDudeActor() || dudeactor->x().aiState->stateType != kAiStateGenIdle) continue;
        aiInitSprite(dudeactor);
    }
}

//---------------------------------------------------------------------------
//
// this function sets target to -1 for all dudes that hunting for nSprite
//
//---------------------------------------------------------------------------

void aiFightFreeTargets(DBloodActor* actor) 
{
    BloodStatIterator it(kStatDude);
    while (auto targetactor = it.Next())
    {
        if (!targetactor->IsDudeActor() || !targetactor->hasX()) continue;
        else if (targetactor->GetTarget() == actor)
            aiSetTarget(targetactor, targetactor->spr.pos.X, targetactor->spr.y, targetactor->spr.z);
    }
}

//---------------------------------------------------------------------------
//
// this function sets target to -1 for all targets that hunting for dudes affected by selected kModernDudeTargetChanger
//
//---------------------------------------------------------------------------

void aiFightFreeAllTargets(DBloodActor* sourceactor) 
{
    auto txID = sourceactor->x().txID;
    if (txID <= 0) return;
    for (int i = bucketHead[txID]; i < bucketHead[txID + 1]; i++) 
    {
        if (!rxBucket[i].isActor()) continue;
        auto actor = rxBucket[i].actor();

        if (actor && actor->hasX())
            aiFightFreeTargets(actor);
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool aiFightDudeIsAffected(DBloodActor* dudeactor) 
{
    auto pXDude = &dudeactor->x();
    if (pXDude->rxID <= 0 || pXDude->locked == 1) return false;
    BloodStatIterator it(kStatModernDudeTargetChanger);
    while (auto actor = it.Next())
    {
        if (!actor->hasX()) continue;
        XSPRITE* pXSprite = &actor->x();
        if (pXSprite->txID <= 0 || pXSprite->state != 1) continue;
        for (int i = bucketHead[pXSprite->txID]; i < bucketHead[pXSprite->txID + 1]; i++) 
        {
            if (!rxBucket[i].isActor()) continue;
            auto rxactor = rxBucket[i].actor();

            if (!rxactor || !rxactor->hasX() || !rxactor->IsDudeActor()) continue;
            else if (rxactor == dudeactor) return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
//
// this function tells if there any dude found for kModernDudeTargetChanger
//
//---------------------------------------------------------------------------

bool aiFightGetDudesForBattle(DBloodActor* actor) 
{
    auto txID = actor->x().txID;
    for (int i = bucketHead[txID]; i < bucketHead[txID + 1]; i++) 
    {
        if (!rxBucket[i].isActor()) continue;
        auto actor = rxBucket[i].actor();
        if (!actor || !actor->hasX() || !actor->IsDudeActor()) continue;
        if (actor->x().health > 0) return true;
    }

    // check redirected TX buckets
    int rx = -1; 
    DBloodActor* pXRedir = nullptr;
    while ((pXRedir = evrListRedirectors(OBJ_SPRITE, nullptr, nullptr, actor, pXRedir, &rx)) != nullptr)
    {
        for (int i = bucketHead[rx]; i < bucketHead[rx + 1]; i++) 
        {
            if (!rxBucket[i].isActor()) continue;
            auto actor = rxBucket[i].actor();
	        if (!actor || !actor->hasX() || !actor->IsDudeActor()) continue;
	        if (actor->x().health > 0) return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiFightAlarmDudesInSight(DBloodActor* actor, int max) 
{
    auto pSprite = &actor->s();

    DUDEINFO* pDudeInfo = getDudeInfo(pSprite->type);

    BloodStatIterator it(kStatDude);
    while (auto dudeactor = it.Next())
    {
        if (dudeactor == actor || !dudeactor->IsDudeActor() || !dudeactor->hasX())
            continue;

        if (aiFightDudeCanSeeTarget(actor, pDudeInfo, dudeactor)) 
        {
            if (dudeactor->GetTarget() != nullptr || dudeactor->x().rxID > 0)
                continue;

            auto pDude = &dudeactor->s();
            aiSetTarget(dudeactor, pDude->pos.X, pDude->y, pDude->z);
            aiActivateDude(dudeactor);
            if (max-- < 1)
                break;
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool aiFightUnitCanFly(DBloodActor* dude) 
{
    return (dude->IsDudeActor() && gDudeInfoExtra[dude->spr.type - kDudeBase].flying);
}

bool aiFightIsMeleeUnit(DBloodActor* dude)
{
    if (dude->spr.type == kDudeModernCustom) return (dude->hasX() && dudeIsMelee(dude));
    else return (dude->IsDudeActor() && gDudeInfoExtra[dude->spr.type - kDudeBase].melee);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

int aiFightGetTargetDist(DBloodActor* actor, DUDEINFO* pDudeInfo, DBloodActor* target) 
{
    int dx = target->spr.pos.X - actor->spr.pos.X;
    int dy = target->spr.y - actor->spr.y;

    int dist = approxDist(dx, dy);
    if (dist <= pDudeInfo->meleeDist) return 0;
    if (dist >= pDudeInfo->seeDist) return 13;
    if (dist <= pDudeInfo->seeDist / 12) return 1;
    if (dist <= pDudeInfo->seeDist / 11) return 2;
    if (dist <= pDudeInfo->seeDist / 10) return 3;
    if (dist <= pDudeInfo->seeDist / 9) return 4;
    if (dist <= pDudeInfo->seeDist / 8) return 5;
    if (dist <= pDudeInfo->seeDist / 7) return 6;
    if (dist <= pDudeInfo->seeDist / 6) return 7;
    if (dist <= pDudeInfo->seeDist / 5) return 8;
    if (dist <= pDudeInfo->seeDist / 4) return 9;
    if (dist <= pDudeInfo->seeDist / 3) return 10;
    if (dist <= pDudeInfo->seeDist / 2) return 11;
    return 12;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

int aiFightGetFineTargetDist(DBloodActor* actor, DBloodActor* target)
{
    int dx = target->spr.pos.X - actor->spr.pos.X;
    int dy = target->spr.y - actor->spr.y;

    int dist = approxDist(dx, dy);
    return dist;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void sectorKillSounds(sectortype* pSector)
{
    BloodSectIterator it(pSector);
    while (auto actor = it.Next())
    {
        if (actor->spr.type != kSoundSector) continue;
        sfxKill3DSound(actor);
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void sectorPauseMotion(sectortype* pSector) 
{
    if (!pSector->hasX()) return;
    XSECTOR* pXSector = &pSector->xs();
    pXSector->unused1 = 1;
    
    evKillSector(pSector);

    sectorKillSounds(pSector);
    if ((pXSector->busy == 0 && !pXSector->state) || (pXSector->busy == 65536 && pXSector->state))
    SectorEndSound(pSector, pXSector->state);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void sectorContinueMotion(sectortype* pSector, EVENT event) 
{
    if (!pSector->hasX()) return;

    XSECTOR* pXSector = &pSector->xs();
    pXSector->unused1 = 0;
    
    int busyTimeA = pXSector->busyTimeA;   
    int waitTimeA = pXSector->waitTimeA;
    int busyTimeB = pXSector->busyTimeB;   
    int waitTimeB = pXSector->waitTimeB;
    if (pSector->type == kSectorPath) 
    {
        if (!pXSector->marker0) return;
        busyTimeA = busyTimeB = pXSector->marker0->x().busyTime;
        waitTimeA = waitTimeB = pXSector->marker0->x().waitTime;
    }
    
    if (!pXSector->interruptable && event.cmd != kCmdSectorMotionContinue
        && ((!pXSector->state && pXSector->busy) || (pXSector->state && pXSector->busy != 65536))) 
    {
            event.cmd = kCmdSectorMotionContinue;
    } 
    else if (event.cmd == kCmdToggle) 
    {
        event.cmd = (pXSector->state) ? kCmdOn : kCmdOff;
    }

    int nDelta = 1;
    switch (event.cmd) 
    {
        case kCmdOff:
            if (pXSector->busy == 0) 
            {
                if (pXSector->reTriggerB && waitTimeB) evPostSector(pSector, (waitTimeB * 120) / 10, kCmdOff);
                return;
            }
            pXSector->state = 1;
            nDelta = 65536 / ClipLow((busyTimeB * 120) / 10, 1);
            break;

        case kCmdOn:
            if (pXSector->busy == 65536) 
            {
                if (pXSector->reTriggerA && waitTimeA) evPostSector(pSector, (waitTimeA * 120) / 10, kCmdOn);
                return;
            }
            pXSector->state = 0;
            nDelta = 65536 / ClipLow((busyTimeA * 120) / 10, 1);
            break;

        case kCmdSectorMotionContinue:
            nDelta = 65536 / ClipLow((((pXSector->state) ? busyTimeB : busyTimeA) * 120) / 10, 1);
            break;
    }

    //bool crush = pXSector->Crush;
    int busyFunc = BUSYID_0;
    switch (pSector->type) 
    {
        case kSectorZMotion:
            busyFunc = BUSYID_2;
            break;
        case kSectorZMotionSprite:
            busyFunc = BUSYID_1;
            break;
        case kSectorSlideMarked:
        case kSectorSlide:
            busyFunc = BUSYID_3;
            break;
        case kSectorRotateMarked:
        case kSectorRotate:
            busyFunc = BUSYID_4;
            break;
        case kSectorRotateStep:
            busyFunc = BUSYID_5;
            break;
        case kSectorPath:
            busyFunc = BUSYID_7;
            break;
        default:
            I_Error("Unsupported sector type %d", pSector->type);
            break;
    }

    SectorStartSound(pSector, pXSector->state);
    nDelta = (pXSector->state) ? -nDelta : nDelta;
    BUSY b = { pSector, nDelta, (int)pXSector->busy, (BUSYID)busyFunc };
    gBusy.Push(b);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool modernTypeOperateSector(sectortype* pSector, const EVENT& event) 
{
    auto pXSector = &pSector->xs();
    if (event.cmd >= kCmdLock && event.cmd <= kCmdToggleLock) 
    {
        switch (event.cmd) 
        {
            case kCmdLock:
                pXSector->locked = 1;
                break;
            case kCmdUnlock:
                pXSector->locked = 0;
                break;
            case kCmdToggleLock:
                pXSector->locked = pXSector->locked ^ 1;
                break;
        }

        switch (pSector->type) 
        {
            case kSectorCounter:
                if (pXSector->locked != 1) break;
                SetSectorState(pSector, 0);
                evPostSector(pSector, 0, kCallbackCounterCheck);
                break;
        }
        return true;
    
    // continue motion of the paused sector
    } 
    else if (pXSector->unused1) 
    {
        switch (event.cmd) 
        {
            case kCmdOff:
            case kCmdOn:
            case kCmdToggle:
            case kCmdSectorMotionContinue:
                sectorContinueMotion(pSector, event);
                return true;
        }
    
    // pause motion of the sector
    }
    else if (event.cmd == kCmdSectorMotionPause) 
    {
        sectorPauseMotion(pSector);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void useCustomDudeSpawn(DBloodActor* pSource, DBloodActor* pSprite)
{
    genDudeSpawn(pSource, pSprite, pSprite->spr.clipdist << 1);
}

void useDudeSpawn(DBloodActor* pSource, DBloodActor* pSprite)
{
    if (randomSpawnDude(pSource, pSprite, pSprite->spr.clipdist << 1, 0) == nullptr)
        nnExtSpawnDude(pSource, pSprite, pSource->x().data1, pSprite->spr.clipdist << 1, 0);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool modernTypeOperateSprite(DBloodActor* actor, EVENT& event) 
{
    auto pSprite = &actor->s();
    auto pXSprite = &actor->x();

    if (event.cmd >= kCmdLock && event.cmd <= kCmdToggleLock) 
    {
        switch (event.cmd) 
        {
            case kCmdLock:
                pXSprite->locked = 1;
                break;
            case kCmdUnlock:
                pXSprite->locked = 0;
                break;
            case kCmdToggleLock:
                pXSprite->locked = pXSprite->locked ^ 1;
                break;
        }

        switch (pSprite->type) 
        {
            case kModernCondition:
            case kModernConditionFalse:
                pXSprite->restState = 0;
                if (pXSprite->busyTime <= 0) break;
                else if (!pXSprite->locked) pXSprite->busy = 0;
                break;
        }
        return true;
    }
    else if (event.cmd == kCmdDudeFlagsSet)
    {
        if (!event.isActor())
        {
            viewSetSystemMessage("Only sprites can use command #%d", event.cmd);
            return true;

        } 
        else
        {
            auto pEvActor = event.getActor();
            if (pEvActor && pEvActor->hasX())
            {

                // copy dude flags from the source to destination sprite
                aiPatrolFlagsMgr(pEvActor, actor, true, false);
            }

        }
    }
    if (pSprite->statnum == kStatDude && actor->IsDudeActor()) 
    {
        switch (event.cmd) 
        {
            case kCmdOff:
                if (pXSprite->state) SetSpriteState(actor, 0);
                break;
            case kCmdOn:
                if (!pXSprite->state) SetSpriteState(actor, 1);
                if (!actor->IsDudeActor() || actor->IsPlayerActor() || pXSprite->health <= 0) break;
                else if (pXSprite->aiState->stateType >= kAiStatePatrolBase && pXSprite->aiState->stateType < kAiStatePatrolMax)
                    break;

                switch (pXSprite->aiState->stateType) 
                {
                    case kAiStateIdle:
                    case kAiStateGenIdle:
                        aiActivateDude(actor);
                        break;
                }
                break;

            case kCmdDudeFlagsSet:
                if (event.isActor())
                {
                    auto pEvActor = event.getActor();
                    if (!pEvActor || !pEvActor->hasX()) break;
                    else aiPatrolFlagsMgr(pEvActor, actor, false, true); // initialize patrol dude with possible new flags
                }
                break;

            default:
                if (!pXSprite->state) evPostActor(actor, 0, kCmdOn);
                else evPostActor(actor, 0, kCmdOff);
                break;
        }
        return true;
    }

    switch (pSprite->type) 
    {
        default:
            return false; // no modern type found to work with, go normal OperateSprite();
        case kThingBloodBits:
        case kThingBloodChunks:
            // dude to thing morphing causing a lot of problems since it continues receiving commands after dude is dead.
            // this leads to weird stuff like exploding with gargoyle gib or corpse disappearing immediately.
            // let's allow only specific commands here to avoid this.
            if (pSprite->inittype < kDudeBase || pSprite->inittype >= kDudeMax) return false;
            else if (event.cmd != kCmdToggle && event.cmd != kCmdOff && event.cmd != kCmdSpriteImpact) return true;
            DudeToGibCallback1(0, actor); // set proper gib type just in case DATAs was changed from the outside.
            return false;

        case kModernCondition:
        case kModernConditionFalse:
            if (!pXSprite->isTriggered) useCondition(actor, event);
            return true;

        // add spawn random dude feature - works only if at least 2 data fields are not empty.
        case kMarkerDudeSpawn:
            if (!gGameOptions.nMonsterSettings) return true;
            else if (!(pSprite->flags & kModernTypeFlag4)) useDudeSpawn(actor, actor);
            else if (pXSprite->txID) evSendActor(actor, pXSprite->txID, kCmdModernUse);
            return true;

        case kModernCustomDudeSpawn:
            if (!gGameOptions.nMonsterSettings) return true;
            else if (!(pSprite->flags & kModernTypeFlag4)) useCustomDudeSpawn(actor, actor);
            else if (pXSprite->txID) evSendActor(actor, pXSprite->txID, kCmdModernUse);
            return true;

        case kModernRandomTX: // random Event Switch takes random data field and uses it as TX ID
        case kModernSequentialTX: // sequential Switch takes values from data fields starting from data1 and uses it as TX ID
            if (pXSprite->command == kCmdLink) return true; // work as event redirector
            switch (pSprite->type) 
            {
                case kModernRandomTX:
                    useRandomTx(actor, (COMMAND_ID)pXSprite->command, true);
                    break;

                case kModernSequentialTX:
                    if (!(pSprite->flags & kModernTypeFlag1)) useSequentialTx(actor, (COMMAND_ID)pXSprite->command, true);
                    else seqTxSendCmdAll(actor, actor, (COMMAND_ID)pXSprite->command, false);
                    break;
            }
            return true;

        case kModernSpriteDamager:
            switch (event.cmd) 
            {
                case kCmdOff:
                    if (pXSprite->state == 1) SetSpriteState(actor, 0);
                    break;
                case kCmdOn:
                    evKillActor(actor); // queue overflow protect
                    if (pXSprite->state == 0) SetSpriteState(actor, 1);
                    [[fallthrough]];
                case kCmdRepeat:
                    if (pXSprite->txID > 0) modernTypeSendCommand(actor, pXSprite->txID, (COMMAND_ID)pXSprite->command);
                    else if (pXSprite->data1 == 0 && pSprite->insector()) useSpriteDamager(actor, OBJ_SECTOR, pSprite->sector(), nullptr);
                    else if (pXSprite->data1 >= 666 && pXSprite->data1 < 669) useSpriteDamager(actor, -1, nullptr, nullptr);
                    else
                    {
                PLAYER* pPlayer = getPlayerById(pXSprite->data1);
                        if (pPlayer != NULL)
                            useSpriteDamager(actor, OBJ_SPRITE, 0, pPlayer->actor);
                    }

                    if (pXSprite->busyTime > 0)
                        evPostActor(actor, pXSprite->busyTime, kCmdRepeat);
                            break;

                default:
                    if (pXSprite->state == 0) evPostActor(actor, 0, kCmdOn);
                    else evPostActor(actor, 0, kCmdOff);
                            break;
                    }
                return true;

        case kMarkerWarpDest:
            if (pXSprite->txID <= 0) {
               
                PLAYER* pPlayer = getPlayerById(pXSprite->data1);
                if (pPlayer != NULL && SetSpriteState(actor, pXSprite->state ^ 1) == 1)
                    useTeleportTarget(actor, pPlayer->actor);
                return true;
            }
            [[fallthrough]];
        case kModernObjPropertiesChanger:
            if (pXSprite->txID <= 0) 
            {
                if (SetSpriteState(actor, pXSprite->state ^ 1) == 1)
                    usePropertiesChanger(actor, -1, nullptr, nullptr, nullptr);
                return true;
            }
            [[fallthrough]];
        case kModernSlopeChanger:
        case kModernObjSizeChanger:
        case kModernObjPicnumChanger:
        case kModernSectorFXChanger:
        case kModernObjDataChanger:
            modernTypeSetSpriteState(actor, pXSprite->state ^ 1);
            return true;

        case kModernSeqSpawner:
        case kModernEffectSpawner:
            switch (event.cmd) 
            {
                case kCmdOff:
                    if (pXSprite->state == 1) SetSpriteState(actor, 0);
                    break;
                case kCmdOn:
                    evKillActor(actor); // queue overflow protect
                    if (pXSprite->state == 0) SetSpriteState(actor, 1);
                    if (pSprite->type == kModernSeqSpawner) seqSpawnerOffSameTx(actor);
                    [[fallthrough]];
                case kCmdRepeat:
                    if (pXSprite->txID > 0) modernTypeSendCommand(actor, pXSprite->txID, (COMMAND_ID)pXSprite->command);
                    else if (pSprite->type == kModernSeqSpawner) useSeqSpawnerGen(actor, OBJ_SPRITE, nullptr, nullptr, actor);
                    else useEffectGen(actor, nullptr);
            
                    if (pXSprite->busyTime > 0)
                        evPostActor(actor, ClipLow((int(pXSprite->busyTime) + Random2(pXSprite->data1)) * 120 / 10, 0), kCmdRepeat);
                    break;
                default:
                    if (pXSprite->state == 0) evPostActor(actor, 0, kCmdOn);
                    else evPostActor(actor, 0, kCmdOff);
                    break;
            }
            return true;

        case kModernWindGenerator:
            switch (event.cmd) 
            {
                case kCmdOff:
                    windGenStopWindOnSectors(actor);
                    if (pXSprite->state == 1) SetSpriteState(actor, 0);
                    break;
                case kCmdOn:
                    evKillActor(actor); // queue overflow protect
                    if (pXSprite->state == 0) SetSpriteState(actor, 1);
                    [[fallthrough]];
                case kCmdRepeat:
                    if (pXSprite->txID > 0) modernTypeSendCommand(actor, pXSprite->txID, (COMMAND_ID)pXSprite->command);
                    else useSectorWindGen(actor, nullptr);

                    if (pXSprite->busyTime > 0) evPostActor(actor, pXSprite->busyTime, kCmdRepeat);
                    break;
                default:
                    if (pXSprite->state == 0) evPostActor(actor, 0, kCmdOn);
                    else evPostActor(actor, 0, kCmdOff);
                    break;
            }
            return true;
        case kModernDudeTargetChanger:

            // this one is required if data4 of generator was dynamically changed
            // it turns monsters in normal idle state instead of genIdle, so they not ignore the world.
            if (pXSprite->dropMsg == 3 && 3 != pXSprite->data4)
                aiFightActivateDudes(pXSprite->txID);

            switch (event.cmd) 
            {
                case kCmdOff:
                    if (pXSprite->data4 == 3) aiFightActivateDudes(pXSprite->txID);
                    if (pXSprite->state == 1) SetSpriteState(actor, 0);
                    break;
                case kCmdOn:
                    evKillActor(actor); // queue overflow protect
                    if (pXSprite->state == 0) SetSpriteState(actor, 1);
                    [[fallthrough]];
                case kCmdRepeat:
                    if (pXSprite->txID <= 0 || !aiFightGetDudesForBattle(actor)) 
                    {
                        aiFightFreeAllTargets(actor);
                        evPostActor(actor, 0, kCmdOff);
                        break;
                    }
                    else 
                    {
                        modernTypeSendCommand(actor, pXSprite->txID, (COMMAND_ID)pXSprite->command);
                    }

                    if (pXSprite->busyTime > 0) evPostActor(actor, pXSprite->busyTime, kCmdRepeat);
                    break;
                default:
                    if (pXSprite->state == 0) evPostActor(actor, 0, kCmdOn);
                    else evPostActor(actor, 0, kCmdOff);
                    break;
            }
            pXSprite->dropMsg = uint8_t(pXSprite->data4);
            return true;

        case kModernObjDataAccumulator:
            switch (event.cmd) {
                case kCmdOff:
                    if (pXSprite->state == 1) SetSpriteState(actor, 0);
                    break;
                case kCmdOn:
                    evKillActor(actor); // queue overflow protect
                    if (pXSprite->state == 0) SetSpriteState(actor, 1);
                    [[fallthrough]];
                case kCmdRepeat:
                    // force OFF after *all* TX objects reach the goal value
                    if (pSprite->flags == kModernTypeFlag0 && incDecGoalValueIsReached(actor)) 
                    {
                        evPostActor(actor, 0, kCmdOff);
                        break;
                    }
                    
                    modernTypeSendCommand(actor, pXSprite->txID, (COMMAND_ID)pXSprite->command);
                    if (pXSprite->busyTime > 0) evPostActor(actor, pXSprite->busyTime, kCmdRepeat);
                    break;
                default:
                    if (pXSprite->state == 0) evPostActor(actor, 0, kCmdOn);
                    else evPostActor(actor, 0, kCmdOff);
                    break;
            }
            return true;
        case kModernRandom:
        case kModernRandom2:
            switch (event.cmd) 
            {
                case kCmdOff:
                    if (pXSprite->state == 1) SetSpriteState(actor, 0);
                    break;
                case kCmdOn:
                    evKillActor(actor); // queue overflow protect
                    if (pXSprite->state == 0) SetSpriteState(actor, 1);
                    [[fallthrough]];
                case kCmdRepeat:
                    useRandomItemGen(actor);
                    if (pXSprite->busyTime > 0)
                        evPostActor(actor, (120 * pXSprite->busyTime) / 10, kCmdRepeat);
                    break;
                default:
                    if (pXSprite->state == 0) evPostActor(actor, 0, kCmdOn);
                    else evPostActor(actor, 0, kCmdOff);
                    break;
            }
            return true;

        case kModernThingTNTProx:
            if (pSprite->statnum != kStatRespawn) 
            {
                switch (event.cmd) 
                {
                case kCmdSpriteProximity:
                    if (pXSprite->state) break;
                    sfxPlay3DSound(actor, 452, 0, 0);
                    evPostActor(actor, 30, kCmdOff);
                    pXSprite->state = 1;
                    [[fallthrough]];
                case kCmdOn:
                    sfxPlay3DSound(actor, 451, 0, 0);
                    pXSprite->Proximity = 1;
                    break;
                default:
                    actExplodeSprite(actor);
                    break;
                }
            }
            return true;
        case kModernThingEnemyLifeLeech:
            dudeLeechOperate(actor, event);
            return true;
        case kModernPlayerControl: { // WIP
            PLAYER* pPlayer = NULL; int cmd = (event.cmd >= kCmdNumberic) ? event.cmd : pXSprite->command;
            if ((pPlayer = getPlayerById(pXSprite->data1)) == NULL
                    || ((cmd < 67 || cmd > 68) && !modernTypeSetSpriteState(actor, pXSprite->state ^ 1)))
                        return true;

            TRPLAYERCTRL* pCtrl = &gPlayerCtrl[pPlayer->nPlayer];

            /// !!! COMMANDS OF THE CURRENT SPRITE, NOT OF THE EVENT !!! ///
            if ((cmd -= kCmdNumberic) < 0) return true;
            else if (pPlayer->pXSprite->health <= 0) 
            {
                        
                switch (cmd) {
                    case 36:
                        actHealDude(pPlayer->actor, ((pXSprite->data2 > 0) ? ClipHigh(pXSprite->data2, 200) : getDudeInfo(pPlayer->pSprite->type)->startHealth), 200);
                        pPlayer->curWeapon = kWeapPitchFork;
                        break;
                }
                return true;
            }

            switch (cmd) 
            {
                case 0: // 64 (player life form)
                    if (pXSprite->data2 < kModeHuman || pXSprite->data2 > kModeHumanGrown) break;
                    else trPlayerCtrlSetRace(pXSprite->data2, pPlayer);
                    break;
                case 1: // 65 (move speed and jump height)
                    // player movement speed (for all races and postures)
                    if (valueIsBetween(pXSprite->data2, -1, 32767))
                        trPlayerCtrlSetMoveSpeed(pXSprite->data2, pPlayer);

                    // player jump height (for all races and stand posture only)
                    if (valueIsBetween(pXSprite->data3, -1, 32767))
                        trPlayerCtrlSetJumpHeight(pXSprite->data3, pPlayer);
                    break;
                case 2: // 66 (player screen effects)
                    if (pXSprite->data3 < 0) break;
                    else trPlayerCtrlSetScreenEffect(pXSprite->data2, pXSprite->data3, pPlayer);
                    break;
                case 3: // 67 (start playing qav scene)
                    trPlayerCtrlStartScene(actor, pPlayer, (pXSprite->data4 == 1) ? true : false);
                    break;
                case 4: // 68 (stop playing qav scene)
                    if (pXSprite->data2 > 0 && pXSprite->data2 != pPlayer->sceneQav) break;
                    else trPlayerCtrlStopScene(pPlayer);
                    break;
                case 5: // 69 (set player look angle, TO-DO: if tx > 0, take a look on TX ID sprite)
                    //data4 is reserved
                    if (pXSprite->data4 != 0) break;
                    else if (valueIsBetween(pXSprite->data2, -128, 128))
                        trPlayerCtrlSetLookAngle(pXSprite->data2, pPlayer);
                    break;
                case 6: // 70 (erase player stuff...)
                    if (pXSprite->data2 < 0) break;
                    else trPlayerCtrlEraseStuff(pXSprite->data2, pPlayer);
                    break;
                case 7: // 71 (give something to player...)
                    if (pXSprite->data2 <= 0) break;
                    else trPlayerCtrlGiveStuff(pXSprite->data2, pXSprite->data3,pXSprite->data4, pPlayer, pCtrl);
                    break;
                case 8: // 72 (use inventory item)
                    if (pXSprite->data2 < 1 || pXSprite->data2 > 5) break;
                    else trPlayerCtrlUsePackItem(pXSprite->data2, pXSprite->data3, pXSprite->data4, pPlayer, event.cmd);
                    break;
                case 9: // 73 (set player's sprite angle, TO-DO: if tx > 0, take a look on TX ID sprite)
                    //data4 is reserved
                    if (pXSprite->data4 != 0) break;
                    else if (pSprite->flags & kModernTypeFlag1) 
                    {
                        pPlayer->angle.settarget(pSprite->ang);
                        pPlayer->angle.lockinput();
                    }
                    else if (valueIsBetween(pXSprite->data2, -kAng360, kAng360)) 
                    {
                        pPlayer->angle.settarget(pXSprite->data2);
                        pPlayer->angle.lockinput();
                    }
                    break;
                case 10: // 74 (de)activate powerup
                    if (pXSprite->data2 <= 0 || pXSprite->data2 > (kMaxAllowedPowerup - (kMinAllowedPowerup << 1) + 1)) break;
                    trPlayerCtrlUsePowerup(actor, pPlayer, event.cmd);
                    break;
               // case 11: // 75 (print the book)
                    // data2: RFF TXT id
                    // data3: background tile
                    // data4: font base tile
                    // pal: font / background palette
                    // hitag:
                    // d1: 0: print whole text at a time, 1: print line by line, 2: word by word, 3: letter by letter
                    // d2: 1: force pause the game (sp only)
                    // d3: 1: inherit palette for font, 2: inherit palette for background, 3: both
                    // busyTime: speed of word/letter/line printing
                    // waitTime: if TX ID > 0 and TX ID object is book reader, trigger it?
                    //break;

            }
        }
        return true;

        case kGenModernSound:
            switch (event.cmd) {
                case kCmdOff:
                    if (pXSprite->state == 1) SetSpriteState(actor, 0);
                    break;
                case kCmdOn:
                    evKillActor(actor); // queue overflow protect
                    if (pXSprite->state == 0) SetSpriteState(actor, 1);
                    [[fallthrough]];
                case kCmdRepeat:
                if (pXSprite->txID)  modernTypeSendCommand(actor, pXSprite->txID, (COMMAND_ID)pXSprite->command);
                else useSoundGen(actor, actor);
                
                if (pXSprite->busyTime > 0)
                    evPostActor(actor, (120 * pXSprite->busyTime) / 10, kCmdRepeat);
                            break;
            default:
                if (pXSprite->state == 0) evPostActor(actor, 0, kCmdOn);
                else evPostActor(actor, 0, kCmdOff);
                            break;
                    }
            return true;
        case kGenModernMissileUniversal:
            switch (event.cmd) 
            {
                case kCmdOff:
                    if (pXSprite->state == 1) SetSpriteState(actor, 0);
                    break;
                case kCmdOn:
                    evKillActor(actor); // queue overflow protect
                    if (pXSprite->state == 0) SetSpriteState(actor, 1);
                    [[fallthrough]];
                case kCmdRepeat:
                    if (pXSprite->txID)  modernTypeSendCommand(actor, pXSprite->txID, (COMMAND_ID)pXSprite->command);
                    else useUniMissileGen(actor, actor);
                    
                    if (pXSprite->busyTime > 0)
                        evPostActor(actor, (120 * pXSprite->busyTime) / 10, kCmdRepeat);

                    break;
                default:
                    if (pXSprite->state == 0) evPostActor(actor, 0, kCmdOn);
                    else evPostActor(actor, 0, kCmdOff);
                    break;
            }
            return true;
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool modernTypeOperateWall(walltype* pWall, const EVENT& event) 
{
    switch (pWall->type) 
    {
        case kSwitchOneWay:
            switch (event.cmd) {
                case kCmdOff:
                    SetWallState(pWall, 0);
                    break;
                case kCmdOn:
                    SetWallState(pWall, 1);
                    break;
                default:
                    SetWallState(pWall, pWall->xw().restState ^ 1);
                    break;
            }
            return true;
        default:
            return false; // no modern type found to work with, go normal OperateWall();
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool txIsRanged(DBloodActor* source) 
{
    if (!source->hasX()) return false;
    auto pXSource = &source->x();
    if (pXSource->data1 > 0 && pXSource->data2 <= 0 && pXSource->data3 <= 0 && pXSource->data4 > 0) 
    {
        if (pXSource->data1 > pXSource->data4) 
        {
            // data1 must be less than data4
            int tmp = pXSource->data1; pXSource->data1 = pXSource->data4;
            pXSource->data4 = tmp;
        }
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void seqTxSendCmdAll(DBloodActor* source, DBloodActor* actor, COMMAND_ID cmd, bool modernSend) 
{
    bool ranged = txIsRanged(source);
    auto pXSource = &source->x();
    if (ranged)
    {
        for (pXSource->txID = pXSource->data1; pXSource->txID <= pXSource->data4; pXSource->txID++) 
        {
            if (pXSource->txID <= 0 || pXSource->txID >= kChannelUserMax) continue;
            else if (!modernSend) evSendActor(actor, pXSource->txID, cmd);
            else modernTypeSendCommand(actor, pXSource->txID, cmd);
        }
    } 
    else 
    {
        for (int i = 0; i <= 3; i++) 
        {
            pXSource->txID = GetDataVal(source, i);
            if (pXSource->txID <= 0 || pXSource->txID >= kChannelUserMax) continue;
            else if (!modernSend) evSendActor(actor, pXSource->txID, cmd);
            else modernTypeSendCommand(actor, pXSource->txID, cmd);
        }
    }
    pXSource->txID = pXSource->sysData1 = 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useRandomTx(DBloodActor* sourceactor, COMMAND_ID cmd, bool setState) 
{
    auto pXSource = &sourceactor->x();
    int tx = 0; int maxRetries = kMaxRandomizeRetries;
    
    if (txIsRanged(sourceactor)) 
    {
        while (maxRetries-- >= 0) 
        {
            if ((tx = nnExtRandom(pXSource->data1, pXSource->data4)) != pXSource->txID)
                break;
        }
    } 
    else 
    {
        while (maxRetries-- >= 0) 
        {
            if ((tx = randomGetDataValue(sourceactor, kRandomizeTX)) > 0 && tx != pXSource->txID)
                break;
        }
    }

    pXSource->txID = (tx > 0 && tx < kChannelUserMax) ? tx : 0;
    if (setState)
        SetSpriteState(sourceactor, pXSource->state ^ 1);
        //evSendActor(pSource->index, pXSource->txID, (COMMAND_ID)pXSource->command);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useSequentialTx(DBloodActor* sourceactor, COMMAND_ID cmd, bool setState) 
{
    auto pXSource = &sourceactor->x();

    bool range = txIsRanged(sourceactor); int cnt = 3; int tx = 0;

    if (range) 
    {
        // make sure sysData is correct as we store current index of TX ID here.
        if (pXSource->sysData1 < pXSource->data1) pXSource->sysData1 = pXSource->data1;
        else if (pXSource->sysData1 > pXSource->data4) pXSource->sysData1 = pXSource->data4;

    }
    else 
    {
        // make sure sysData is correct as we store current index of data field here.
        if (pXSource->sysData1 > 3) pXSource->sysData1 = 0;
        else if (pXSource->sysData1 < 0) pXSource->sysData1 = 3;
    }

    switch (cmd) 
    {
        case kCmdOff:
            if (!range) 
            {
                while (cnt-- >= 0) // skip empty data fields
                {
                    if (pXSource->sysData1-- < 0) pXSource->sysData1 = 3;
                    if ((tx = GetDataVal(sourceactor, pXSource->sysData1)) <= 0) continue;
                    else break;
                }
            } 
            else 
            {
                if (--pXSource->sysData1 < pXSource->data1) pXSource->sysData1 = pXSource->data4;
                tx = pXSource->sysData1;
            }
            break;

        default:
            if (!range) 
            {
                while (cnt-- >= 0) // skip empty data fields
                {
                    if (pXSource->sysData1 > 3) pXSource->sysData1 = 0;
                    if ((tx = GetDataVal(sourceactor, pXSource->sysData1++)) <= 0) continue;
                    else break;
                }
            } 
            else 
            {
                tx = pXSource->sysData1;
                if (pXSource->sysData1 >= pXSource->data4) 
                {
                    pXSource->sysData1 = pXSource->data1;
                    break;
                }
                pXSource->sysData1++;
            }
            break;
    }

    pXSource->txID = (tx > 0 && tx < kChannelUserMax) ? tx : 0;
    if (setState)
        SetSpriteState(sourceactor, pXSource->state ^ 1);
        //evSendActor(pSource->index, pXSource->txID, (COMMAND_ID)pXSource->command);

}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int useCondition(DBloodActor* sourceactor, EVENT& event)
{
    spritetype* pSource = &sourceactor->s();
    auto pXSource = &sourceactor->x();

    bool srcIsCondition = false;

    auto const pActor = event.isActor() ? event.getActor() : nullptr;

    if (event.isActor() && pActor == nullptr) return -1;
    if (event.isActor() && pActor != sourceactor)
        srcIsCondition = (pActor->spr.type == kModernCondition || pActor->spr.type == kModernConditionFalse);

    // if it's a tracking condition, it must ignore all the commands sent from objects
    if (pXSource->busyTime > 0 && event.funcID != kCallbackMax) return -1;
    else if (!srcIsCondition) // save object serials in the stack and make copy of initial object
    {
        condPush(sourceactor, event.target);
        condBackup(sourceactor);
    } 
    else  // or grab serials of objects from previous conditions
    {
        sourceactor->condition[0] = pActor->condition[0];
        sourceactor->condition[1] = pActor->condition[1];

    }
    
    int cond = pXSource->data1; 
    bool ok = false; 
    bool RVRS = (pSource->type == kModernConditionFalse);
    bool RSET = (pXSource->command == kCmdNumberic + 36); 
    bool PUSH = (pXSource->command == kCmdNumberic);
    int comOp = pSource->cstat; // comparison operator

    if (pXSource->restState == 0) 
    {
        if (cond == 0) ok = true; // dummy
        else if (cond >= kCondGameBase && cond < kCondGameMax) ok = condCheckGame(sourceactor, event, comOp, PUSH);
        else if (cond >= kCondMixedBase && cond < kCondMixedMax) ok = condCheckMixed(sourceactor, event, comOp, PUSH);
        else if (cond >= kCondWallBase && cond < kCondWallMax) ok = condCheckWall(sourceactor, comOp, PUSH);
        else if (cond >= kCondSectorBase && cond < kCondSectorMax) ok = condCheckSector(sourceactor, comOp, PUSH);
        else if (cond >= kCondPlayerBase && cond < kCondPlayerMax) ok = condCheckPlayer(sourceactor, comOp, PUSH);
        else if (cond >= kCondDudeBase && cond < kCondDudeMax) ok = condCheckDude(sourceactor, comOp, PUSH);
        else if (cond >= kCondSpriteBase && cond < kCondSpriteMax) ok = condCheckSprite(sourceactor, comOp, PUSH);
        else condError(sourceactor,"Unexpected condition id %d!", cond);

        pXSource->state = (ok ^ RVRS);
        
        if (pXSource->waitTime > 0 && pXSource->state > 0) 
        {
            pXSource->restState = 1;
            evKillActor(sourceactor);
            evPostActor(sourceactor, (pXSource->waitTime * 120) / 10, kCmdRepeat);
            return -1;
        }
    } 
    else if (event.cmd == kCmdRepeat) 
    {
        pXSource->restState = 0;
    }
    else 
    {
        return -1;
    }

    if (pXSource->state) 
    {
        pXSource->isTriggered = pXSource->triggerOnce;
        
        if (RSET)
            condRestore(sourceactor); // reset focus to the initial object

        // send command to rx bucket
        if (pXSource->txID)
            evSendActor(sourceactor, pXSource->txID, (COMMAND_ID)pXSource->command);

        if (pSource->flags) {
        
            // send it for object currently in the focus
            if (pSource->flags & kModernTypeFlag1)
            {
                nnExtTriggerObject(event.target, pXSource->command);
            }

            // send it for initial object
            if ((pSource->flags & kModernTypeFlag2) && (sourceactor->condition[0] != sourceactor->condition[1] || !(pSource->hitag & kModernTypeFlag1))) 
            {
                auto co = condGet(sourceactor);
                nnExtTriggerObject(co, pXSource->command);
            }
        }
    }
    return pXSource->state;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useRandomItemGen(DBloodActor* actor) 
{
    spritetype* pSource = &actor->s(); 
    XSPRITE* pXSource = &actor->x();

    // let's first search for previously dropped items and remove it
    if (pXSource->dropMsg > 0) 
    {
        BloodStatIterator it(kStatItem);
        while (auto iactor = it.Next())
        {
            spritetype* pItem = &iactor->s();
            if ((unsigned int)pItem->type == pXSource->dropMsg && pItem->pos.X == pSource->pos.X && pItem->y == pSource->y && pItem->z == pSource->z) 
            {
                gFX.fxSpawnActor((FX_ID)29, pSource->sector(), pSource->pos.X, pSource->y, pSource->z, 0);
                pItem->type = kSpriteDecoration;
                actPostSprite(iactor, kStatFree);
                break;
            }
        }
    }

    // then drop item
    auto dropactor = randomDropPickupObject(actor, pXSource->dropMsg);

    if (dropactor != nullptr) 
    {
        clampSprite(dropactor);

        // check if generator affected by physics
        if (debrisGetIndex(actor) != -1) 
        {
            dropactor->addX();
            int nIndex = debrisGetFreeIndex();
            if (nIndex >= 0)
            {
                dropactor->x().physAttr |= kPhysMove | kPhysGravity | kPhysFalling; // must fall always
                pSource->cstat &= ~CSTAT_SPRITE_BLOCK;

                gPhysSpritesList[nIndex] = dropactor;
                if (nIndex >= gPhysSpritesCount) gPhysSpritesCount++;
                getSpriteMassBySize(dropactor); // create mass cache
            }
        }
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useUniMissileGen(DBloodActor* sourceactor, DBloodActor* actor) 
{
    if (actor == nullptr) actor = sourceactor;
    XSPRITE* pXSource = &sourceactor->x();
    spritetype* pSource = &sourceactor->s();
    spritetype* pSprite = &actor->s();

    int dx = 0, dy = 0, dz = 0;

    if (pXSource->data1 < kMissileBase || pXSource->data1 >= kMissileMax)
        return;

    if (pSprite->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR) 
    {
        if (pSprite->cstat & CSTAT_SPRITE_YFLIP) dz = 0x4000;
        else dz = -0x4000;
    }
    else 
    {
        dx = bcos(pSprite->ang);
        dy = bsin(pSprite->ang);
        dz = pXSource->data3 << 6; // add slope controlling
        if (dz > 0x10000) dz = 0x10000;
        else if (dz < -0x10000) dz = -0x10000;
    }

    auto missileactor = actFireMissile(actor, 0, 0, dx, dy, dz, actor->x().data1);
    if (missileactor) 
    {
        auto pMissile = &missileactor->s();
        int from; // inherit some properties of the generator
        if ((from = (pSource->flags & kModernTypeFlag3)) > 0) 
        {
            int canInherit = 0xF;
            if (missileactor->hasX() && seqGetStatus(missileactor) >= 0) 
            {
                canInherit &= ~0x8;
               
                SEQINST* pInst = GetInstance(missileactor); 
                Seq* pSeq = pInst->pSequence;
                for (int i = 0; i < pSeq->nFrames; i++) 
                {
                    if ((canInherit & 0x4) && pSeq->frames[i].palette != 0) canInherit &= ~0x4;
                    if ((canInherit & 0x2) && pSeq->frames[i].xrepeat != 0) canInherit &= ~0x2;
                    if ((canInherit & 0x1) && pSeq->frames[i].yrepeat != 0) canInherit &= ~0x1;
                }
            }

            if (canInherit != 0) 
            {
                if (canInherit & 0x2)
                    pMissile->xrepeat = (from == kModernTypeFlag1) ? pSource->xrepeat : pSprite->xrepeat;
                
                if (canInherit & 0x1)
                    pMissile->yrepeat = (from == kModernTypeFlag1) ? pSource->yrepeat : pSprite->yrepeat;

                if (canInherit & 0x4)
                    pMissile->pal = (from == kModernTypeFlag1) ? pSource->pal : pSprite->pal;
                
                if (canInherit & 0x8)
                    pMissile->shade = (from == kModernTypeFlag1) ? pSource->shade : pSprite->shade;
            }
        }

        // add velocity controlling
        if (pXSource->data2 > 0) 
        {
            int velocity = pXSource->data2 << 12;
            missileactor->xvel = MulScale(velocity, dx, 14);
            missileactor->yvel = MulScale(velocity, dy, 14);
            missileactor->zvel = MulScale(velocity, dz, 14);
        }

        // add bursting for missiles
        if (pMissile->type != kMissileFlareAlt && pXSource->data4 > 0)
            evPostActor(missileactor, ClipHigh(pXSource->data4, 500), kCallbackMissileBurst);
    }

}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useSoundGen(DBloodActor* sourceactor, DBloodActor* actor)
{
    int pitch = sourceactor->x().data4 << 1; 
    if (pitch < 2000) pitch = 0;
    sfxPlay3DSoundCP(actor, sourceactor->x().data2, -1, 0, pitch, sourceactor->x().data3);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useIncDecGen(DBloodActor* sourceactor, int objType, sectortype* destSect, walltype* destWall, DBloodActor* objactor) 
{
    auto pXSource = &sourceactor->x();
    spritetype* pSource = &sourceactor->s();

    char buffer[7]; 
    int data = -65535; 
    short tmp = 0; 
    int dataIndex = 0;
    snprintf(buffer, 7, "%d", abs(pXSource->data1)); 
    int len = int(strlen(buffer));
    
    for (int i = 0; i < len; i++) 
    {
        dataIndex = (buffer[i] - 52) + 4;
        if ((data = getDataFieldOfObject(objType, destSect, destWall, objactor, dataIndex)) == -65535) 
        {
            Printf(PRINT_HIGH, "\nWrong index of data (%c) for IncDec Gen! Only 1, 2, 3 and 4 indexes allowed!\n", buffer[i]);
            continue;
        }
        
        if (pXSource->data2 < pXSource->data3) 
        {
            data = ClipRange(data, pXSource->data2, pXSource->data3);
            if ((data += pXSource->data4) >= pXSource->data3) 
            {
                switch (pSource->flags) 
                {
                case kModernTypeFlag0:
                case kModernTypeFlag1:
                    if (data > pXSource->data3) data = pXSource->data3;
                    break;
                case kModernTypeFlag2:
                    if (data > pXSource->data3) data = pXSource->data3;
                    if (!incDecGoalValueIsReached(sourceactor)) break;
                    tmp = pXSource->data3;
                    pXSource->data3 = pXSource->data2;
                    pXSource->data2 = tmp;
                    break;
                case kModernTypeFlag3:
                    if (data > pXSource->data3) data = pXSource->data2;
                    break;
                }
            }

        }
        else if (pXSource->data2 > pXSource->data3) 
        {
            data = ClipRange(data, pXSource->data3, pXSource->data2);
            if ((data -= pXSource->data4) <= pXSource->data3) 
            {
                switch (pSource->flags) 
                {
                case kModernTypeFlag0:
                case kModernTypeFlag1:
                    if (data < pXSource->data3) data = pXSource->data3;
                    break;
                case kModernTypeFlag2:
                    if (data < pXSource->data3) data = pXSource->data3;
                    if (!incDecGoalValueIsReached(sourceactor)) break;
                    tmp = pXSource->data3;
                    pXSource->data3 = pXSource->data2;
                    pXSource->data2 = tmp;
                    break;
                case kModernTypeFlag3:
                    if (data < pXSource->data3) data = pXSource->data2;
                    break;
                }
            }
        }
        pXSource->sysData1 = data;
        setDataValueOfObject(objType, destSect, destWall, objactor, dataIndex, data);
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void sprite2sectorSlope(DBloodActor* actor, sectortype* pSector, char rel, bool forcez) 
{
    auto pSprite = &actor->s();
    int slope = 0, z = 0;
    switch (rel) {
        default:
            z = getflorzofslopeptr(pSprite->sector(), pSprite->pos.X, pSprite->y);
            if ((pSprite->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR) && actor->hasX() && actor->x().Touch) z--;
            slope = pSector->floorheinum;
            break;
        case 1:
            z = getceilzofslopeptr(pSprite->sector(), pSprite->pos.X, pSprite->y);
            if ((pSprite->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR) && actor->hasX() && actor->x().Touch) z++;
            slope = pSector->ceilingheinum;
            break;
    }

    spriteSetSlope(pSprite, slope);
    if (forcez) pSprite->z = z;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useSlopeChanger(DBloodActor* sourceactor, int objType, sectortype* pSect, DBloodActor* objActor) 
{
    auto pXSource = &sourceactor->x();
    spritetype* pSource = &sourceactor->s();

    int slope, oslope;
    bool flag2 = (pSource->flags & kModernTypeFlag2);

    if (pSource->flags & kModernTypeFlag1) slope = ClipRange(pXSource->data2, -32767, 32767);
    else slope = (32767 / kPercFull) * ClipRange(pXSource->data2, -kPercFull, kPercFull);

    if (objType == OBJ_SECTOR) 
    {
        switch (pXSource->data1)
        {
            case 2:
            case 0:

            // just set floor slope
            if (flag2)
            {
                pSect->setfloorslope(slope);
            }
            else
            {
                // force closest floor aligned sprites to inherit slope of the sector's floor
                oslope = pSect->floorheinum;
                BloodSectIterator it(pSect);
                while (auto iactor = it.Next())
                {
                    auto spr = &iactor->s();
                    if (!(spr->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR)) continue;
                    else if (getflorzofslopeptr(pSect, spr->pos.X, spr->y) - kSlopeDist <= spr->z)
                    {
                        sprite2sectorSlope(iactor, pSect, 0, true);

                        // set temporary slope of floor
                        pSect->floorheinum = slope;

                        // force sloped sprites to be on floor slope z
                        sprite2sectorSlope(iactor, pSect, 0, true);

                        // restore old slope for next sprite
                        pSect->floorheinum = oslope;
                }
                }

                // finally set new slope of floor
                pSect->setfloorslope(slope);

            }

                if (pXSource->data1 == 0) break;
                [[fallthrough]];
            case 1:

            // just set ceiling slope
            if (flag2)
            {
                pSect->setceilingslope(slope);
            }
            else
            { 
                oslope = pSect->ceilingheinum;
                BloodSectIterator it(pSect);
                while (auto iactor = it.Next())
                {
                    auto spr = &iactor->s();
                    if (!(spr->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR)) continue;
                    else if (getceilzofslopeptr(pSect, spr->pos.X, spr->y) + kSlopeDist >= spr->z)
                    {
                        sprite2sectorSlope(iactor, pSect, 1, true);

                        // set new slope of ceiling
                        pSect->ceilingheinum = slope;

                        // force sloped sprites to be on ceiling slope z
                        sprite2sectorSlope(iactor, pSect, 1, true);

                        // restore old slope for next sprite
                        pSect->ceilingheinum = oslope;
                }
                }

                // finally set new slope of ceiling
                pSect->setceilingslope(slope);

            }
                break;
        }

        // let's give a little impulse to the physics sprites...
        BloodSectIterator it(pSect);
        while (auto iactor = it.Next())
        {
            auto spr = &iactor->s();
            auto xspr = &iactor->x();
            if (iactor->hasX() && xspr->physAttr > 0) 
            {
                xspr->physAttr |= kPhysFalling;
                iactor->zvel++;
            } 
            else if ((spr->statnum == kStatThing || spr->statnum == kStatDude) && (spr->flags & kPhysGravity))
            {
                spr->flags |= kPhysFalling;
                iactor->zvel++;
                }
            }
    } 
    else if (objType == OBJ_SPRITE) 
    {
        spritetype* pSpr = &objActor->s();
        if (!(pSpr->cstat & CSTAT_SPRITE_ALIGNMENT_FLOOR)) pSpr->cstat |= CSTAT_SPRITE_ALIGNMENT_FLOOR;
        if ((pSpr->cstat & CSTAT_SPRITE_ALIGNMENT_SLOPE) != CSTAT_SPRITE_ALIGNMENT_SLOPE)
            pSpr->cstat |= CSTAT_SPRITE_ALIGNMENT_SLOPE;

        switch (pXSource->data4) 
        {
            case 1:
            case 2:
            case 3:
                if (!pSpr->insector()) break;
                switch (pXSource->data4) 
                {
                    case 1: sprite2sectorSlope(objActor, pSpr->sector(), 0, flag2); break;
                    case 2: sprite2sectorSlope(objActor, pSpr->sector(), 1, flag2); break;
                    case 3:
                        if (getflorzofslopeptr(pSpr->sector(), pSpr->pos.X, pSpr->y) - kSlopeDist <= pSpr->z) sprite2sectorSlope(objActor, pSpr->sector(), 0, flag2);
                        if (getceilzofslopeptr(pSpr->sector(), pSpr->pos.X, pSpr->y) + kSlopeDist >= pSpr->z) sprite2sectorSlope(objActor, pSpr->sector(), 1, flag2);
                        break;
                }
                break;
            default:
                spriteSetSlope(pSpr, slope);
                break;
    }
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useDataChanger(DBloodActor* sourceactor, int objType, sectortype* pSector, walltype* pWall, DBloodActor* objActor)
{
    auto pXSource = &sourceactor->x();
    spritetype* pSource = &sourceactor->s();

    switch (objType) 
    {
        case OBJ_SECTOR:
            if ((pSource->flags & kModernTypeFlag1) || (pXSource->data1 != -1 && pXSource->data1 != 32767))
                setDataValueOfObject(objType, pSector, pWall, nullptr, 1, pXSource->data1);
            break;
        case OBJ_SPRITE:
            if ((pSource->flags & kModernTypeFlag1) || (pXSource->data1 != -1 && pXSource->data1 != 32767))
                setDataValueOfObject(objType, pSector, pWall, objActor, 1, pXSource->data1);

            if ((pSource->flags & kModernTypeFlag1) || (pXSource->data2 != -1 && pXSource->data2 != 32767))
                setDataValueOfObject(objType, pSector, pWall, objActor, 2, pXSource->data2);

            if ((pSource->flags & kModernTypeFlag1) || (pXSource->data3 != -1 && pXSource->data3 != 32767))
                setDataValueOfObject(objType, pSector, pWall, objActor, 3, pXSource->data3);

            if ((pSource->flags & kModernTypeFlag1) || pXSource->data4 != 65535)
                setDataValueOfObject(objType, pSector, pWall, objActor, 4, pXSource->data4);
            break;
        case OBJ_WALL:
            if ((pSource->flags & kModernTypeFlag1) || (pXSource->data1 != -1 && pXSource->data1 != 32767))
                setDataValueOfObject(objType, pSector, pWall, nullptr, 1, pXSource->data1);
            break;
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useSectorLightChanger(DBloodActor* sourceactor, sectortype* pSector) 
{
    auto pXSource = &sourceactor->x();
    spritetype* pSource = &sourceactor->s();
    auto pXSector = &pSector->xs();

    if (valueIsBetween(pXSource->data1, -1, 32767))
        pXSector->wave = ClipHigh(pXSource->data1, 11);

    int oldAmplitude = pXSector->amplitude;
    if (valueIsBetween(pXSource->data2, -128, 128))
        pXSector->amplitude = uint8_t(pXSource->data2);

    if (valueIsBetween(pXSource->data3, -1, 32767))
        pXSector->freq = ClipHigh(pXSource->data3, 255);

    if (valueIsBetween(pXSource->data4, -1, 65535))
        pXSector->phase = ClipHigh(pXSource->data4, 255);

    if (pSource->flags) 
    {
        if (pSource->flags != kModernTypeFlag1) 
        {
            pXSector->shadeAlways   = (pSource->flags & 0x0001) ? true : false;
            pXSector->shadeFloor    = (pSource->flags & 0x0002) ? true : false;
            pXSector->shadeCeiling  = (pSource->flags & 0x0004) ? true : false;
            pXSector->shadeWalls    = (pSource->flags & 0x0008) ? true : false;
        }
        else 
        {
            pXSector->shadeAlways   = true;
        }
    }

    // add to shadeList if amplitude was set to 0 previously
    if (oldAmplitude != pXSector->amplitude) 
    {
        if (!shadeList.Contains(pSector))
            shadeList.Push(pSector);
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void useTargetChanger(DBloodActor* sourceactor, DBloodActor* actor)
{
    spritetype* pSprite = &actor->s();
    
    if (!actor->IsDudeActor() || pSprite->statnum != kStatDude) 
    {
        switch (pSprite->type) // can be dead dude turned in gib
        {
            // make current target and all other dudes not attack this dude anymore
        case kThingBloodBits:
        case kThingBloodChunks:
            aiFightFreeTargets(actor);
            return;
        default:
            return;
        }
    }
    
    
    auto pXSource = &sourceactor->x();
    XSPRITE* pXSprite = &actor->x();

    int receiveHp = 33 + Random(33);
    DUDEINFO* pDudeInfo = getDudeInfo(pSprite->type); 
    int matesPerEnemy = 1;

    // dude is burning?
    if (pXSprite->burnTime > 0 && actor->GetBurnSource())
    {
        if (IsBurningDude(actor)) return;
        else 
        {
            auto burnactor = actor->GetBurnSource();
            if (burnactor->hasX()) 
            {
                if (pXSource->data2 == 1 && pXSprite->rxID == burnactor->x().rxID) 
                {
                    pXSprite->burnTime = 0;
                    
                    // heal dude a bit in case of friendly fire
                    int startHp = (pXSprite->sysData2 > 0) ? ClipRange(pXSprite->sysData2 << 4, 1, 65535) : pDudeInfo->startHealth << 4;
                    if (pXSprite->health < (unsigned)startHp) actHealDude(actor, receiveHp, startHp);
                } 
                else if (burnactor->x().health <= 0) 
                {
                    pXSprite->burnTime = 0;
                }
            }
        }
    }

    auto playeractor = aiFightTargetIsPlayer(actor);
    // special handling for player(s) if target changer data4 > 2.
    if (playeractor != nullptr)
    {
        auto pPlayer = &playeractor->s();
        auto actLeech = leechIsDropped(actor);
        if (pXSource->data4 == 3) 
        {
            aiSetTarget(actor, pSprite->pos.X, pSprite->y, pSprite->z);
            aiSetGenIdleState(actor);
            if (pSprite->type == kDudeModernCustom && actLeech)
                removeLeech(actLeech);
        }
        else if (pXSource->data4 == 4) 
        {
            aiSetTarget(actor, pPlayer->pos.X, pPlayer->y, pPlayer->z);
            if (pSprite->type == kDudeModernCustom && actLeech)
                removeLeech(actLeech);
        }
    }

    int maxAlarmDudes = 8 + Random(8);
    auto targetactor = actor->GetTarget();
    if (targetactor && targetactor->hasX() && playeractor == nullptr) 
    {
        auto pTarget = &targetactor->s(); 
        auto pXTarget = &targetactor->x();

        if (aiFightUnitCanFly(actor) && aiFightIsMeleeUnit(targetactor) && !aiFightUnitCanFly(targetactor))
            pSprite->flags |= 0x0002;
        else if (aiFightUnitCanFly(actor))
            pSprite->flags &= ~0x0002;

        if (!targetactor->IsDudeActor() || pXTarget->health < 1 || !aiFightDudeCanSeeTarget(actor, pDudeInfo, targetactor))
        {
            aiSetTarget(actor, pSprite->pos.X, pSprite->y, pSprite->z);
        }
        // dude attack or attacked by target that does not fit by data id?
        else if (pXSource->data1 != 666 && pXTarget->data1 != pXSource->data1) 
        {
            if (aiFightDudeIsAffected(targetactor)) 
            {
                // force stop attack target
                aiSetTarget(actor, pSprite->pos.X, pSprite->y, pSprite->z);
                if (actor->GetBurnSource() == targetactor) 
                {
                    pXSprite->burnTime = 0;
                    actor->SetBurnSource(nullptr);
                }

                // force stop attack dude
                aiSetTarget(targetactor, pTarget->pos.X, pTarget->y, pTarget->z);
                if (targetactor->GetBurnSource() == actor) 
                {
                    pXTarget->burnTime = 0;
                    targetactor->SetBurnSource(nullptr);
                }
            }
        }
        else if (pXSource->data2 == 1 && pXSprite->rxID == pXTarget->rxID) 
        {
            auto mateactor = targetactor;
            spritetype* pMate = pTarget; 
            XSPRITE* pXMate = pXTarget;

            // heal dude
            int startHp = (pXSprite->sysData2 > 0) ? ClipRange(pXSprite->sysData2 << 4, 1, 65535) : pDudeInfo->startHealth << 4;
            if (pXSprite->health < (unsigned)startHp) actHealDude(actor, receiveHp, startHp);

            // heal mate
            startHp = (pXMate->sysData2 > 0) ? ClipRange(pXMate->sysData2 << 4, 1, 65535) : getDudeInfo(pMate->type)->startHealth << 4;
            if (pXMate->health < (unsigned)startHp) actHealDude(mateactor, receiveHp, startHp);

            auto matetarget = mateactor->GetTarget();
            if (matetarget != nullptr && matetarget->hasX())
            {
                // force mate stop attack dude, if he does
                if (matetarget == actor)
                {
                    aiSetTarget(mateactor, pMate->pos.X, pMate->y, pMate->z);
                }
                else if (pXSprite->rxID != matetarget->x().rxID) 
                {
                    // force dude to attack same target that mate have
                    aiSetTarget(actor, matetarget);
                    return;
                }
                else 
                {
                    // force mate to stop attack another mate
                    aiSetTarget(mateactor, pMate->pos.X, pMate->y, pMate->z);
                }
            }

            // force dude stop attack mate, if target was not changed previously
            if (actor == mateactor)
                aiSetTarget(actor, pSprite->pos.X, pSprite->y, pSprite->z);


        }
        // check if targets aims player then force this target to fight with dude
        else if (aiFightTargetIsPlayer(actor) != nullptr)
        {
            aiSetTarget(targetactor, actor);
        }

        int mDist = 3; 
        if (aiFightIsMeleeUnit(actor)) mDist = 2;

        if (targetactor != nullptr && aiFightGetTargetDist(actor, pDudeInfo, targetactor) < mDist) 
        {
            if (!isActive(actor)) aiActivateDude(actor);
            return;
        }
        // lets try to look for target that fits better by distance
        else if ((PlayClock & 256) != 0 && (targetactor == nullptr || aiFightGetTargetDist(actor, pDudeInfo, targetactor) >= mDist)) 
        {
            auto newtargactor = aiFightGetTargetInRange(actor, 0, mDist, pXSource->data1, pXSource->data2);
            if (newtargactor != nullptr)
            {
                // Make prev target not aim in dude
                if (targetactor)
                {
                    aiSetTarget(targetactor, targetactor->spr.pos.X, targetactor->spr.y, targetactor->spr.z);
                    if (!isActive(newtargactor))
                        aiActivateDude(newtargactor);
                }

                // Change target for dude
                aiSetTarget(actor, newtargactor);
                if (!isActive(actor))
                    aiActivateDude(actor);

                // ...and change target of target to dude to force it fight
                if (pXSource->data3 > 0 && newtargactor->GetTarget() != actor) 
                {
                    aiSetTarget(newtargactor, actor);
                    if (!isActive(newtargactor))
                        aiActivateDude(newtargactor);
                }

                return;
            }
        }
    }
    
    if ((targetactor == nullptr || playeractor != nullptr) && (PlayClock & 32) != 0) 
    {
        // try find first target that dude can see
        BloodStatIterator it(kStatDude);
        while (auto newtargactor = it.Next())
        {
            auto pXNewTarg = &newtargactor->x();

            if (newtargactor->GetTarget() == actor)
            {
                aiSetTarget(actor, newtargactor);
                return;
            }

            // skip non-dudes and players
            if (!newtargactor->IsDudeActor() || (newtargactor->IsPlayerActor() && pXSource->data4 > 0) || newtargactor->GetOwner() == actor) continue;
            // avoid self aiming, those who dude can't see, and those who dude own
            else if (!aiFightDudeCanSeeTarget(actor, pDudeInfo, newtargactor) || actor == newtargactor) continue;
            // if Target Changer have data1 = 666, everyone can be target, except AI team mates.
            else if (pXSource->data1 != 666 && pXSource->data1 != pXNewTarg->data1) continue;
            // don't attack immortal, burning dudes and mates
            if (IsBurningDude(newtargactor) || !IsKillableDude(newtargactor) || (pXSource->data2 == 1 && pXSprite->rxID == pXNewTarg->rxID))
                continue;

            if (pXSource->data2 == 0 || (pXSource->data2 == 1 && !aiFightMatesHaveSameTarget(actor, newtargactor, matesPerEnemy))) 
            {
                // Change target for dude
                aiSetTarget(actor, newtargactor);
                if (!isActive(actor))
                    aiActivateDude(actor);

                // ...and change target of target to dude to force it fight
                if (pXSource->data3 > 0 && newtargactor->GetTarget() != actor) 
                {
                    aiSetTarget(newtargactor, actor);
                    if (playeractor == nullptr && !isActive(newtargactor))
                        aiActivateDude(newtargactor);

                    if (pXSource->data3 == 2)
                        aiFightAlarmDudesInSight(newtargactor, maxAlarmDudes);
                }
                return;
            }
            break;
        }
    }

    // got no target - let's ask mates if they have targets
    if ((actor->GetTarget() == nullptr || playeractor != nullptr) && pXSource->data2 == 1 && (PlayClock & 64) != 0) 
    {
        DBloodActor* pMateTargetActor = aiFightGetMateTargets(actor);
        if (pMateTargetActor != nullptr && pMateTargetActor->hasX()) 
        {

            if (aiFightDudeCanSeeTarget(actor, pDudeInfo, pMateTargetActor)) 
            {
                if (pMateTargetActor->GetTarget() == nullptr) 
                {
                    aiSetTarget(pMateTargetActor, actor);
                    if (pMateTargetActor->IsDudeActor() && !isActive(pMateTargetActor))
                        aiActivateDude(pMateTargetActor);
                }

                aiSetTarget(actor, pMateTargetActor);
                if (!isActive(actor))
                    aiActivateDude(actor);
                return;

                // try walk in mate direction in case if not see the target
            } 
            else if (pMateTargetActor->GetTarget() && aiFightDudeCanSeeTarget(actor, pDudeInfo, pMateTargetActor->GetTarget()))
            {
                actor->SetTarget(pMateTargetActor);
                spritetype* pMate = &pMateTargetActor->GetTarget()->s();
                pXSprite->targetX = pMate->pos.X;
                pXSprite->targetY = pMate->y;
                pXSprite->targetZ = pMate->z;
                if (!isActive(actor))
                    aiActivateDude(actor);
                return;
            }
        }
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void usePictureChanger(DBloodActor* sourceactor, int objType, sectortype* targSect, walltype* targWall, DBloodActor* objActor)
{
    auto pXSource = &sourceactor->x();
    
    switch (objType) {
        case OBJ_SECTOR:
            if (valueIsBetween(pXSource->data1, -1, 32767))
                targSect->floorpicnum = pXSource->data1;

            if (valueIsBetween(pXSource->data2, -1, 32767))
                targSect->ceilingpicnum = pXSource->data2;

            if (valueIsBetween(pXSource->data3, -1, 32767))
                targSect->floorpal = uint8_t(pXSource->data3);

            if (valueIsBetween(pXSource->data4, -1, 65535))
                targSect->ceilingpal = uint8_t(pXSource->data4);
            break;
        case OBJ_SPRITE:
            if (valueIsBetween(pXSource->data1, -1, 32767))
                objActor->spr.picnum = pXSource->data1;

            if (pXSource->data2 >= 0) objActor->spr.shade = (pXSource->data2 > 127) ? 127 : pXSource->data2;
            else if (pXSource->data2 < -1) objActor->spr.shade = (pXSource->data2 < -127) ? -127 : pXSource->data2;

            if (valueIsBetween(pXSource->data3, -1, 32767))
                objActor->spr.pal = uint8_t(pXSource->data3);
            break;
        case OBJ_WALL:
            if (valueIsBetween(pXSource->data1, -1, 32767))
                targWall->picnum = pXSource->data1;

            if (valueIsBetween(pXSource->data2, -1, 32767))
                targWall->overpicnum = pXSource->data2;

            if (valueIsBetween(pXSource->data3, -1, 32767))
                targWall->pal = uint8_t(pXSource->data3);
            break;
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

QAV* playerQavSceneLoad(int qavId) 
{
    QAV* pQav = getQAV(qavId);
    if (!pQav) viewSetSystemMessage("Failed to load QAV animation #%d", qavId);
    return pQav;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void playerQavSceneProcess(PLAYER* pPlayer, QAVSCENE* pQavScene)
{
    auto initiator = pQavScene->initiator;
    if (initiator->hasX())
    {
        XSPRITE* pXSprite = &initiator->x();
        if (pXSprite->waitTime > 0 && --pXSprite->sysData1 <= 0) 
        {
            if (pXSprite->txID >= kChannelUser) 
            {
                for (int i = bucketHead[pXSprite->txID]; i < bucketHead[pXSprite->txID + 1]; i++) 
                {
                    if (rxBucket[i].isActor()) 
                    {
                        auto rxactor = rxBucket[i].actor();
                        if (!rxactor || !rxactor->hasX() || rxactor == initiator) continue;

                        spritetype* pSpr = &rxactor->s();
                        auto pXSpr = &rxactor->x();
                        if (pSpr->type == kModernPlayerControl && pXSpr->command == 67) 
                        {
                            if (pXSpr->data2 == pXSprite->data2 || pXSpr->locked) continue;
                            else trPlayerCtrlStartScene(rxactor, pPlayer, true);
                            return;
                        }

                    }
                    nnExtTriggerObject(rxBucket[i], pXSprite->command);

                }
            }
            trPlayerCtrlStopScene(pPlayer);
        }
        else 
        {
            playerQavScenePlay(pPlayer);
            pPlayer->weaponTimer = ClipLow(pPlayer->weaponTimer -= 4, 0);
        }
    }
    else 
    {
        
        pQavScene->initiator = nullptr;
        pPlayer->sceneQav = -1;
        pQavScene->qavResrc = NULL;
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void playerQavSceneDraw(PLAYER* pPlayer, int a2, double a3, double a4, int a5)
{
    if (pPlayer == NULL || pPlayer->sceneQav == -1) return;

    QAVSCENE* pQavScene = &gPlayerCtrl[pPlayer->nPlayer].qavScene;
    spritetype* pSprite = &pQavScene->initiator->s();

    if (pQavScene->qavResrc != NULL) 
    {
        QAV* pQAV = pQavScene->qavResrc;
        int v4;
        double smoothratio;

        qavProcessTimer(pPlayer, pQAV, &v4, &smoothratio);

        int flags = 2; int nInv = powerupCheck(pPlayer, kPwUpShadowCloak);
        if (nInv >= 120 * 8 || (nInv != 0 && (PlayClock & 32))) 
        {
            a2 = -128; flags |= 1;
        }

        // draw as weapon
        if (!(pSprite->flags & kModernTypeFlag1)) 
        {
            pQAV->x = int(a3); pQAV->y = int(a4);
            pQAV->Draw(a3, a4, v4, flags, a2, a5, true, smoothratio);

            // draw fullscreen (currently 4:3 only)
        }
        else 
        {
            // What an awful hack. This throws proper ordering out of the window, but there is no way to reproduce this better with strict layering of elements.
			// From the above commit it seems to be incomplete anyway...
            pQAV->Draw(v4, flags, a2, a5, false, smoothratio);
        }
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void playerQavScenePlay(PLAYER* pPlayer)
{
    if (pPlayer == NULL) return;
    
    QAVSCENE* pQavScene = &gPlayerCtrl[pPlayer->nPlayer].qavScene;
    if (pPlayer->sceneQav == -1 && pQavScene->initiator != nullptr)
        pPlayer->sceneQav = pQavScene->initiator->x().data2;

    if (pQavScene->qavResrc != NULL) 
    {
        QAV* pQAV = pQavScene->qavResrc;
        int nTicks = pQAV->duration - pPlayer->weaponTimer;
        pQAV->Play(nTicks - 4, nTicks, pPlayer->qavCallback, pPlayer);
    }
}

void playerQavSceneReset(PLAYER* pPlayer) 
{
    QAVSCENE* pQavScene = &gPlayerCtrl[pPlayer->nPlayer].qavScene;
    pQavScene->initiator = nullptr;
    pQavScene->dummy = pPlayer->sceneQav = -1;
    pQavScene->qavResrc = NULL;
}

bool playerSizeShrink(PLAYER* pPlayer, int divider) 
{
    pPlayer->pXSprite->scale = 256 / divider;
    playerSetRace(pPlayer, kModeHumanShrink);
    return true;
}

bool playerSizeGrow(PLAYER* pPlayer, int multiplier) 
{
    pPlayer->pXSprite->scale = 256 * multiplier;
    playerSetRace(pPlayer, kModeHumanGrown);
    return true;
}

bool playerSizeReset(PLAYER* pPlayer) 
{
    playerSetRace(pPlayer, kModeHuman);
    pPlayer->pXSprite->scale = 0;
    return true;
}

void playerDeactivateShrooms(PLAYER* pPlayer) 
{
    powerupDeactivate(pPlayer, kPwUpGrowShroom);
    pPlayer->pwUpTime[kPwUpGrowShroom] = 0;

    powerupDeactivate(pPlayer, kPwUpShrinkShroom);
    pPlayer->pwUpTime[kPwUpShrinkShroom] = 0;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

PLAYER* getPlayerById(int id) 
{
    // relative to connected players
    if (id >= 1 && id <= kMaxPlayers) 
    {
        id = id - 1;
        for (int i = connecthead; i >= 0; i = connectpoint2[i]) 
        {
            if (id == gPlayer[i].nPlayer)
                return &gPlayer[i];
        }

    // absolute sprite type
    } 
    else if (id >= kDudePlayer1 && id <= kDudePlayer8) 
    {
        for (int i = connecthead; i >= 0; i = connectpoint2[i]) 
        {
            if (id == gPlayer[i].pSprite->type)
                return &gPlayer[i];
        }
    }

    //viewSetSystemMessage("There is no player id #%d", id);
    return NULL;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool IsBurningDude(DBloodActor* actor) 
{
    if (actor == NULL) return false;
    switch (actor->spr.type) 
    {
    case kDudeBurningInnocent:
    case kDudeBurningCultist:
    case kDudeBurningZombieAxe:
    case kDudeBurningZombieButcher:
    case kDudeBurningTinyCaleb:
    case kDudeBurningBeast:
    case kDudeModernCustomBurning:
        return true;
    }

    return false;
}

bool IsKillableDude(DBloodActor* actor)
{
    switch (actor->spr.type) 
    {
    case kDudeGargoyleStatueFlesh:
    case kDudeGargoyleStatueStone:
        return false;
    default:
        if (!actor->IsDudeActor() || actor->x().locked == 1) return false;
        return true;
    }
}

bool isGrown(DBloodActor* actor)
{
    if (powerupCheck(&gPlayer[actor->spr.type - kDudePlayer1], kPwUpGrowShroom) > 0) return true;
    else if (actor->hasX() && actor->x().scale >= 512) return true;
    else return false;
}

bool isShrinked(DBloodActor* actor)
{
    if (powerupCheck(&gPlayer[actor->spr.type - kDudePlayer1], kPwUpShrinkShroom) > 0) return true;
    else if (actor->hasX() && actor->x().scale > 0 && actor->x().scale <= 128) return true;
    else return false;
}

bool isActive(DBloodActor* actor) 
{
    if (!actor->hasX())
        return false;

    switch (actor->x().aiState->stateType) 
    {
    case kAiStateIdle:
    case kAiStateGenIdle:
    case kAiStateSearch:
    case kAiStateMove:
    case kAiStateOther:
        return false;
    default:
        return true;
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

int getDataFieldOfObject(EventObject &eob, int dataIndex)
{
    int data = -65535;

    if (eob.isActor())
    {
        auto actor = eob.actor();
        if (actor)
        {
            switch (dataIndex) 
            {
                case 1: return actor->x().data1;
                case 2: return actor->x().data2;
                case 3:
                    switch (actor->spr.type) 
                    {
                        case kDudeModernCustom: return actor->x().sysData1;
                        default: return actor->x().data3;
                    }
                case 4: return actor->x().data4;
                default: return data;
            }
        }
    }
    else if (eob.isSector())
    {
        return eob.sector()->xs().data;
    }
    else if (eob.isWall())
    {
        return eob.wall()->xw().data;
    }
    return data;
}

int getDataFieldOfObject(int objType, sectortype* sect, walltype* wal, DBloodActor* actor, int dataIndex)
{
    int data = -65535;
    switch (objType)
    {
    case OBJ_SPRITE:
        switch (dataIndex)
        {
        case 1: return actor->x().data1;
        case 2: return actor->x().data2;
        case 3:
            switch (actor->spr.type)
            {
            case kDudeModernCustom: return actor->x().sysData1;
            default: return actor->x().data3;
            }
        case 4: return actor->x().data4;
        default: return data;
        }
    case OBJ_SECTOR: return sect->xs().data;
    case OBJ_WALL: return wal->xw().data;
        default: return data;
    }
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

bool setDataValueOfObject(int objType, sectortype* sect, walltype* wal, DBloodActor* objActor, int dataIndex, int value)
{
    switch (objType) 
    {
        case OBJ_SPRITE: 
        {
            XSPRITE* pXSprite = &objActor->x();
            int type = objActor->spr.type;

            // exceptions
            if (objActor->IsDudeActor() && pXSprite->health <= 0) return true;
            switch (type)
            {
                case kThingBloodBits:
                case kThingBloodChunks:
                case kThingZombieHead:
                    return true;
                    break;
            }

            switch (dataIndex) 
            {
                case 1:
                    pXSprite->data1 = value;
                    switch (type) 
                    {
                        case kSwitchCombo:
                            if (value == pXSprite->data2) SetSpriteState(objActor, 1);
                            else SetSpriteState(objActor, 0);
                            break;
                        case kDudeModernCustom:
                        case kDudeModernCustomBurning:
                            objActor->genDudeExtra.updReq[kGenDudePropertyWeapon] = true;
                            objActor->genDudeExtra.updReq[kGenDudePropertyDmgScale] = true;
                            evPostActor(objActor, kGenDudeUpdTimeRate, kCallbackGenDudeUpdate);
                            break;
                    }
                    return true;
                case 2:
                    pXSprite->data2 = value;
                    switch (type) 
                    {
                        case kDudeModernCustom:
                        case kDudeModernCustomBurning:
                            objActor->genDudeExtra.updReq[kGenDudePropertySpriteSize] = true;
                            objActor->genDudeExtra.updReq[kGenDudePropertyMass] = true;
                            objActor->genDudeExtra.updReq[kGenDudePropertyDmgScale] = true;
                            objActor->genDudeExtra.updReq[kGenDudePropertyStates] = true;
                            objActor->genDudeExtra.updReq[kGenDudePropertyAttack] = true;
                            evPostActor(objActor, kGenDudeUpdTimeRate, kCallbackGenDudeUpdate);
                            break;
                    }
                    return true;
                case 3:
                    pXSprite->data3 = value;
                    switch (type)
                    {
                        case kDudeModernCustom:
                        case kDudeModernCustomBurning:
                            pXSprite->sysData1 = value;
                            break;
                    }
                    return true;
                case 4:
                    pXSprite->data4 = value;
                    return true;
                default:
                    return false;
            }
        }
        case OBJ_SECTOR:
            sect->xs().data = value;
            return true;
        case OBJ_WALL:
            wal->xw().data = value;
            return true;
        default:
            return false;
    }
}

//---------------------------------------------------------------------------
//
// a replacement of vanilla CanMove for patrol dudes
//
//---------------------------------------------------------------------------

bool nnExtCanMove(DBloodActor* actor, DBloodActor* target, int nAngle, int nRange) 
{
    auto pSprite = &actor->s();
    int x = pSprite->pos.X, y = pSprite->y, z = pSprite->z;
    auto pSector = pSprite->sector();
    HitScan(actor, z, Cos(nAngle) >> 16, Sin(nAngle) >> 16, 0, CLIPMASK0, nRange);
    int nDist = approxDist(x - gHitInfo.hitpos.X, y - gHitInfo.hitpos.Y);
    if (target != nullptr && nDist - (pSprite->clipdist << 2) < nRange)
        return (target == gHitInfo.actor());

    x += MulScale(nRange, Cos(nAngle), 30);
    y += MulScale(nRange, Sin(nAngle), 30);
    if (!FindSector(x, y, z, &pSector))
        return false;

    if (pSector->hasX()) {

        XSECTOR* pXSector = &pSector->xs();
        return !((pSector->type == kSectorDamage || pXSector->damageType > 0) && pXSector->state && !nnExtIsImmune(actor, pXSector->damageType, 16));

    }

    return true;

}

//---------------------------------------------------------------------------
//
// a replacement of vanilla aiChooseDirection for patrol dudes
//
//---------------------------------------------------------------------------

void nnExtAiSetDirection(DBloodActor* actor, int a3) 
{
    spritetype* pSprite = &actor->s();
    XSPRITE* pXSprite = &actor->x();
    assert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    
    int vc = ((a3 + 1024 - pSprite->ang) & 2047) - 1024;
    int t1 = DMulScale(actor->xvel, Cos(pSprite->ang), actor->yvel, Sin(pSprite->ang), 30);
    int vsi = ((t1 * 15) >> 12) / 2;
    int v8 = 341;
    
    if (vc < 0)
        v8 = -341;

    if (nnExtCanMove(actor, actor->GetTarget(), pSprite->ang + vc, vsi))
        pXSprite->goalAng = pSprite->ang + vc;
    else if (nnExtCanMove(actor, actor->GetTarget(), pSprite->ang + vc / 2, vsi))
        pXSprite->goalAng = pSprite->ang + vc / 2;
    else if (nnExtCanMove(actor, actor->GetTarget(), pSprite->ang - vc / 2, vsi))
        pXSprite->goalAng = pSprite->ang - vc / 2;
    else if (nnExtCanMove(actor, actor->GetTarget(), pSprite->ang + v8, vsi))
        pXSprite->goalAng = pSprite->ang + v8;
    else if (nnExtCanMove(actor, actor->GetTarget(), pSprite->ang, vsi))
        pXSprite->goalAng = pSprite->ang;
    else if (nnExtCanMove(actor, actor->GetTarget(), pSprite->ang - v8, vsi))
        pXSprite->goalAng = pSprite->ang - v8;
    else
        pXSprite->goalAng = pSprite->ang + 341;

    if (pXSprite->dodgeDir) 
        {
        if (!nnExtCanMove(actor, actor->GetTarget(), pSprite->ang + pXSprite->dodgeDir * 512, 512))
        {
            pXSprite->dodgeDir = -pXSprite->dodgeDir;
            if (!nnExtCanMove(actor, actor->GetTarget(), pSprite->ang + pXSprite->dodgeDir * 512, 512))
                pXSprite->dodgeDir = 0;
        }
    }
}


//---------------------------------------------------------------------------
//
/// patrol functions
//
//---------------------------------------------------------------------------

void aiPatrolState(DBloodActor* actor, int state) 
{
    spritetype* pSprite = &actor->s();
    assert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax && actor->hasX());
    assert(actor->GetTarget());
    
    XSPRITE* pXSprite = &actor->x();
    
    auto markeractor = actor->GetTarget();
    spritetype* pMarker = &markeractor->s();
    XSPRITE* pXMarker = &markeractor->x();
    assert(pMarker->type == kMarkerPath);

    bool nSeqOverride = false, crouch = false;
    int i, seq = -1, start = 0, end = kPatrolStateSize;
    
    const DUDEINFO_EXTRA* pExtra = &gDudeInfoExtra[pSprite->type - kDudeBase];
    
    switch (state) {
        case kAiStatePatrolWaitL:
            seq = pExtra->idlgseqofs;
            start = 0; end = 2;
            break;
        case kAiStatePatrolMoveL:
            seq = pExtra->mvegseqofs;
            start = 2, end = 7;
            break;
        case kAiStatePatrolTurnL:
            seq = pExtra->mvegseqofs;
            start = 7, end = 12;
            break;
        case kAiStatePatrolWaitW:
            seq = pExtra->idlwseqofs;
            start = 12; end = 18;
            break;
        case kAiStatePatrolMoveW:
            seq = pExtra->mvewseqofs;
            start = 18; end = 25;
            break;
        case kAiStatePatrolTurnW:
            seq = pExtra->mvewseqofs;
            start = 25; end = 32;
            break;
        case kAiStatePatrolWaitC:
            seq = pExtra->idlcseqofs;
            start = 32; end = 36;
            crouch = true;
            break;
        case kAiStatePatrolMoveC:
            seq = pExtra->mvecseqofs;
            start = 36; end = 39;
            crouch = true;
            break;
        case kAiStatePatrolTurnC:
            seq = pExtra->mvecseqofs;
            start = 39; end = kPatrolStateSize;
            crouch = true;
            break;
    }

    
    if (pXMarker->data4 > 0) seq = pXMarker->data4, nSeqOverride = true;
    else if (!nSeqOverride && state == kAiStatePatrolWaitC && (pSprite->type == kDudeCultistTesla || pSprite->type == kDudeCultistTNT))
        seq = 11537, nSeqOverride = true;  // these don't have idle crouch seq for some reason...

    if (seq < 0)
        return aiPatrolStop(actor, nullptr);

    for (i = start; i < end; i++) 
    {
        AISTATE* newState = &genPatrolStates[i];
        if (newState->stateType != state || (!nSeqOverride && seq != newState->seqId))
            continue;

        if (pSprite->type == kDudeModernCustom) aiGenDudeNewState(actor, newState);
        else aiNewState(actor, newState);

        if (crouch) pXSprite->unused1 |= kDudeFlagCrouch;
        else pXSprite->unused1 &= ~kDudeFlagCrouch;

        if (nSeqOverride)
            seqSpawn(seq, actor);

        return;
    }

    if (i == end) 
    {
        viewSetSystemMessage("No patrol state #%d found for dude #%d (type = %d)", state, actor->GetIndex(), pSprite->type);
        aiPatrolStop(actor, nullptr);
    }
}

//---------------------------------------------------------------------------
//
// check if some dude already follows the given marker
//
//---------------------------------------------------------------------------

DBloodActor* aiPatrolMarkerBusy(DBloodActor* except, DBloodActor* marker) 
{
    BloodStatIterator it(kStatDude);
    while (auto actor = it.Next())
    {
        if (!actor->IsDudeActor() || actor == except || !actor->hasX())
            continue;

        auto targ = actor->GetTarget();
        if (actor->x().health > 0 && targ != nullptr && targ->spr.type == kMarkerPath && targ == marker)
            return actor;
    }
    return nullptr;
}

//---------------------------------------------------------------------------
//
// check if some dude already follows the given marker
//
//---------------------------------------------------------------------------

bool aiPatrolMarkerReached(DBloodActor* actor) 
{
    spritetype* pSprite = &actor->s();
    assert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);

    const DUDEINFO_EXTRA* pExtra = &gDudeInfoExtra[pSprite->type - kDudeBase];
    auto markeractor = actor->GetTarget();
    if (markeractor && markeractor->spr.type == kMarkerPath) 
    {
        spritetype* pMarker = &markeractor->s();
        int okDist = ClipLow(pMarker->clipdist << 1, 4);
        int oX = abs(pMarker->pos.X - pSprite->pos.X) >> 4;
        int oY = abs(pMarker->y - pSprite->y) >> 4;

        if (approxDist(oX, oY) <= okDist) 
        {
            if (spriteIsUnderwater(actor) || pExtra->flying) 
            {
                okDist = pMarker->clipdist << 4;
                int ztop, zbot, ztop2, zbot2;
                GetActorExtents(actor, &ztop, &zbot);
                GetActorExtents(markeractor, &ztop2, &zbot2);

                int oZ1 = abs(zbot - ztop2) >> 6;
                int oZ2 = abs(ztop - zbot2) >> 6;
                if (oZ1 > okDist && oZ2 > okDist)
                    return false;
            }
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

DBloodActor* findNextMarker(DBloodActor* mark, bool back) 
{
    auto pXMark = &mark->x();

    BloodStatIterator it(kStatPathMarker);
    while (auto next = it.Next())
    {
        if (!next->hasX() || next == mark) continue;

        XSPRITE* pXNext = &next->x();
        if ((pXNext->locked || pXNext->isTriggered || pXNext->DudeLockout) || (back && pXNext->data2 != pXMark->data1) || (!back && pXNext->data1 != pXMark->data2))
            continue;

        return next;
    }
    return nullptr;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool markerIsNode(DBloodActor* mark, bool back) 
{
    auto pXMark = &mark->x();
    int cnt = 0;

    BloodStatIterator it(kStatPathMarker);
    while (auto next = it.Next())
    {
        if (!next->hasX() || next == mark) continue;

        XSPRITE* pXNext = &next->x();

        if ((pXNext->locked || pXNext->isTriggered || pXNext->DudeLockout) || (back && pXNext->data2 != pXMark->data1) || (!back && pXNext->data1 != pXMark->data2))
            continue;

        if (++cnt > 1)
            return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolSetMarker(DBloodActor* actor)
{
    auto pSprite = &actor->s();
    auto pXSprite = &actor->x();
    auto targetactor = actor->GetTarget();

    DBloodActor* selected = nullptr;
    int closest = 200000;

    // select closest marker that dude can see
    if (targetactor == nullptr)
    {
        int zt1, zb1, zt2, zb2, dist;
        GetActorExtents(actor, &zt2, &zb2);

        BloodStatIterator it(kStatPathMarker);
        while (auto nextactor = it.Next())
        {
            if (!nextactor->hasX()) continue;

            auto pNext = &nextactor->s();
            auto pXNext = &nextactor->x();

            if (pXNext->locked || pXNext->isTriggered || pXNext->DudeLockout || (dist = approxDist(pNext->pos.X - pSprite->pos.X, pNext->y - pSprite->y)) > closest)
                continue;

            GetActorExtents(nextactor, &zt1, &zb1); 
            if (cansee(pNext->pos.X, pNext->y, zt1, pNext->sector(), pSprite->pos.X, pSprite->y, zt2, pSprite->sector()))
            {
                closest = dist;
                selected = nextactor;
            }
        }
    } 
    // set next marker
    else if (targetactor->spr.type == kMarkerPath && targetactor->hasX())
    {
        // idea: which one of next (allowed) markers are closer to the potential target?
        // idea: -3 select random next marker that dude can see in radius of reached marker
        // if reached marker is in radius of another marker with -3, but greater radius, use that marker
        // idea: for nodes only flag32 = specify if enemy must return back to node or allowed to select
        // another marker which belongs that node?
        spritetype* pPrev = NULL;   XSPRITE* pXPrev = NULL;
        DBloodActor* prevactor = nullptr;

        DBloodActor* firstFinePath = nullptr;
        int next;

        int breakChance = 0;
        auto pXCur = &targetactor->x();
        if (actor->prevmarker)
        {
            prevactor = actor->prevmarker;
            pPrev = &prevactor->s();
            pXPrev = &prevactor->x();
        }

        bool node = markerIsNode(targetactor, false);
        pXSprite->unused2 = aiPatrolGetPathDir(actor, targetactor); // decide if it should go back or forward
        if (pXSprite->unused2 == kPatrolMoveBackward && Chance(0x8000) && node)
            pXSprite->unused2 = kPatrolMoveForward;

        bool back = (pXSprite->unused2 == kPatrolMoveBackward); next = (back) ? pXCur->data1 : pXCur->data2;
        BloodStatIterator it(kStatPathMarker);
        while(auto nextactor = it.Next())
        {
            if (nextactor == targetactor || !nextactor->hasX()) continue;
            else if (pXSprite->targetX >= 0 && nextactor == prevactor && node) 
            {
                if (pXCur->data2 == pXPrev->data1)
                    continue;
            }

            auto pXNext = &nextactor->x();
            if ((pXNext->locked || pXNext->isTriggered || pXNext->DudeLockout) || (back && pXNext->data2 != next) || (!back && pXNext->data1 != next))
                continue;
            
            if (firstFinePath == nullptr) firstFinePath = nextactor;
            if (aiPatrolMarkerBusy(actor, nextactor) && !Chance(0x0010)) continue;
            else selected = nextactor;
            
            breakChance += nnExtRandom(1, 5);
            if (breakChance >= 5)
                break;

        }

        if (firstFinePath == nullptr) 
        {
            viewSetSystemMessage("No markers with id #%d found for dude #%d! (back = %d)", next, actor->GetIndex(), back);
            return;
        }

        if (selected == nullptr)
            selected = firstFinePath;
    }

    if (!selected)
        return;

    actor->SetTarget(selected);
    selected->SetOwner(actor);
    actor->prevmarker = targetactor; // keep previous marker index here, use actual sprite coords when selecting direction
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolStop(DBloodActor* actor, DBloodActor* targetactor, bool alarm)
{
    auto pSprite = &actor->s();
    if (actor->hasX()) 
    {
        XSPRITE* pXSprite = &actor->x();
        pXSprite->data3 = 0; // reset spot progress
        pXSprite->unused1 &= ~kDudeFlagCrouch; // reset the crouch status
        pXSprite->unused2 = kPatrolMoveForward; // reset path direction
        actor->prevmarker = nullptr;
        pXSprite->targetX = -1; // reset the previous marker index
        if (pXSprite->health <= 0)
            return;

        auto mytarget = actor->GetTarget();

        if (mytarget && mytarget->spr.type == kMarkerPath) 
        {
            if (targetactor == nullptr) pSprite->ang = mytarget->spr.ang & 2047;
            actor->SetTarget(nullptr);
        }

        bool patrol = pXSprite->dudeFlag4; 
        pXSprite->dudeFlag4 = 0;
        if (targetactor && targetactor->hasX() && targetactor->IsDudeActor())
        {
            aiSetTarget(actor, targetactor);
            aiActivateDude(actor);
            
            // alarm only when in non-recoil state?
            //if (((pXSprite->unused1 & kDudeFlagStealth) && stype != kAiStateRecoil) || !(pXSprite->unused1 & kDudeFlagStealth)) {
                if (alarm) aiPatrolAlarmFull(actor, targetactor, Chance(0x0100));
                else aiPatrolAlarmLite(actor, targetactor);
            //}

        }
        else 
        {
            aiInitSprite(actor);
            aiSetTarget(actor, pXSprite->targetX, pXSprite->targetY, pXSprite->targetZ);
        }
        
        pXSprite->dudeFlag4 = patrol; // this must be kept so enemy can patrol after respawn again
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolRandGoalAng(DBloodActor* actor) 
{
    int goal = kAng90;
    if (Chance(0x4000))
        goal = kAng120;

    if (Chance(0x4000))
        goal = kAng180;

    if (Chance(0x8000))
        goal = -goal;

    actor->x().goalAng = (actor->spr.ang + goal) & 2047;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolTurn(DBloodActor* actor)
{
    auto pXSprite = &actor->x();
    auto pSprite = &actor->s();

    int nTurnRange = (getDudeInfo(pSprite->type)->angSpeed << 1) >> 4;
    int nAng = ((pXSprite->goalAng + 1024 - pSprite->ang) & 2047) - 1024;
    pSprite->ang = (pSprite->ang + ClipRange(nAng, -nTurnRange, nTurnRange)) & 2047;

}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolMove(DBloodActor* actor) 
{
    auto pXSprite = &actor->x();
    auto pSprite = &actor->s();
    auto targetactor = actor->GetTarget();

    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax) || !targetactor)
        return;


    int dudeIdx = pSprite->type - kDudeBase;
    switch (pSprite->type) 
    {
        case kDudeCultistShotgunProne:  dudeIdx = kDudeCultistShotgun - kDudeBase;  break;
        case kDudeCultistTommyProne:    dudeIdx = kDudeCultistTommy - kDudeBase;    break;
    }

    spritetype* pTarget = &targetactor->s();
    XSPRITE* pXTarget   = &targetactor->x();
    DUDEINFO* pDudeInfo = &dudeInfo[dudeIdx];
    const DUDEINFO_EXTRA* pExtra = &gDudeInfoExtra[dudeIdx];
    
    int dx = (pTarget->pos.X - pSprite->pos.X);
    int dy = (pTarget->y - pSprite->y);
    int dz = (pTarget->z - (pSprite->z - pDudeInfo->eyeHeight)) * 6;
    int vel = (pXSprite->unused1 & kDudeFlagCrouch) ? kMaxPatrolCrouchVelocity : kMaxPatrolVelocity;
    int goalAng = 341;

    if (pExtra->flying || spriteIsUnderwater(actor)) 
    {
        goalAng >>= 1;
        actor->zvel = dz;
        if (pSprite->flags & kPhysGravity)
            pSprite->flags &= ~kPhysGravity;
    } 
    else if (!pExtra->flying) 
    {
        pSprite->flags |= kPhysGravity | kPhysFalling;
    }

    int nTurnRange = (pDudeInfo->angSpeed << 2) >> 4;
    int nAng = ((pXSprite->goalAng + 1024 - pSprite->ang) & 2047) - 1024;
    pSprite->ang = (pSprite->ang + ClipRange(nAng, -nTurnRange, nTurnRange)) & 2047;
    
    if (abs(nAng) > goalAng || ((pXTarget->waitTime > 0 || pXTarget->data1 == pXTarget->data2) && aiPatrolMarkerReached(actor))) 
    {
       actor->xvel = 0;
       actor->yvel = 0;
        return;
    }
   
    if (actor->hit.hit.type == kHitSprite)
    {
        auto hitactor = actor->hit.hit.actor();
        hitactor->x().dodgeDir =  -1;
        pXSprite->dodgeDir  =   1;
        aiMoveDodge(hitactor);
    }
    else 
    {
        int frontSpeed = aiPatrolGetVelocity(pDudeInfo->frontSpeed, pXTarget->busyTime);
        actor->xvel += MulScale(frontSpeed, Cos(pSprite->ang), 30);
        actor->yvel += MulScale(frontSpeed, Sin(pSprite->ang), 30);
    }

    vel = MulScale(vel, approxDist(dx, dy) << 6, 16);
    actor->xvel = ClipRange(actor->xvel, -vel, vel);
    actor->yvel = ClipRange(actor->yvel, -vel, vel);
    return;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolAlarmLite(DBloodActor* actor, DBloodActor* targetactor) 
{
    if (!actor->hasX() || !actor->IsDudeActor())
        return;

    spritetype* pSprite = &actor->s();
    XSPRITE* pXSprite = &actor->x();
    spritetype* pTarget = &targetactor->s();

    if (pXSprite->health <= 0)
        return;

    int zt1, zb1, zt2, zb2; //int eaz1 = (getDudeInfo(pSprite->type)->eyeHeight * pSprite->yrepeat) << 2;
    GetActorExtents(actor, &zt1, &zb1); 
    GetActorExtents(targetactor, &zt2, &zb2);
    
    BloodStatIterator it(kStatDude);
    while (auto dudeactor = it.Next())
    {
        auto pDude = &dudeactor->s();
        if (dudeactor == actor || !dudeactor->IsDudeActor() || dudeactor->IsPlayerActor() || !dudeactor->hasX())
            continue;

        auto pXDude = &dudeactor->x();
        if (pXDude->health <= 0)
            continue;

        int eaz2 = (getDudeInfo(pTarget->type)->eyeHeight * pTarget->yrepeat) << 2;
        int nDist = approxDist(pDude->pos.X - pSprite->pos.X, pDude->y - pSprite->y);
        if (nDist >= kPatrolAlarmSeeDist || !cansee(pSprite->pos.X, pSprite->y, zt1, pSprite->sector(), pDude->pos.X, pDude->y, pDude->z - eaz2, pDude->sector()))
        {
            nDist = approxDist(pDude->pos.X - pTarget->pos.X, pDude->y - pTarget->y);
            if (nDist >= kPatrolAlarmSeeDist || !cansee(pTarget->pos.X, pTarget->y, zt2, pTarget->sector(), pDude->pos.X, pDude->y, pDude->z - eaz2, pDude->sector()))
                continue;
        }

        if (aiInPatrolState(pXDude->aiState)) aiPatrolStop(dudeactor, dudeactor->GetTarget());
        if (dudeactor->GetTarget() && dudeactor->GetTarget() == actor->GetTarget())
            continue;

        aiSetTarget(dudeactor, targetactor);
        aiActivateDude(dudeactor);
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolAlarmFull(DBloodActor* actor, DBloodActor* targetactor, bool chain) 
{
    if (!actor->hasX() || !actor->IsDudeActor())
        return;

    spritetype* pSprite = &actor->s();
    XSPRITE* pXSprite = &actor->x();
    spritetype* pTarget = &targetactor->s();

    if (pXSprite->health <= 0)
        return;

    int eaz2 = (getDudeInfo(pSprite->type)->eyeHeight * pSprite->yrepeat) << 2;
    int x2 = pSprite->pos.X, y2 = pSprite->y, z2 = pSprite->z - eaz2;
    
    auto pSect2 = pSprite->sector();
    
    int tzt, tzb; 
    GetActorExtents(targetactor, &tzt, &tzb);
    int x3 = pTarget->pos.X, y3 = pTarget->y, z3 = tzt;
    
    auto pSect3 = pTarget->sector();

    BloodStatIterator it(kStatDude);
    while (auto dudeactor = it.Next())
    {
        auto pDude = &dudeactor->s();
        if (dudeactor == actor || !dudeactor->IsDudeActor() || dudeactor->IsPlayerActor() || !dudeactor->hasX())
            continue;

        auto pXDude = &dudeactor->x();
        if (pXDude->health <= 0)
            continue;

        int eaz1 = (getDudeInfo(pDude->type)->eyeHeight * pDude->yrepeat) << 2;
        int x1 = pDude->pos.X, y1 = pDude->y, z1 = pDude->z - eaz1;
        
        auto pSect1 = pDude->sector();

        int nDist1 = approxDist(x1 - x2, y1 - y2);
        int nDist2 = approxDist(x1 - x3, y1 - y3);
        //int hdist = (pXDude->dudeDeaf)  ? 0 : getDudeInfo(pDude->type)->hearDist / 4;
        int sdist = (pXDude->dudeGuard) ? 0 : getDudeInfo(pDude->type)->seeDist / 2;

        if (//(nDist1 < hdist || nDist2 < hdist) ||
            ((nDist1 < sdist && cansee(x1, y1, z1, pSect1, x2, y2, z2, pSect2)) || (nDist2 < sdist && cansee(x1, y1, z1, pSect1, x3, y3, z3, pSect3)))) {

            if (aiInPatrolState(pXDude->aiState)) aiPatrolStop(dudeactor, dudeactor->GetTarget());
            if (dudeactor->GetTarget() && dudeactor->GetTarget() == actor->GetTarget())
                continue;

            if (actor->GetTarget() ) aiSetTarget(dudeactor, actor->GetTarget());
            else aiSetTarget(dudeactor, pSprite->pos.X, pSprite->y, pSprite->z);
            aiActivateDude(dudeactor);

            if (chain)
                aiPatrolAlarmFull(dudeactor, targetactor, Chance(0x0010));
            //Printf("Dude #%d alarms dude #%d", actor->GetIndex(), pDude->index);
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool spritesTouching(DBloodActor *actor1, DBloodActor* actor2) 
{
    if (!actor1->hasX() || !actor2->hasX())
        return false;

    auto hit = &actor1->hit;
    DBloodActor* hitactor = nullptr;
    if (hit->hit.type == kHitSprite) hitactor = hit->hit.actor();
    else if (hit->florhit.type == kHitSprite) hitactor = hit->florhit.actor();
    else if (hit->ceilhit.type == kHitSprite) hitactor = hit->ceilhit.actor();
    else return false;
    return hitactor->hasX() && hitactor == actor2;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool aiCanCrouch(DBloodActor* actor)
{
    auto pSprite = &actor->s();
    if (pSprite->type >= kDudeBase && pSprite->type < kDudeVanillaMax)
        return (gDudeInfoExtra[pSprite->type - kDudeBase].idlcseqofs >= 0 && gDudeInfoExtra[pSprite->type - kDudeBase].mvecseqofs >= 0);
    else if (pSprite->type == kDudeModernCustom || pSprite->type == kDudeModernCustomBurning)
        return actor->genDudeExtra.canDuck;

    return false;

}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool readyForCrit(DBloodActor* hunter, DBloodActor* victim) 
{
    auto pHunter = &hunter->s();
    auto pVictim = &victim->s();
    if (!(pHunter->type >= kDudeBase && pHunter->type < kDudeMax) || !(pVictim->type >= kDudeBase && pVictim->type < kDudeMax))
        return false;

    int dx, dy;
    dx = pVictim->pos.X - pHunter->pos.X;
    dy = pVictim->y - pHunter->y;
    if (approxDist(dx, dy) >= (7000 / ClipLow(gGameOptions.nDifficulty >> 1, 1)))
        return false;
    
    return (abs(((getangle(dx, dy) + 1024 - pVictim->ang) & 2047) - 1024) <= kAng45);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

DBloodActor* aiPatrolSearchTargets(DBloodActor* actor) 
{
    spritetype* pSprite = &actor->s();
    XSPRITE* pXSprite = &actor->x();

    enum { kMaxPatrolFoundSounds = 256 }; // should be the maximum amount of sound channels the engine can play at the same time.
    PATROL_FOUND_SOUNDS patrolBonkles[kMaxPatrolFoundSounds];

    assert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO* pDudeInfo = getDudeInfo(pSprite->type); PLAYER* pPlayer = NULL;
    
    for (int i = 0; i < kMaxPatrolFoundSounds; i++) 
    {
        patrolBonkles[i].snd = patrolBonkles[i].cur = 0;
        patrolBonkles[i].max = ClipLow((gGameOptions.nDifficulty + 1) >> 1, 1);
    }

    int i, j, f, mod, x, y, z, dx, dy, nDist, eyeAboveZ, sndCnt = 0, seeDist, hearDist, feelDist, seeChance, hearChance;
    bool stealth = (pXSprite->unused1 & kDudeFlagStealth);
    bool blind = (pXSprite->dudeGuard); 
    bool deaf = (pXSprite->dudeDeaf);

    DBloodActor* newtarget = nullptr;
    // search for player targets
    for (i = connecthead; i != -1; i = connectpoint2[i]) 
    {
        pPlayer = &gPlayer[i];
        if (!pPlayer->actor->hasX()) continue;

        spritetype* pSpr = pPlayer->pSprite;
        XSPRITE* pXSpr = &pPlayer->actor->x();
        if (pXSpr->health <= 0)
            continue;

        newtarget = nullptr;
        seeChance = hearChance = 0x0000;
        x = pSpr->pos.X, y = pSpr->y, z = pSpr->z, dx = x - pSprite->pos.X, dy = y - pSprite->y; nDist = approxDist(dx, dy);
        seeDist = (stealth) ? pDudeInfo->seeDist / 3 : pDudeInfo->seeDist >> 1;
        hearDist = pDudeInfo->hearDist; feelDist = hearDist >> 1;

        // TO-DO: is there any dudes that sees this patrol dude and sees target?


        if (nDist <= seeDist) 
        {
            eyeAboveZ = (pDudeInfo->eyeHeight * pSprite->yrepeat) << 2;
            if (nDist < seeDist >> 3) GetActorExtents(pPlayer->actor, &z, &j); //use ztop of the target sprite
            if (!cansee(x, y, z, pSpr->sector(), pSprite->pos.X, pSprite->y, pSprite->z - eyeAboveZ, pSprite->sector()))
                continue;
        }
        else 
            continue;

        bool invisible = (powerupCheck(pPlayer, kPwUpShadowCloak) > 0);
        if (spritesTouching(actor, pPlayer->actor) || spritesTouching(pPlayer->actor, actor)) 
        {
            DPrintf(DMSG_SPAMMY, "Patrol dude #%d spot the Player #%d via touch.", actor->GetIndex(), pPlayer->nPlayer + 1);
            if (invisible) pPlayer->pwUpTime[kPwUpShadowCloak] = 0;
            newtarget = pPlayer->actor;
            break;
        }

        if (!deaf) 
        {

            soundEngine->EnumerateChannels([&](FSoundChan* chan)
                {
                    int sndx = 0, sndy = 0;
                    sectortype* searchsect = nullptr;
                    if (chan->SourceType == SOURCE_Actor)
                    {
                        auto emitterActor = (DBloodActor*)chan->Source;
                        if (emitterActor == nullptr) return false; // not a valid source.
                        sndx = emitterActor->spr.pos.X;
                        sndy = emitterActor->spr.y;

                        // sound attached to the sprite
                        if (pPlayer->actor != emitterActor && emitterActor->GetOwner() != actor)
                        {

                            if (!emitterActor->spr.insector()) return false;
                            searchsect = emitterActor->spr.sector();
                        }
                    }
                    else if (chan->SourceType == SOURCE_Unattached)
                    {
                        if (chan->UserData < 0 || !validSectorIndex(chan->UserData)) return false; // not a vaild sector sound.
                        sndx = int(chan->Point[0] * 16);
                        sndy = int(chan->Point[1] * -16);
                        searchsect = &sector[chan->UserData];
                    }
                    if (searchsect == nullptr) return false;
                    int nDist = approxDist(sndx - pSprite->pos.X, sndy - pSprite->y);
                    if (nDist > hearDist) return false;


                    int sndnum = chan->OrgID;

                    // N same sounds per single enemy
                    for (int f = 0; f < sndCnt; f++) 
                    {
                        if (patrolBonkles[f].snd != sndnum) continue;
                        else if (++patrolBonkles[f].cur >= patrolBonkles[f].max)
                            return false;
                    }
                    if (sndCnt < kMaxPatrolFoundSounds - 1)
                        patrolBonkles[sndCnt++].snd = sndnum;

                    bool found = false;
                    BloodSectIterator it(searchsect);
                    while (auto act = it.Next())
                    {
                        if (act->GetOwner() == pPlayer->actor)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found) return false;

                    f = ClipLow((hearDist - nDist) / 8, 0);
                    int sndvol = int(chan->Volume * (80.f / 0.8f));
                    hearChance += mulscale8(sndvol, f) + Random(gGameOptions.nDifficulty);
                    return (hearChance >= kMaxPatrolSpotValue);
                });

            if (invisible && hearChance >= kMaxPatrolSpotValue >> 2) 
            {
                newtarget = pPlayer->actor;
                pPlayer->pwUpTime[kPwUpShadowCloak] = 0;
                invisible = false;
                break;
            }
        }

        if (!invisible && (!deaf || !blind)) 
        {
            if (stealth) 
            {
                switch (pPlayer->lifeMode) 
                {
                    case kModeHuman:
                    case kModeHumanShrink:
                        if (pPlayer->lifeMode == kModeHumanShrink) 
                        {
                            seeDist  -= mulscale8(164, seeDist);
                            feelDist -= mulscale8(164, feelDist);
                        }
                        if (pPlayer->posture == kPostureCrouch) 
                        {
                            seeDist  -= mulscale8(64, seeDist);
                            feelDist -= mulscale8(128, feelDist);
                        }
                        break;
                    case kModeHumanGrown:
                        if (pPlayer->posture != kPostureCrouch) 
                        {
                            seeDist  += mulscale8(72, seeDist);
                            feelDist += mulscale8(64, feelDist);
                        }
                        else 
                        {
                            seeDist  += mulscale8(48, seeDist);
                        }
                        break;
                }
            }

            bool itCanHear = false; bool itCanSee = false;
            feelDist = ClipLow(feelDist, 0);  
            seeDist = ClipLow(seeDist, 0);

            if (hearDist) 
            {
                DBloodActor* act = pPlayer->actor;
                itCanHear = (!deaf && (nDist < hearDist || hearChance > 0));
                if (act && itCanHear && nDist < feelDist && (act->xvel || act->yvel || act->zvel))
                    hearChance += ClipLow(mulscale8(1, ClipLow(((feelDist - nDist) + (abs(act->xvel) + abs(act->yvel) + abs(act->zvel))) >> 6, 0)), 0);
            }

            if (seeDist) 
            {
                int periphery = ClipLow(pDudeInfo->periphery, kAng60);
                int nDeltaAngle = abs(((getangle(dx, dy) + 1024 - pSprite->ang) & 2047) - 1024);
                if ((itCanSee = (!blind && nDist < seeDist && nDeltaAngle < periphery)) == true) 
                {
                    int base = 100 + ((20 * gGameOptions.nDifficulty) - (nDeltaAngle / 5));
                    //seeChance = base - MulScale(ClipRange(5 - gGameOptions.nDifficulty, 1, 4), nDist >> 1, 16);
                    //scale(0x40000, a6, dist2);
                    int d = nDist >> 2;
                    int m = DivScale(d, 0x2000, 8);
                    int t = MulScale(d, m, 8);
                    //int n = mulscale8(nDeltaAngle >> 2, 64);
                    seeChance = ClipRange(DivScale(base, t, 8), 0, kMaxPatrolSpotValue >> 1);
                    //seeChance = scale(0x1000, base, t);
                    //viewSetSystemMessage("SEE CHANCE: %d, BASE %d, DIST %d, T %d", seeChance, base, nDist, t);
                    //itCanSee = false;
                }
            }

            if (!itCanSee && !itCanHear)
                continue;

            if (stealth) 
            {
                // search in stealth regions to modify spot chances
                BloodStatIterator it(kStatModernStealthRegion);
                while (auto iactor = it.Next())
                {
                    if (!iactor->hasX())
                        continue;

                    spritetype* pSteal = &iactor->s();
                    XSPRITE* pXSteal = &iactor->x();
                    if (pXSteal->locked) // ignore locked regions
                        continue;

                    bool fixd = (pSteal->flags & kModernTypeFlag1); // fixed percent value
                    bool both = (pSteal->flags & kModernTypeFlag4); // target AND dude must be in this region
                    bool dude = (both || (pSteal->flags & kModernTypeFlag2)); // dude must be in this region
                    bool trgt = (both || !dude); // target must be in this region
                    bool crouch = (pSteal->flags & kModernTypeFlag8); // target must crouch
                    //bool floor = (pSteal->cstat & CSTAT_SPRITE_BLOCK); // target (or dude?) must touch floor of the sector

                    if (trgt) 
                    {

                        if (pXSteal->data1 > 0)
                        {
                            if (approxDist(abs(pSteal->pos.X - pSpr->pos.X) >> 4, abs(pSteal->y - pSpr->y) >> 4) >= pXSteal->data1)
                                continue;

                        } 
                        else if (pSpr->sector() != pSteal->sector())
                            continue;

                        if (crouch && pPlayer->posture == kPostureStand)
                            continue;
                    }

                    if (dude) 
                    {
                        if (pXSteal->data1 > 0)
                        {
                            if (approxDist(abs(pSteal->pos.X - pSprite->pos.X) >> 4, abs(pSteal->y - pSprite->y) >> 4) >= pXSteal->data1)
                                continue;

                        } 
                        else if (pSpr->sector() != pSteal->sector())
                            continue;
                    }

                    if (itCanHear) 
                    {
                        if (fixd)
                            hearChance = ClipLow(hearChance, pXSteal->data2);

                        mod = (hearChance * pXSteal->data2) / kPercFull;
                        if (fixd)  hearChance = mod; else hearChance += mod;

                        hearChance = ClipRange(hearChance, -kMaxPatrolSpotValue, kMaxPatrolSpotValue);
                    }

                    if (itCanSee) 
                    {
                        if (fixd)
                            seeChance = ClipLow(seeChance, pXSteal->data3);

                        mod = (seeChance * pXSteal->data3) / kPercFull;
                        if (fixd) seeChance = mod; else seeChance += mod;

                        seeChance = ClipRange(seeChance, -kMaxPatrolSpotValue, kMaxPatrolSpotValue);
                    }

                    // trigger this region if target gonna be spot
                    if (pXSteal->txID && pXSprite->data3 + hearChance + seeChance >= kMaxPatrolSpotValue)
                        trTriggerSprite(iactor, kCmdToggle);
                   
                    // continue search another stealth regions to affect chances
                }
            }

            if (itCanHear && hearChance > 0) 
            {
                DPrintf(DMSG_SPAMMY, "Patrol dude #%d hearing the Player #%d.", actor->GetIndex(), pPlayer->nPlayer + 1);
                pXSprite->data3 = ClipRange(pXSprite->data3 + hearChance, -kMaxPatrolSpotValue, kMaxPatrolSpotValue);
                if (!stealth) 
                {
                    newtarget = pPlayer->actor;
                    break;
                }
            }

            if (itCanSee && seeChance > 0) 
            {
                //DPrintf(DMSG_SPAMMY, "Patrol dude #%d seeing the Player #%d.", actor->GetIndex(), pPlayer->nPlayer + 1);
                //pXSprite->data3 += seeChance;
                pXSprite->data3 = ClipRange(pXSprite->data3 + seeChance, -kMaxPatrolSpotValue, kMaxPatrolSpotValue);
                if (!stealth) 
                {
                    newtarget = pPlayer->actor;
                    break;
                }
            }
        }

        // add check for corpses?

        if ((pXSprite->data3 = ClipRange(pXSprite->data3, 0, kMaxPatrolSpotValue)) == kMaxPatrolSpotValue) 
        {
            newtarget = pPlayer->actor;
            break;
        }

        //int perc = (100 * ClipHigh(pXSprite->data3, kMaxPatrolSpotValue)) / kMaxPatrolSpotValue;
        //viewSetSystemMessage("%d / %d / %d / %d", hearChance, seeDist, seeChance, perc);

    }

    if (newtarget) return newtarget;
    pXSprite->data3 -= ClipLow(((kPercFull * pXSprite->data3) / kMaxPatrolSpotValue) >> 2, 3);
    return nullptr;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolFlagsMgr(DBloodActor* sourceactor, DBloodActor* destactor, bool copy, bool init) 
{
    XSPRITE* pXSource = &sourceactor->x();
    XSPRITE* pXDest = &destactor->x();

    // copy flags
    if (copy) 
    {
        pXDest->dudeFlag4  = pXSource->dudeFlag4;
        pXDest->dudeAmbush = pXSource->dudeAmbush;
        pXDest->dudeGuard  = pXSource->dudeGuard;
        pXDest->dudeDeaf   = pXSource->dudeDeaf;
        pXDest->unused1    = pXSource->unused1;

        if (pXSource->unused1 & kDudeFlagStealth) pXDest->unused1 |= kDudeFlagStealth;
        else pXDest->unused1 &= ~kDudeFlagStealth;
    }

    // do init
    if (init) 
    {
        if (!pXDest->dudeFlag4) 
        {
            if (aiInPatrolState(pXDest->aiState))
                aiPatrolStop(destactor, nullptr);
        }
        else 
        {
            if (aiInPatrolState(pXDest->aiState))
                return;
            
            destactor->SetTarget(nullptr);
            pXDest->stateTimer = 0;
            
            aiPatrolSetMarker(destactor);
            if (spriteIsUnderwater(destactor)) aiPatrolState(destactor, kAiStatePatrolWaitW);
            else aiPatrolState(destactor, kAiStatePatrolWaitL);
            pXDest->data3 = 0; // reset the spot progress
        }
    }
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool aiPatrolGetPathDir(DBloodActor* actor, DBloodActor* marker) 
{
    if (actor->x().unused2 == kPatrolMoveForward) return (marker->x().data2 == -2) ? (bool)kPatrolMoveBackward : (bool)kPatrolMoveForward;
    else return (findNextMarker(marker, kPatrolMoveBackward) != nullptr) ? (bool)kPatrolMoveBackward : (bool)kPatrolMoveForward;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

void aiPatrolThink(DBloodActor* actor)
{
    auto pXSprite = &actor->x();
    auto pSprite = &actor->s();

    assert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);

    DBloodActor* targetactor;
    int stateTimer;
    auto markeractor = actor->GetTarget();
    if ((targetactor = aiPatrolSearchTargets(actor)) != nullptr) 
    {
        aiPatrolStop(actor, targetactor, pXSprite->dudeAmbush);
        return;
    }

    
    bool crouch = (pXSprite->unused1 & kDudeFlagCrouch), uwater = spriteIsUnderwater(actor);
    if (markeractor == nullptr || (pSprite->type == kDudeModernCustom && ((uwater && !canSwim(actor)) || !canWalk(actor)))) 
    {
        aiPatrolStop(actor, nullptr);
        return;
    }
    
    spritetype* pMarker = &markeractor->s();
    XSPRITE* pXMarker = &markeractor->x();
    const DUDEINFO_EXTRA* pExtra = &gDudeInfoExtra[pSprite->type - kDudeBase];
    bool isFinal = ((!pXSprite->unused2 && pXMarker->data2 == -1) || (pXSprite->unused2 && pXMarker->data1 == -1));
    bool reached = false;
    
    if (aiPatrolWaiting(pXSprite->aiState)) 
    {
        //viewSetSystemMessage("WAIT %d / %d", pXSprite->targetY, pXSprite->stateTimer);
        
        if (pXSprite->stateTimer > 0 || pXMarker->data1 == pXMarker->data2) 
        {
            if (pExtra->flying)
                actor->zvel = Random2(0x8000);

            // turn while waiting
            if (pMarker->flags & kModernTypeFlag16) 
            {
                stateTimer = pXSprite->stateTimer;
                
                if (--pXSprite->unused4 <= 0) 
                {
                    if (uwater) aiPatrolState(actor, kAiStatePatrolTurnW);
                    else if (crouch) aiPatrolState(actor, kAiStatePatrolTurnC);
                    else aiPatrolState(actor, kAiStatePatrolTurnL);
                    pXSprite->unused4 = kMinPatrolTurnDelay + Random(kPatrolTurnDelayRange);
                }

                // must restore stateTimer for waiting
                pXSprite->stateTimer = stateTimer;
            }
            return;
        }

        // trigger at departure
        if (pXMarker->triggerOff) 
        {
            // send command
            if (pXMarker->txID) 
            {
                evSendActor(markeractor, pXMarker->txID, (COMMAND_ID)pXMarker->command);

                // copy dude flags for current dude
            }
            else if (pXMarker->command == kCmdDudeFlagsSet) 
            {
                aiPatrolFlagsMgr(markeractor, actor, true, true);
                if (!pXSprite->dudeFlag4) // this dude is not in patrol anymore
                    return;
            }
        }

        // release the enemy
        if (isFinal) 
        {
            aiPatrolStop(actor, nullptr);
            return;
        }

        // move next marker
        aiPatrolSetMarker(actor);

    } else if (aiPatrolTurning(pXSprite->aiState)) 
    {
        //viewSetSystemMessage("TURN");
        if ((int)pSprite->ang == (int)pXSprite->goalAng) 
        {
            // save imer for waiting
            stateTimer = pXSprite->stateTimer;
            
            if (uwater) aiPatrolState(actor, kAiStatePatrolWaitW);
            else if (crouch) aiPatrolState(actor, kAiStatePatrolWaitC);
            else aiPatrolState(actor, kAiStatePatrolWaitL);
            
            // must restore it
            pXSprite->stateTimer = stateTimer;
        }
        return;

    }
    else if ((reached = aiPatrolMarkerReached(actor)) == true) 
    {
        pXMarker->isTriggered = pXMarker->triggerOnce; // can't select this marker for path anymore if true

        if (pMarker->flags > 0) 
        {
            if ((pMarker->flags & kModernTypeFlag2) && (pMarker->flags & kModernTypeFlag1)) crouch = !crouch;
            else if (pMarker->flags & kModernTypeFlag2) crouch = false;
            else if ((pMarker->flags & kModernTypeFlag1) && aiCanCrouch(actor)) crouch = true;
        }

        if (pXMarker->waitTime > 0 || pXMarker->data1 == pXMarker->data2) 
        {
            // take marker's angle
            if (!(pMarker->flags & kModernTypeFlag4)) 
            {
                pXSprite->goalAng = ((!(pMarker->flags & kModernTypeFlag8) && pXSprite->unused2) ? pMarker->ang + kAng180 : pMarker->ang) & 2047;
                if ((int)pSprite->ang != (int)pXSprite->goalAng) // let the enemy play move animation while turning
                    return;
            }

            if (markeractor->GetOwner() == actor)
                markeractor->SetOwner(aiPatrolMarkerBusy(actor, markeractor));

            // trigger at arrival
            if (pXMarker->triggerOn) 
            {
                // send command
                if (pXMarker->txID) 
                {
                    evSendActor(markeractor, pXMarker->txID, (COMMAND_ID)pXMarker->command);
                } 
                else if (pXMarker->command == kCmdDudeFlagsSet) 
                {
                // copy dude flags for current dude
                    aiPatrolFlagsMgr(markeractor, actor, true, true);
                    if (!pXSprite->dudeFlag4) // this dude is not in patrol anymore
                        return;
                }
            }

            if (uwater) aiPatrolState(actor, kAiStatePatrolWaitW);
            else if (crouch) aiPatrolState(actor, kAiStatePatrolWaitC);
            else aiPatrolState(actor, kAiStatePatrolWaitL);

            if (pXMarker->waitTime)
                pXSprite->stateTimer = (pXMarker->waitTime * 120) / 10;

            if (pMarker->flags & kModernTypeFlag16)
                pXSprite->unused4 = kMinPatrolTurnDelay + Random(kPatrolTurnDelayRange);

            return;
        }
        else 
        {
            if (markeractor->GetOwner() == actor)
                markeractor->SetOwner(aiPatrolMarkerBusy(actor, markeractor));

            if (pXMarker->triggerOn || pXMarker->triggerOff) 
            {
                if (pXMarker->txID) 
                {
                    // send command at arrival
                    if (pXMarker->triggerOn)
                        evSendActor(markeractor, pXMarker->txID, (COMMAND_ID)pXMarker->command);

                    // send command at departure
                    if (pXMarker->triggerOff)
                        evSendActor(markeractor, pXMarker->txID, (COMMAND_ID)pXMarker->command);

                // copy dude flags for current dude
                }
                else if (pXMarker->command == kCmdDudeFlagsSet) 
                {
                    aiPatrolFlagsMgr(markeractor, actor, true, true);
                    if (!pXSprite->dudeFlag4) // this dude is not in patrol anymore
                        return;
                }
            }

            // release the enemy
            if (isFinal) 
            {
                aiPatrolStop(actor, nullptr);
                return;
            }

            // move the next marker
            aiPatrolSetMarker(actor);
        }
    }
    
    nnExtAiSetDirection(actor, getangle(pMarker->pos.X - pSprite->pos.X, pMarker->y - pSprite->y));

    if (aiPatrolMoving(pXSprite->aiState) && !reached) return;
    else if (uwater) aiPatrolState(actor, kAiStatePatrolMoveW);
    else if (crouch) aiPatrolState(actor, kAiStatePatrolMoveC);
    else aiPatrolState(actor, kAiStatePatrolMoveL);
    return;

}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

int listTx(DBloodActor* actor, int tx) 
{
    if (txIsRanged(actor)) 
    {
        XSPRITE* pXRedir = &actor->x();
        if (tx == -1) tx = pXRedir->data1;
        else if (tx < pXRedir->data4) tx++;
        else tx = -1;
    } 
    else 
    {
        if (tx == -1) 
        {
            for (int i = 0; i <= 3; i++) 
            {
                if ((tx = GetDataVal(actor, i)) <= 0) continue;
                else return tx;
            }
        } 
        else 
        {
            int saved = tx; bool savedFound = false;
            for (int i = 0; i <= 3; i++) 
            {
                tx = GetDataVal(actor, i);
                if (savedFound && tx > 0) return tx;
                else if (tx != saved) continue;
                else savedFound = true;
            }
        }
        tx = -1;
    }
    return tx;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

DBloodActor* evrIsRedirector(DBloodActor* actor) 
{
    if (actor)
    {
        switch (actor->spr.type) 
        {
        case kModernRandomTX:
        case kModernSequentialTX:
            if (actor->hasX() && actor->x().command == kCmdLink && !actor->x().locked) return actor;
            break;
        }
    }

    return nullptr;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

DBloodActor* evrListRedirectors(int objType, sectortype* pSector, walltype* pWall, DBloodActor* objActor, DBloodActor* pXRedir, int* tx) 
{
    if (!gEventRedirectsUsed) return nullptr;
    else if (pXRedir && (*tx = listTx(pXRedir, *tx)) != -1)
        return pXRedir;

    int id = 0;
    switch (objType) 
    {
        case OBJ_SECTOR:
        {
            if (!pSector->hasX()) return nullptr;
            id = pSector->xs().txID;
            break;
        }
        case OBJ_SPRITE:
            if (!objActor) return nullptr;
            id = objActor->x().txID;
            break;
        case OBJ_WALL:
        {
            if (!pWall->hasX()) return nullptr;
            id = pWall->xw().txID;
            break;
        }
        default:
            return nullptr;
    }

    bool prevFound = false;
    for (int i = bucketHead[id]; i < bucketHead[id + 1]; i++) 
    {
        if (!rxBucket[i].isActor()) continue;
        auto rxactor = rxBucket[i].actor();

        auto pXSpr = evrIsRedirector(rxactor);
        if (!pXSpr) continue;
        else if (prevFound || pXRedir == nullptr) 
        {
            *tx = listTx(pXSpr, *tx); 
            return pXSpr; 
        }
        else if (pXRedir != pXSpr) continue;
        else prevFound = true;
    }

    *tx = -1;
    return NULL;
}

//---------------------------------------------------------------------------
//
// this function checks if all TX objects have the same value
//
//---------------------------------------------------------------------------

bool incDecGoalValueIsReached(DBloodActor* actor) 
{
    XSPRITE* pXSprite = &actor->x();
    if (pXSprite->data3 != pXSprite->sysData1) return false;
    char buffer[7]; 
    snprintf(buffer, 7, "%d", abs(pXSprite->data1)); 
    int len = int(strlen(buffer)); 
    int rx = -1;

    for (int i = bucketHead[pXSprite->txID]; i < bucketHead[pXSprite->txID + 1]; i++) 
    {
        if (!rxBucket[i].isActor()) continue;
        auto rxactor = rxBucket[i].actor();

        if (evrIsRedirector(rxactor)) continue;
        for (int a = 0; a < len; a++) 
        {
            if (getDataFieldOfObject(rxBucket[i], (buffer[a] - 52) + 4) != pXSprite->data3)
                return false;
        }
    }

    DBloodActor* pXRedir = nullptr; // check redirected TX buckets
    while ((pXRedir = evrListRedirectors(OBJ_SPRITE, nullptr, nullptr, actor, pXRedir, &rx)) != nullptr)
    {
        for (int i = bucketHead[rx]; i < bucketHead[rx + 1]; i++) 
        {
            for (int a = 0; a < len; a++) 
            {
                if (getDataFieldOfObject(rxBucket[i], (buffer[a] - 52) + 4) != pXSprite->data3)
                    return false;
            }
        }
    }
    
    return true;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void seqSpawnerOffSameTx(DBloodActor* actor) 
{
    auto pXSource = &actor->x();
    BloodSpriteIterator it;
    while (auto iactor = it.Next())
    {
        if (iactor->spr.type != kModernSeqSpawner || !iactor->hasX() || iactor == actor) continue;
        XSPRITE* pXSprite = &iactor->x();
        if (pXSprite->txID == pXSource->txID && pXSprite->state == 1) 
        {
            evKillActor(iactor);
            pXSprite->state = 0;
        }
    }
}

//---------------------------------------------------------------------------
//
// this function can be called via sending numbered command to TX kChannelModernEndLevelCustom
// it allows to set custom next level instead of taking it from INI file.
//
//---------------------------------------------------------------------------

void levelEndLevelCustom(int nLevel) 
{
    gGameOptions.uGameFlags |= GF_AdvanceLevel;
    gNextLevel = FindMapByIndex(currentLevel->cluster, nLevel + 1);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void callbackUniMissileBurst(DBloodActor* actor, sectortype*) // 22
{
    if (!actor) return;
    spritetype* pSprite = &actor->s();
    if (pSprite->statnum != kStatProjectile) return;
    int nAngle = getangle(actor->xvel, actor->yvel);
    int nRadius = 0x55555;

    for (int i = 0; i < 8; i++)
    {
        auto burstactor = actSpawnSprite(actor, 5);
        if (!burstactor) break;
        spritetype* pBurst = &burstactor->s();

        pBurst->type = pSprite->type;
        pBurst->shade = pSprite->shade;
        pBurst->picnum = pSprite->picnum;

        
        pBurst->cstat = pSprite->cstat;
        if ((pBurst->cstat & CSTAT_SPRITE_BLOCK)) 
        {
            pBurst->cstat &= ~CSTAT_SPRITE_BLOCK; // we don't want missiles impact each other
            evPostActor(burstactor, 100, kCallbackMissileSpriteBlock); // so set blocking flag a bit later
        }

        pBurst->pal = pSprite->pal;
        pBurst->clipdist = pSprite->clipdist / 4;
        pBurst->flags = pSprite->flags;
        pBurst->xrepeat = pSprite->xrepeat / 2;
        pBurst->yrepeat = pSprite->yrepeat / 2;
        pBurst->ang = ((pSprite->ang + missileInfo[pSprite->type - kMissileBase].angleOfs) & 2047);
        burstactor->SetOwner(actor);

        actBuildMissile(burstactor, actor);

        int nAngle2 = (i << 11) / 8;
        int dx = 0;
        int dy = mulscale30r(nRadius, Sin(nAngle2));
        int dz = mulscale30r(nRadius, -Cos(nAngle2));
        if (i & 1)
        {
            dy >>= 1;
            dz >>= 1;
        }
        RotateVector(&dx, &dy, nAngle);
        burstactor->xvel += dx;
        burstactor->yvel += dy;
        burstactor->zvel += dz;
        evPostActor(burstactor, 960, kCallbackRemove);
    }
    evPostActor(actor, 0, kCallbackRemove);
}


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void callbackMakeMissileBlocking(DBloodActor* actor, sectortype*) // 23
{
    if (!actor || actor->spr.statnum != kStatProjectile) return;
    actor->spr.cstat |= CSTAT_SPRITE_BLOCK;
}

void callbackGenDudeUpdate(DBloodActor* actor, sectortype*) // 24
{
    if (actor)
        genDudeUpdate(actor);
}

void clampSprite(DBloodActor* actor, int which) 
{
    auto pSprite = &actor->s();
    int zTop, zBot;
    if (pSprite->insector())
    {
        GetSpriteExtents(pSprite, &zTop, &zBot);
        if (which & 0x01)
            pSprite->z += ClipHigh(getflorzofslopeptr(pSprite->sector(), pSprite->pos.X, pSprite->y) - zBot, 0);
        if (which & 0x02)
            pSprite->z += ClipLow(getceilzofslopeptr(pSprite->sector(), pSprite->pos.X, pSprite->y) - zTop, 0);

    }

}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

FSerializer& Serialize(FSerializer& arc, const char* keyname, GENDUDEEXTRA& w, GENDUDEEXTRA* def)
{
    if (arc.BeginObject(keyname))
    {
        arc.Array("initvals", w.initVals, 3)
            .Array("availdeaths", w.availDeaths, kDamageMax)
            ("movespeed", w.moveSpeed)
            ("firedist", w.fireDist)
            ("throwdist", w.throwDist)
            ("curweapon", w.curWeapon)
            ("weapontype", w.weaponType)
            ("basedispersion", w.baseDispersion)
            ("slavecount", w.slaveCount)
            ("lifeleech", w.pLifeLeech)
            .Array("slaves", w.slave, w.slaveCount)
            .Array("dmgcontrol", w.dmgControl, kDamageMax)
            .Array("updreq", w.updReq, kGenDudePropertyMax)
            ("flags", w.flags)
            .EndObject();
    }
    return arc;
}

FSerializer& Serialize(FSerializer& arc, const char* keyname, SPRITEMASS& w, SPRITEMASS* def)
{
    static SPRITEMASS nul;
    if (arc.isReading()) w = {};
    if (arc.BeginObject(keyname))
    {
        arc ("seq", w.seqId, &nul.seqId)
            ("picnum", w.picnum, &nul.picnum)
            ("xrepeat", w.xrepeat, &nul.xrepeat)
            ("yrepeat", w.yrepeat, &nul.yrepeat)
            ("clipdist", w.clipdist)
            ("mass", w.mass)
            ("airvel", w.airVel)
            ("fraction", w.fraction)
            .EndObject();
    }
    return arc;
}

FSerializer& Serialize(FSerializer& arc, const char* keyname, OBJECTS_TO_TRACK& w, OBJECTS_TO_TRACK* def)
{
    static OBJECTS_TO_TRACK nul;
    if (arc.isReading()) w = {};
    if (arc.BeginObject(keyname))
    {
        arc("obj", w.obj, &nul.obj)
            ("cmd", w.cmd, &nul.cmd)
            .EndObject();
    }
    return arc;
}

FSerializer& Serialize(FSerializer& arc, const char* keyname, TRCONDITION& w, TRCONDITION* def)
{
    static TRCONDITION nul;
    if (arc.isReading()) w = {};
    if (arc.BeginObject(keyname))
    {
        arc("length", w.length, &nul.length)
            ("xindex", w.actor, &nul.actor)
            .Array("obj", w.obj, w.length)
            .EndObject();
    }
    return arc;
}

void SerializeNNExts(FSerializer& arc)
{
    if (arc.BeginObject("nnexts"))
    {
        arc ("proxyspritescount", gProxySpritesCount)
            .Array("proxyspriteslist", gProxySpritesList, gProxySpritesCount)
            ("sightspritescount", gSightSpritesCount)
            .Array("sightspriteslist", gSightSpritesList, gSightSpritesCount)
            ("physspritescount", gPhysSpritesCount)
            .Array("physspriteslist", gPhysSpritesList, gPhysSpritesCount)
            ("impactspritescount", gImpactSpritesCount)
            .Array("impactspriteslist", gImpactSpritesList, gImpactSpritesCount)
            ("eventredirects", gEventRedirectsUsed)
            ("trconditioncount",  gTrackingCondsCount)
            .Array("trcondition", gCondition, gTrackingCondsCount)
            .EndObject();
    }
}

///////////////////////////////////////////////////////////////////
// This file provides modern features for mappers.
// For full documentation please visit http://cruo.bloodgame.ru/xxsystem
///////////////////////////////////////////////////////////////////
END_BLD_NS

#endif
