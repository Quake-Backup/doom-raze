#include "ns.h"
#include "wh.h"

BEGIN_WH_NS

static void chasedemon(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();
	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0)
		spr.lotag = 250;

	if (plr.z < spr.z) {
		spr.z -= TICSPERFRAME << 8;
	}
	if (plr.z > spr.z) {
		spr.z += TICSPERFRAME << 8;
	}

	short osectnum = spr.sectnum;
	if (krand() % 63 == 0) {
		if (cansee(plr.x, plr.y, plr.z, plr.sector, spr.x, spr.y,
			spr.z - (tileHeight(spr.picnum) << 7), spr.sectnum) && plr.invisibletime < 0)
			SetNewStatus(actor, ATTACK);
		return;
	}
	else {
		if (PlayClock % 100 > 70)
			trailingsmoke(actor,true);

		int dax = (bcos(spr.ang) * TICSPERFRAME) << 2;
		int day = (bsin(spr.ang) * TICSPERFRAME) << 2;
		checksight(plr, actor);


		if (!checkdist(plr, actor)) {
			checkmove(actor, dax, day);
		}
		else {
			if (plr.invisibletime < 0) {
				if (krand() % 8 == 0) // NEW
					SetNewStatus(actor, ATTACK); // NEW
				else { // NEW
					spr.ang = (short)(((krand() & 512 - 256) + spr.ang + 1024) & 2047); // NEW
					SetNewStatus(actor, CHASE); // NEW
				}
			}
		}
	}

	getzrange(spr.x, spr.y, spr.z - 1, spr.sectnum, (spr.clipdist) << 2, CLIPMASK0);
	if ((spr.sectnum != osectnum) && (spr.sector()->lotag == 10))
		warpsprite(actor);

	checksector6(actor);

	processfluid(actor, zr_florHit, true);

	if (sector[osectnum].lotag == KILLSECTOR && spr.z + (8 << 8) >= sector[osectnum].floorz) {
		spr.hitag--;
		if (spr.hitag < 0)
			SetNewStatus(actor, DIE);
	}

	SetActorPos(actor, &spr.pos);

	if (zr_florHit.type == kHitSector && (spr.sector()->floorpicnum == LAVA
		|| spr.sector()->floorpicnum == LAVA1 || spr.sector()->floorpicnum == ANILAVA)) {
		if (spr.z + (8 << 8) >= sector[osectnum].floorz) {
			spr.hitag--;
			if (spr.hitag < 0)
				SetNewStatus(actor, DIE);
		}
	}
}

static void searchdemon(PLAYER& plr, DWHActor* actor)
{
	aisearch(plr, actor, true);
	checksector6(actor);
}

static void paindemon(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		spr.picnum = DEMON;
		spr.ang = plr.angle.ang.asbuild();
		SetNewStatus(actor, FLEE);
	}

	aifly(actor);
	SetActorPos(actor, &spr.pos);
}

static void facedemon(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	boolean cansee = ::cansee(plr.x, plr.y, plr.z, plr.sector, spr.x, spr.y, spr.z - (tileHeight(spr.picnum) << 7), spr.sectnum);

	if (cansee && plr.invisibletime < 0) {
		spr.ang = (short)(getangle(plr.x - spr.x, plr.y - spr.y) & 2047);
		if (plr.shadowtime > 0) {
			spr.ang = (short)(((krand() & 512 - 256) + spr.ang + 1024) & 2047);
			SetNewStatus(actor, FLEE);
		}
		else {
			actor->SetOwner(plr.actor());
			SetNewStatus(actor, CHASE);
		}
	}
	else { // get off the wall
		if (actor->GetOwner() == plr.actor()) {
			spr.ang = (short)(((krand() & 512 - 256) + spr.ang) & 2047);
			SetNewStatus(actor, FINDME);
		}
		else if (cansee) SetNewStatus(actor, FLEE);
	}

	if (checkdist(plr, actor))
		SetNewStatus(actor, ATTACK);
}


static void attackdemon(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	if (plr.z < spr.z) {
		spr.z -= TICSPERFRAME << 8;
	}
	if (plr.z > spr.z) {
		spr.z += TICSPERFRAME << 8;
	}

	spr.lotag -= TICSPERFRAME;
	if (spr.lotag < 0) {
		if (cansee(plr.x, plr.y, plr.z, plr.sector, spr.x, spr.y, spr.z - (tileHeight(spr.picnum) << 7),
			spr.sectnum))
			SetNewStatus(actor, CAST);
		else
			SetNewStatus(actor, CHASE);
	}
	else
		spr.ang = getangle(plr.x - spr.x, plr.y - spr.y);
}

static void fleedemon(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;
	short osectnum = spr.sectnum;

	auto moveStat = aifly(actor);

	if (moveStat.type != kHitNone) {
		if (moveStat.type == kHitWall) {
			int nWall = moveStat.index;
			int nx = -(wall[wall[nWall].point2].y - wall[nWall].y) >> 4;
			int ny = (wall[wall[nWall].point2].x - wall[nWall].x) >> 4;
			spr.ang = getangle(nx, ny);
		}
		else {
			spr.ang = getangle(plr.x - spr.x, plr.y - spr.y);
			SetNewStatus(actor, FACE);
		}
	}
	if (spr.lotag < 0)
		SetNewStatus(actor, FACE);

	if ((spr.sectnum != osectnum) && (spr.sector()->lotag == 10))
		warpsprite(actor);

	if (checksector6(actor))
		return;

	processfluid(actor, zr_florHit, true);

	SetActorPos(actor, &spr.pos);
}

static void castdemon(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	spr.lotag -= TICSPERFRAME;

	if (plr.z < spr.z) {
		spr.z -= TICSPERFRAME << 8;
	}
	if (plr.z > spr.z) {
		spr.z += TICSPERFRAME << 8;
	}

	if (spr.lotag < 0) {
		castspell(plr, actor);
		SetNewStatus(actor, CHASE);
	}
}

static void nukeddemon(PLAYER& plr, DWHActor* actor)
{
	SPRITE& spr = actor->s();

	chunksofmeat(plr, actor,spr.x, spr.y, spr.z, spr.sectnum, spr.ang);
	trailingsmoke(actor,false);
	SetNewStatus(actor, DIE);
}


void createDemonAI() {
	auto& e = enemy[DEMONTYPE];
	enemy[DEMONTYPE].info.Init(38, 41, 4096 + 2048, 120, 0, 64, true, 300, 0);

	e.chase = chasedemon;
	e.resurect;
	e.nuked = nukeddemon;
	e.frozen;
	e.pain = paindemon;
	e.face = facedemon;
	e.attack = attackdemon;
	e.flee = fleedemon;
	e.cast = castdemon;
	e.die;
	e.skirmish;
	e.stand;
	e.search = searchdemon;
}


void premapDemon(DWHActor* actor) {
	SPRITE& spr = actor->s();

	spr.detail = DEMONTYPE;
	ChangeActorStat(actor, FACE);
	enemy[DEMONTYPE].info.set(spr);
}

END_WH_NS
