//-------------------------------------------------------------------------
/*
Copyright (C) 1997, 2005 - 3D Realms Entertainment

This file is part of Shadow Warrior version 1.2

Shadow Warrior is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Original Source: 1997 - Frank Maddin and Jim Norwood
Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------
#include "ns.h"
#include "build.h"

#include "names2.h"
#include "panel.h"
#include "misc.h"
#include "hw_drawinfo.h"

BEGIN_SW_NS

////////////////////////////////////////////////////////////////////
//
// FLOOR ABOVE FLOOR
//
////////////////////////////////////////////////////////////////////


#define ZMAX 400
typedef struct
{
    sectortype* sect[ZMAX];
    int32_t zval[ZMAX];
    int16_t pic[ZMAX];
    int16_t zcount;
    int16_t slope[ZMAX];
} SAVE, *SAVEp;

SAVE save;

bool FAF_DebugView = false;

DSWActor* insertActor(sectortype* sect, int statnum)
{
    auto pActor = static_cast<DSWActor*>(::InsertActor(sect, statnum));
    auto pSprite = &pActor->s();

    pSprite->x = pSprite->y = pSprite->z = 0;
    pSprite->cstat = 0;
    pSprite->picnum = 0;
    pSprite->shade = 0;
    pSprite->pal = 0;
    pSprite->clipdist = 0;
    pSprite->xrepeat = pSprite->yrepeat = 0;
    pSprite->xoffset = pSprite->yoffset = 0;
    pSprite->ang = 0;
    pSprite->owner = -1;
    pSprite->xvel = pSprite->yvel = pSprite->zvel = 0;
    pSprite->lotag = 0;
    pSprite->hitag = 0;
    pSprite->extra = 0;

    return pActor;
}

bool FAF_Sector(sectortype* sect)
{
    SPRITEp sp;

    SWSectIterator it(sect);
    while (auto actor = it.Next())
    {
        sp = &actor->s();

        if (sp->statnum == STAT_FAF &&
            (sp->hitag >= VIEW_LEVEL1 && sp->hitag <= VIEW_LEVEL6))
        {
            return true;
        }
    }

    return false;
}

void SetWallWarpHitscan(sectortype* sect)
{
    DSWActor* sp_warp;

    if (!WarpSectorInfo(sect, &sp_warp))
        return;

    if (!sp_warp)
        return;

    auto start_wall = sect->firstWall();
    auto wall_num = start_wall;

    // Travel all the way around loop setting wall bits
    do
    {
        if (wall_num->twoSided())
            SET(wall_num->cstat, CSTAT_WALL_WARP_HITSCAN);
        wall_num = wall_num->point2Wall();
    }
    while (wall_num != start_wall);
}

void ResetWallWarpHitscan(sectortype* sect)
{
    auto start_wall = sect->firstWall();
    auto wall_num = start_wall;

    // Travel all the way around loop setting wall bits
    do
    {
        RESET(wall_num->cstat, CSTAT_WALL_WARP_HITSCAN);
        wall_num = wall_num->point2Wall();
    }
    while (wall_num != start_wall);
}

void
FAFhitscan(int32_t x, int32_t y, int32_t z, sectortype* sect,
    int32_t xvect, int32_t yvect, int32_t zvect,
    HitInfo& hit, int32_t clipmask)
{
    int loz, hiz;
    auto newsector = sect;
    int startclipmask = 0;
    bool plax_found = false;

    if (clipmask == CLIPMASK_MISSILE)
        startclipmask = CLIPMASK_WARP_HITSCAN;

    hitscan({ x, y, z }, sect, { xvect, yvect, zvect }, hit, startclipmask);

    if (hit.hitSector == nullptr)
        return;

    if (hit.hitWall != nullptr)
    {
        // hitscan warping
        if (TEST(hit.hitWall->cstat, CSTAT_WALL_WARP_HITSCAN))
        {
            // back it up a bit to get a correct warp location
            hit.hitpos.x -= xvect>>9;
            hit.hitpos.y -= yvect>>9;

            // warp to new x,y,z, sectnum
            sectortype* newsect;
            if (Warp(&hit.hitpos.x, &hit.hitpos.y, &hit.hitpos.z, &newsect))
            {
                // hitscan needs to pass through dest sect
                ResetWallWarpHitscan(newsect);

                // NOTE: This could be recursive I think if need be
                auto pos = hit.hitpos;
                hitscan(pos, newsect, { xvect, yvect, zvect }, hit, startclipmask);

                // reset hitscan block for dest sect
                SetWallWarpHitscan(newsect);

                return;
            }
            else
            {
                ASSERT(true == false);
            }
        }
    }

    // make sure it hit JUST a sector before doing a check
    if (hit.hitWall == nullptr && hit.actor() == nullptr)
    {
        if (TEST(hit.hitSector->extra, SECTFX_WARP_SECTOR))
        {
            if (TEST(hit.hitSector->firstWall()->cstat, CSTAT_WALL_WARP_HITSCAN))
            {
                // hit the floor of a sector that is a warping sector
                sectortype* newsect;
                if (Warp(&hit.hitpos.x, &hit.hitpos.y, &hit.hitpos.z, &newsect))
                {
                    auto pos = hit.hitpos;
                    hitscan(pos, newsect, { xvect, yvect, zvect }, hit, clipmask);
                    return;
                }
            }
            else
            {
                sectortype* newsect;
                if (WarpPlane(&hit.hitpos.x, &hit.hitpos.y, &hit.hitpos.z, &newsect))
                {
                    auto pos = hit.hitpos;
                    hitscan(pos, newsect, { xvect, yvect, zvect }, hit, clipmask);
                    return;
                }
            }
        }

        getzsofslopeptr(hit.hitSector, hit.hitpos.x, hit.hitpos.y, &hiz, &loz);
        if (abs(hit.hitpos.z - loz) < Z(4))
        {
            if (FAF_ConnectFloor(hit.hitSector) && !TEST(hit.hitSector->floorstat, FLOOR_STAT_FAF_BLOCK_HITSCAN))
            {
                updatesectorz(hit.hitpos.x, hit.hitpos.y, hit.hitpos.z + Z(12), &newsector);
                plax_found = true;
            }
        }
        else if (labs(hit.hitpos.z - hiz) < Z(4))
        {
            if (FAF_ConnectCeiling(hit.hitSector) && !TEST(hit.hitSector->floorstat, CEILING_STAT_FAF_BLOCK_HITSCAN))
            {
                updatesectorz(hit.hitpos.x, hit.hitpos.y, hit.hitpos.z - Z(12), &newsector);
                plax_found = true;
            }
        }
    }

    if (plax_found)
    {
        auto pos = hit.hitpos;
        hitscan(pos, newsector, { xvect, yvect, zvect }, hit, clipmask);
    }
}

bool FAFcansee(int32_t xs, int32_t ys, int32_t zs, sectortype* sects,
          int32_t xe, int32_t ye, int32_t ze, sectortype* secte)
{
    int loz, hiz;
    auto newsect = sects;
    int xvect, yvect, zvect;
    short ang;
    int dist;
    bool plax_found = false;
    vec3_t s = { xs, ys, zs };

    // ASSERT(sects >= 0 && secte >= 0);

    // early out to regular routine
    if ((!sects || !FAF_Sector(sects)) && (!secte || !FAF_Sector(secte)))
    {
        return !!cansee(xs,ys,zs,sects,xe,ye,ze,secte);
    }

    // get angle
    ang = getangle(xe - xs, ye - ys);

    // get x,y,z, vectors
    xvect = bcos(ang);
    yvect = bsin(ang);

    // find the distance to the target
    dist = ksqrt(SQ(xe - xs) + SQ(ye - ys));

    if (dist != 0)
    {
        if (xe - xs != 0)
            zvect = Scale(xvect, ze - zs, xe - xs);
        else if (ye - ys != 0)
            zvect = Scale(yvect, ze - zs, ye - ys);
        else
            zvect = 0;
    }
    else
        zvect = 0;

    HitInfo hit{};
    hitscan(s, sects, { xvect, yvect, zvect }, hit, CLIPMASK_MISSILE);

    if (hit.hitSector == nullptr)
        return false;

    // make sure it hit JUST a sector before doing a check
    if (hit.hitWall == nullptr && hit.actor() == nullptr)
    {
        getzsofslopeptr(hit.hitSector, hit.hitpos.x, hit.hitpos.y, &hiz, &loz);
        if (labs(hit.hitpos.z - loz) < Z(4))
        {
            if (FAF_ConnectFloor(hit.hitSector))
            {
                updatesectorz(hit.hitpos.x, hit.hitpos.y, hit.hitpos.z + Z(12), &newsect);
                plax_found = true;
            }
        }
        else if (labs(hit.hitpos.z - hiz) < Z(4))
        {
            if (FAF_ConnectCeiling(hit.hitSector))
            {
                updatesectorz(hit.hitpos.x, hit.hitpos.y, hit.hitpos.z - Z(12), &newsect);
                plax_found = true;
            }
        }
    }
    else
    {
        return !!cansee(xs,ys,zs,sects,xe,ye,ze,secte);
    }

    if (plax_found)
        return !!cansee(hit.hitpos.x, hit.hitpos.y, hit.hitpos.z, newsect, xe, ye, ze, secte);

    return false;
}


int GetZadjustment(sectortype* sect, short hitag)
{
    SPRITEp sp;

    if (sect == nullptr || !TEST(sect->extra, SECTFX_Z_ADJUST))
        return 0;

    SWStatIterator it(STAT_ST1);
    while (auto itActor = it.Next())
    {
        sp = &itActor->s();

        if (sp->hitag == hitag && sp->sector() == sect)
        {
            return Z(sp->lotag);
        }
    }

    return 0;
}

bool SectorZadjust(const Collision& ceilhit, int32_t* hiz, const Collision& florhit, int32_t* loz)
{
    extern int PlaxCeilGlobZadjust, PlaxFloorGlobZadjust;
    int z_amt = 0;

    bool SkipFAFcheck = false;

    if (florhit.type != -1)
    {
        switch (florhit.type)
        {
        case kHitSector:
        {
            auto hit_sector = florhit.hitSector;

            // don't jack with connect sectors
            if (FAF_ConnectFloor(hit_sector))
            {
                // rippers were dying through the floor in $rock
                if (TEST(hit_sector->floorstat, CEILING_STAT_FAF_BLOCK_HITSCAN))
                    break;

                if (TEST(hit_sector->extra, SECTFX_Z_ADJUST))
                {
                    // see if a z adjust ST1 is around
                    z_amt = GetZadjustment(hit_sector, FLOOR_Z_ADJUST);

                    if (z_amt)
                    {
                        // explicit z adjust overrides Connect Floor
                        *loz += z_amt;
                        SkipFAFcheck = true;
                    }
                }

                break;
            }

            if (!TEST(hit_sector->extra, SECTFX_Z_ADJUST))
                break;

            // see if a z adjust ST1 is around
            z_amt = GetZadjustment(hit_sector, FLOOR_Z_ADJUST);

            if (z_amt)
            {
                // explicit z adjust overrides plax default
                *loz += z_amt;
            }
            else
            // default adjustment for plax
            if (TEST(hit_sector->floorstat, FLOOR_STAT_PLAX))
            {
                *loz += PlaxFloorGlobZadjust;
            }

            break;
        }
        }
    }

    if (ceilhit.type != -1)
    {
        switch (ceilhit.type)
        {
        case kHitSector:
        {
            auto hit_sector = ceilhit.hitSector;

            // don't jack with connect sectors
            if (FAF_ConnectCeiling(hit_sector))
            {
                if (TEST(hit_sector->extra, SECTFX_Z_ADJUST))
                {
                    // see if a z adjust ST1 is around
                    z_amt = GetZadjustment(hit_sector, CEILING_Z_ADJUST);

                    if (z_amt)
                    {
                        // explicit z adjust overrides Connect Floor
                        *loz += z_amt;
                        SkipFAFcheck = true;
                    }
                }

                break;
            }

            if (!TEST(hit_sector->extra, SECTFX_Z_ADJUST))
                break;

            // see if a z adjust ST1 is around
            z_amt = GetZadjustment(hit_sector, CEILING_Z_ADJUST);

            if (z_amt)
            {
                // explicit z adjust overrides plax default
                *hiz -= z_amt;
            }
            else
            // default adjustment for plax
            if (TEST(hit_sector->ceilingstat, CEILING_STAT_PLAX))
            {
                *hiz -= PlaxCeilGlobZadjust;
            }

            break;
        }
        }
    }

    return SkipFAFcheck;
}

void WaterAdjust(const Collision& florhit, int32_t* loz)
{
    if (florhit.type == kHitSector)
    {
        auto sect = florhit.hitSector;
        if (!sect->hasU()) return;

        if (sect->hasU() && FixedToInt(sect->depth_fixed))
            *loz += Z(FixedToInt(sect->depth_fixed));
    }
}

void FAFgetzrange(vec3_t pos, sectortype* sect, int32_t* hiz, Collision* ceilhit, int32_t* loz, Collision* florhit, int32_t clipdist, int32_t clipmask)
{
    int foo1;
    Collision foo2;
    bool SkipFAFcheck;
    Collision trash; trash.invalidate();

    // IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // This will return invalid FAF ceiling and floor heights inside of analyzesprite
    // because the ceiling and floors get moved out of the way for drawing.

    // early out to regular routine
    if (sect == nullptr || !FAF_ConnectArea(sect))
    {
        getzrange(pos, sect, hiz,  *ceilhit, loz,  *florhit, clipdist, clipmask);
        SectorZadjust(*ceilhit, hiz, *florhit, loz);
        WaterAdjust(*florhit, loz);
        return;
    }

    getzrange(pos, sect, hiz,  *ceilhit, loz,  *florhit, clipdist, clipmask);
    SkipFAFcheck = SectorZadjust(*ceilhit, hiz, *florhit, loz);
    WaterAdjust(*florhit, loz);

    if (SkipFAFcheck)
        return;

    if (FAF_ConnectCeiling(sect))
    {
        auto uppersect = sect;
        int newz = *hiz - Z(2);

        if (ceilhit->type == kHitSprite) return;

        updatesectorz(pos.x, pos.y, newz, &uppersect);
        if (uppersect == nullptr)
            return;
        vec3_t npos = pos;
        npos.z = newz;
        getzrange(npos, uppersect, hiz,  *ceilhit, &foo1, foo2, clipdist, clipmask);
        SectorZadjust(*ceilhit, hiz, trash, nullptr);
    }
    else if (FAF_ConnectFloor(sect) && !TEST(sect->floorstat, FLOOR_STAT_FAF_BLOCK_HITSCAN))
    {
        auto lowersect = sect;
        int newz = *loz + Z(2);

        if (florhit->type == kHitSprite) return;

        updatesectorz(pos.x, pos.y, newz, &lowersect);
        if (lowersect == nullptr)
            return; // _ErrMsg(ERR_STD_ARG, "Did not find a sector at %d, %d, %d", x, y, newz);
        vec3_t npos = pos;
        npos.z = newz;
        getzrange(npos, lowersect, &foo1, foo2, loz,  *florhit, clipdist, clipmask);
        SectorZadjust(trash, nullptr, *florhit, loz);
        WaterAdjust(*florhit, loz);
    }
}

void FAFgetzrangepoint(int32_t x, int32_t y, int32_t z, sectortype* const sect,
                       int32_t* hiz, Collision* ceilhit,
                       int32_t* loz, Collision* florhit)
{
    int foo1;
    Collision foo2;
    bool SkipFAFcheck;
    Collision trash; trash.invalidate();

    // IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // This will return invalid FAF ceiling and floor heights inside of analyzesprite
    // because the ceiling and floors get moved out of the way for drawing.

    // early out to regular routine
    if (!FAF_ConnectArea(sect))
    {
        getzrangepoint(x, y, z, sect, hiz,  ceilhit, loz,  florhit);
        SectorZadjust(*ceilhit, hiz, *florhit, loz);
        WaterAdjust(*florhit, loz);
        return;
    }

    getzrangepoint(x, y, z, sect, hiz,  ceilhit, loz,  florhit);
    SkipFAFcheck = SectorZadjust(*ceilhit, hiz, *florhit, loz);
    WaterAdjust(*florhit, loz);

    if (SkipFAFcheck)
        return;

    if (FAF_ConnectCeiling(sect))
    {
        auto uppersect = sect;
        int newz = *hiz - Z(2);
        if (ceilhit->type == kHitSprite)
            return;

        updatesectorz(x, y, newz, &uppersect);
        if (uppersect == nullptr)
            return;
        getzrangepoint(x, y, newz, uppersect, hiz,  ceilhit, &foo1,  &foo2);
        SectorZadjust(*ceilhit, hiz, trash, nullptr);
    }
    else if (FAF_ConnectFloor(sect) && !TEST(sect->floorstat, FLOOR_STAT_FAF_BLOCK_HITSCAN))
    {
        auto lowersect = sect;
        int newz = *loz + Z(2);
        if (florhit->type == kHitSprite)
            return;
        updatesectorz(x, y, newz, &lowersect);
        if (lowersect == nullptr)
            return;
        getzrangepoint(x, y, newz, lowersect, &foo1,  &foo2, loz,  florhit);
        SectorZadjust(trash, nullptr, *florhit, loz);
        WaterAdjust(*florhit, loz);
    }
}

void
SetupMirrorTiles(void)
{
    SPRITEp sp;

    SWStatIterator it(STAT_FAF);
    while (auto actor = it.Next())
    {
        sp = &actor->s();

        if (sp->sector()->ceilingpicnum == FAF_PLACE_MIRROR_PIC)
        {
            sp->sector()->ceilingpicnum = FAF_MIRROR_PIC;
            SET(sp->sector()->ceilingstat, CEILING_STAT_PLAX);
        }

        if (sp->sector()->floorpicnum == FAF_PLACE_MIRROR_PIC)
        {
            sp->sector()->floorpicnum = FAF_MIRROR_PIC;
            SET(sp->sector()->floorstat, FLOOR_STAT_PLAX);
        }

        if (sp->sector()->ceilingpicnum == FAF_PLACE_MIRROR_PIC+1)
            sp->sector()->ceilingpicnum = FAF_MIRROR_PIC+1;

        if (sp->sector()->floorpicnum == FAF_PLACE_MIRROR_PIC+1)
            sp->sector()->floorpicnum = FAF_MIRROR_PIC+1;
    }
}

void GetUpperLowerSector(short match, int x, int y, sectortype** upper, sectortype** lower)
{
    int i;
    sectortype* sectorlist[16];
    int sln = 0;
    SPRITEp sp;

    for(auto& sect : sectors())
    {
        if (inside(x, y, &sect) == 1)
        {
            bool found = false;

            SWSectIterator it(&sect);
            while (auto actor = it.Next())
            {
                sp = &actor->s();

                if (sp->statnum == STAT_FAF &&
                    (sp->hitag >= VIEW_LEVEL1 && sp->hitag <= VIEW_LEVEL6)
                    && sp->lotag == match)
                {
                    found = true;
                }
            }

            if (!found)
                continue;
            if (sln < (int)SIZ(sectorlist))
                sectorlist[sln] = &sect;
            sln++;
        }
    }

    // might not find ANYTHING if not tagged right
    if (sln == 0)
    {
        *upper = nullptr;
        *lower = nullptr;
        return;
    }
    // Map rooms have NOT been dragged on top of each other
    else if (sln == 1)
    {
        *lower = sectorlist[0];
        *upper = sectorlist[0];
        return;
    }
    // Map rooms HAVE been dragged on top of each other
    // inside will somtimes find that you are in two different sectors if the x,y
    // is exactly on a sector line.
    else if (sln > 2)
    {
        // try again moving the x,y pos around until you only get two sectors
        GetUpperLowerSector(match, x - 1, y, upper, lower);
    }

    if (sln == 2)
    {
        if (sectorlist[0]->floorz < sectorlist[1]->floorz)
        {
            // swap
            // make sectorlist[0] the LOW sector
            std::swap(sectorlist[0],sectorlist[1]);
        }

        *lower = sectorlist[0];
        *upper = sectorlist[1];
    }
}

bool FindCeilingView(int match, int* x, int* y, int z, sectortype** sect)
{
    int xoff = 0;
    int yoff = 0;
    SPRITEp sp = nullptr;
    int pix_diff;
    int newz;

    save.zcount = 0;

    // Search Stat List For closest ceiling view sprite
    // Get the match, xoff, yoff from this point
    SWStatIterator it(STAT_FAF);
    while (auto actor = it.Next())
    {
        sp = &actor->s();

        if (sp->hitag == VIEW_THRU_CEILING && sp->lotag == match)
        {
            xoff = *x - sp->x;
            yoff = *y - sp->y;
            break;
        }
    }

    it.Reset(STAT_FAF);
    while (auto actor = it.Next())
    {
        sp = &actor->s();

        if (sp->lotag == match)
        {
            // determine x,y position
            if (sp->hitag == VIEW_THRU_FLOOR)
            {
                sectortype* upper,* lower;

                *x = sp->x + xoff;
                *y = sp->y + yoff;

                // get new sector
                GetUpperLowerSector(match, *x, *y, &upper, &lower);
                *sect = upper;
                break;
            }
        }
    }

    if (*sect == nullptr)
        return false;

    if (!sp || sp->hitag != VIEW_THRU_FLOOR)
    {
        *sect = nullptr;
        return false;
    }


    if (!testnewrenderer)
    {
        pix_diff = labs(z - sp->sector()->floorz) >> 8;
        newz = sp->sector()->floorz + ((pix_diff / 128) + 1) * Z(128);

        it.Reset(STAT_FAF);
        while (auto actor = it.Next())
        {
            sp = &actor->s();

            if (sp->lotag == match)
            {
                // move lower levels ceilings up for the correct view
                if (sp->hitag == VIEW_LEVEL2)
                {
                    // save it off
                    save.sect[save.zcount] = sp->sector();
                    save.zval[save.zcount] = sp->sector()->floorz;
                    save.pic[save.zcount] = sp->sector()->floorpicnum;
                    save.slope[save.zcount] = sp->sector()->floorheinum;

                    sp->sector()->floorz = newz;
                    // don't change FAF_MIRROR_PIC - ConnectArea
                    if (sp->sector()->floorpicnum != FAF_MIRROR_PIC)
                        sp->sector()->floorpicnum = FAF_MIRROR_PIC + 1;
                    sp->sector()->floorheinum = 0;

                    save.zcount++;
                    PRODUCTION_ASSERT(save.zcount < ZMAX);
                }
            }
        }
    }

    return true;
}

bool FindFloorView(int match, int* x, int* y, int z, sectortype** sect)
{
    int xoff = 0;
    int yoff = 0;
    SPRITEp sp = nullptr;
    int newz;
    int pix_diff;

    save.zcount = 0;

    // Search Stat List For closest ceiling view sprite
    // Get the match, xoff, yoff from this point
    SWStatIterator it(STAT_FAF);
    while (auto actor = it.Next())
    {
        sp = &actor->s();

        if (sp->hitag == VIEW_THRU_FLOOR && sp->lotag == match)
        {
            xoff = *x - sp->x;
            yoff = *y - sp->y;
            break;
        }
    }


    it.Reset(STAT_FAF);
    while (auto actor = it.Next())
    {
        sp = &actor->s();

        if (sp->lotag == match)
        {
            // determine x,y position
            if (sp->hitag == VIEW_THRU_CEILING)
            {
                sectortype* upper,* lower;

                *x = sp->x + xoff;
                *y = sp->y + yoff;

                // get new sector
                GetUpperLowerSector(match, *x, *y, &upper, &lower);
                *sect = lower;
                break;
            }
        }
    }

    if (*sect == nullptr)
        return false;

    if (!sp || sp->hitag != VIEW_THRU_CEILING)
    {
        *sect = nullptr;
        return false;
    }

    if (!testnewrenderer)
    {
        // move ceiling multiple of 128 so that the wall tile will line up
        pix_diff = labs(z - sp->sector()->ceilingz) >> 8;
        newz = sp->sector()->ceilingz - ((pix_diff / 128) + 1) * Z(128);

        it.Reset(STAT_FAF);
        while (auto actor = it.Next())
        {
            sp = &actor->s();

            if (sp->lotag == match)
            {
                // move upper levels floors down for the correct view
                if (sp->hitag == VIEW_LEVEL1)
                {
                    // save it off
                    save.sect[save.zcount] = sp->sector();
                    save.zval[save.zcount] = sp->sector()->ceilingz;
                    save.pic[save.zcount] = sp->sector()->ceilingpicnum;
                    save.slope[save.zcount] = sp->sector()->ceilingheinum;

                    sp->sector()->ceilingz = newz;

                    // don't change FAF_MIRROR_PIC - ConnectArea
                    if (sp->sector()->ceilingpicnum != FAF_MIRROR_PIC)
                        sp->sector()->ceilingpicnum = FAF_MIRROR_PIC + 1;
                    sp->sector()->ceilingheinum = 0;

                    save.zcount++;
                    PRODUCTION_ASSERT(save.zcount < ZMAX);
                }
            }
        }
    }
    return true;
}

short FindViewSectorInScene(sectortype* cursect, short level)
{
    SPRITEp sp;
    short match;

    SWStatIterator it(STAT_FAF);
    while (auto actor = it.Next())
    {
        sp = &actor->s();

        if (sp->hitag == level)
        {
            if (cursect == sp->sector())
            {
                // ignore case if sprite is pointing up
                if (sp->ang == 1536)
                    continue;

                // only gets to here is sprite is pointing down

                // found a potential match
                match = sp->lotag;

                return match;
            }
        }
    }

    return -1;
}

struct PortalGroup
{
    TArray<int> sectors;
    int othersector = -1;
    vec3_t offset = { 0,0,0 };
};

    // This is very messy because some portals are linked outside the actual portal sectors, so we have to use the complicated original linking logic to find the connection. :?
void CollectPortals()
{
    int t = testnewrenderer;
    testnewrenderer = true;
    TArray<PortalGroup> floorportals;
    TArray<PortalGroup> ceilingportals;
    BitArray floordone(numsectors), ceilingdone(numsectors);

    for (int i = 0; i < numsectors; i++)
    {
        sector[i].portalflags = sector[i].portalnum = 0;
    }
    floordone.Zero();
    ceilingdone.Zero();
    portalClear();

    for (int i = 0; i < numsectors; i++)
    {
        if (sector[i].floorpicnum == FAF_MIRROR_PIC && !floordone[i])
        {
            auto& fp = floorportals[floorportals.Reserve(1)];
            fp.sectors.Push(i);
            floordone.Set(i);
            for (unsigned ii = 0; ii < fp.sectors.Size(); ii++)
            {
                for (auto& wal : wallsofsector(fp.sectors[ii]))
                {
                    if (!wal.twoSided()) continue;
                    auto nsec = wal.nextSector();
                    auto ns = sectnum(nsec);
                    if (floordone[ns] || nsec->floorpicnum != FAF_MIRROR_PIC) continue;
                    fp.sectors.Push(ns);
                    floordone.Set(ns);
                }
            }
        }
        if (sector[i].ceilingpicnum == FAF_MIRROR_PIC && !ceilingdone[i])
        {
            auto& fp = ceilingportals[ceilingportals.Reserve(1)];
            fp.sectors.Push(i);
            ceilingdone.Set(i);
            for (unsigned ii = 0; ii < fp.sectors.Size(); ii++)
            {
                for (auto& wal : wallsofsector(fp.sectors[ii]))
                {
                    if (!wal.twoSided()) continue;
                    auto nsec = wal.nextSector();
                    auto ns = sectnum(nsec);
                    if (ceilingdone[ns] || nsec->ceilingpicnum != FAF_MIRROR_PIC) continue;
                    fp.sectors.Push(ns);
                    ceilingdone.Set(ns);
                }
            }
        }
    }
    // now try to find connections.
    for (auto& fp : ceilingportals)
    {
        // pick one sprite out of the sectors, repeat until we get a valid connection
        for (auto sec : fp.sectors)
        {
            SWSectIterator it(sec);
            while (auto actor = it.Next())
            {
                int tx = actor->s().x;
                int ty = actor->s().y;
                int tz = actor->s().z;
                auto tsect = &sector[sec];

                int match = FindViewSectorInScene(tsect, VIEW_LEVEL1);
                if (match != -1)
                {
                    FindCeilingView(match, &tx, &ty, tz, &tsect);
                    if (tsect != nullptr &&tsect->floorpicnum == FAF_MIRROR_PIC)
                    {
                        // got something!
                        fp.othersector = sectnum(tsect);
                        fp.offset = { tx, ty, tz };
                        fp.offset -= actor->s().pos;
                        goto nextfg;
                    }
                }
            }
        }
    nextfg:;
    }

    for (auto& fp : floorportals)
    {
        for (auto sec : fp.sectors)
        {
            SWSectIterator it(sec);
            while (auto actor = it.Next())
            {
                int tx = actor->s().x;
                int ty = actor->s().y;
                int tz = actor->s().z;
                auto tsect = &sector[sec];

                int match = FindViewSectorInScene(tsect, VIEW_LEVEL2);
                if (match != -1)
                {
                    FindFloorView(match, &tx, &ty, tz, &tsect);
                    if (tsect != nullptr && tsect->ceilingpicnum == FAF_MIRROR_PIC)
                    {
                        // got something!
                        fp.othersector = sectnum(tsect);
                        fp.offset = { tx, ty, tz };
                        fp.offset -= actor->s().pos;
                        goto nextcg;
                    }
                }
            }
        }
    nextcg:;
    }
    for (auto& pt : floorportals)
    {
        if (pt.othersector > -1)
        {
            auto findother = [&](int other) -> PortalGroup*
            {
                for (auto& pt2 : ceilingportals)
                {
                    if (pt2.sectors.Find(other) != pt2.sectors.Size()) return &pt2;
                }
                return nullptr;
            };

            auto pt2 = findother(pt.othersector);
            if (pt2)
            {
                int pnum = portalAdd(PORTAL_SECTOR_FLOOR, -1, pt.offset.x, pt.offset.y, 0);
                allPortals[pnum].targets = pt2->sectors; // do not move! We still need the original.
                for (auto sec : pt.sectors)
                {
                    sector[sec].portalflags = PORTAL_SECTOR_FLOOR;
                    sector[sec].portalnum = pnum;
                }
            }
        }
    }
    for (auto& pt : ceilingportals)
    {
        if (pt.othersector > -1)
        {
            auto findother = [&](int other) -> PortalGroup*
            {
                for (auto& pt2 : floorportals)
                {
                    if (pt2.sectors.Find(other) != pt2.sectors.Size()) return &pt2;
                }
                return nullptr;
            };

            auto pt2 = findother(pt.othersector);
            if (pt2)
            {
                int pnum = portalAdd(PORTAL_SECTOR_CEILING, -1, pt.offset.x, pt.offset.y, 0);
                allPortals[pnum].targets = std::move(pt2->sectors);
                for (auto sec : pt.sectors)
                {
                    sector[sec].portalflags = PORTAL_SECTOR_CEILING;
                    sector[sec].portalnum = pnum;
                }
            }
        }
    }
    testnewrenderer = t;
}


END_SW_NS
