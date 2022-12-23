// sector effector lotags. Preferably these should be eliminated once SE's get converted to classes. Until then we still need these.
enum EEffectorTypes
{
	SE_0_ROTATING_SECTOR              = 0,
	SE_1_PIVOT                        = 1,
	SE_2_EARTHQUAKE                   = 2,
	SE_3_RANDOM_LIGHTS_AFTER_SHOT_OUT = 3,
	SE_4_RANDOM_LIGHTS                = 4,
	SE_5_BOSS                         = 5,
	SE_6_SUBWAY                       = 6,

	SE_7_TELEPORT                      = 7,
	SE_8_UP_OPEN_DOOR_LIGHTS           = 8,
	SE_9_DOWN_OPEN_DOOR_LIGHTS         = 9,
	SE_10_DOOR_AUTO_CLOSE              = 10,
	SE_11_SWINGING_DOOR                = 11,
	SE_12_LIGHT_SWITCH                 = 12,
	SE_13_EXPLOSIVE                    = 13,
	SE_14_SUBWAY_CAR                   = 14,
	SE_15_SLIDING_DOOR                 = 15,
	SE_16_REACTOR                      = 16,
	SE_17_WARP_ELEVATOR                = 17,
	SE_18_INCREMENTAL_SECTOR_RISE_FALL = 18,
	SE_19_EXPLOSION_LOWERS_CEILING     = 19,
	SE_20_STRETCH_BRIDGE               = 20,
	SE_21_DROP_FLOOR                   = 21,
	SE_22_TEETH_DOOR                   = 22,
	SE_23_ONE_WAY_TELEPORT             = 23,
	SE_24_CONVEYOR                     = 24,
	SE_25_PISTON                       = 25,
	SE_26                              = 26,
	SE_27_DEMO_CAM                     = 27,
	SE_28_LIGHTNING                    = 28,
	SE_29_WAVES                        = 29,
	SE_30_TWO_WAY_TRAIN                = 30,
	SE_31_FLOOR_RISE_FALL              = 31,
	SE_32_CEILING_RISE_FALL            = 32,
	SE_33_QUAKE_DEBRIS                 = 33,
	SE_34                              = 34,
	SE_35                              = 35,
	SE_36_PROJ_SHOOTER                 = 36,
	SE_47_LIGHT_SWITCH                 = 47,
	SE_48_LIGHT_SWITCH                 = 48,
	SE_49_POINT_LIGHT                  = 49,
	SE_50_SPOT_LIGHT                   = 50,
	SE_128_GLASS_BREAKING              = 128,
	SE_130                             = 130,
	SE_131                             = 131,
	SE_156_CONVEYOR_NOSCROLL           = 156,
};

enum ESectorTriggers
{
	ST_0_NO_EFFECT   = 0,
	ST_1_ABOVE_WATER = 1,
	ST_2_UNDERWATER  = 2,
	ST_3             = 3,
	// ^^^ maybe not complete substitution in code
	ST_9_SLIDING_ST_DOOR     = 9,
	ST_15_WARP_ELEVATOR      = 15,
	ST_16_PLATFORM_DOWN      = 16,
	ST_17_PLATFORM_UP        = 17,
	ST_18_ELEVATOR_DOWN      = 18,
	ST_19_ELEVATOR_UP        = 19,
	ST_20_CEILING_DOOR       = 20,
	ST_21_FLOOR_DOOR         = 21,
	ST_22_SPLITTING_DOOR     = 22,
	ST_23_SWINGING_DOOR      = 23,
	ST_25_SLIDING_DOOR       = 25,
	ST_26_SPLITTING_ST_DOOR  = 26,
	ST_27_STRETCH_BRIDGE     = 27,
	ST_28_DROP_FLOOR         = 28,
	ST_29_TEETH_DOOR         = 29,
	ST_30_ROTATE_RISE_BRIDGE = 30,
	ST_31_TWO_WAY_TRAIN      = 31,

	ST_41_JAILDOOR			= 41,
	ST_42_MINECART			= 42,

	ST_160_FLOOR_TELEPORT	= 160,
	ST_161_CEILING_TELEPORT = 161,

	// left: ST 32767, 65534, 65535
};

class DukeActor : CoreActor native
{
	default
	{
		lookallarounddefault;
		falladjustz 24;
		autoaimangle 8.4375;
	}
	enum EStatnums
	{
		STAT_DEFAULT        = 0,
		STAT_ACTOR          = 1,
		STAT_ZOMBIEACTOR    = 2,
		STAT_EFFECTOR       = 3,
		STAT_PROJECTILE     = 4,
		STAT_MISC           = 5,
		STAT_STANDABLE      = 6,
		STAT_LOCATOR        = 7,
		STAT_ACTIVATOR      = 8,
		STAT_TRANSPORT      = 9,
		STAT_PLAYER         = 10,
		STAT_FX             = 11,
		STAT_FALLER         = 12,
		STAT_DUMMYPLAYER    = 13,
		STAT_LIGHT          = 14,
		STAT_RAROR          = 15,

		STAT_DESTRUCT		= 100,
		STAT_BOWLING		= 105,
		STAT_CHICKENPLANT	= 106,
		STAT_LUMBERMILL		= 107,
		STAT_TELEPORT		= 108,
		STAT_BOBBING		= 118,
		STAT_RABBITSPAWN	= 119,

		STAT_REMOVED		= MAXSTATUS-2,

	};

	enum amoveflags_t
	{
		face_player       = 1,
		geth              = 2,
		getv              = 4,
		random_angle      = 8,
		face_player_slow  = 16,
		spin              = 32,
		face_player_smart = 64,
		fleeenemy         = 128,
		jumptoplayer_only = 256,
		justjump1 = 256,
		jumptoplayer      = 257,
		seekplayer        = 512,
		furthestdir       = 1024,
		dodgebullet       = 4096,
		justjump2         = 8192,
		windang           = 16384,
		antifaceplayerslow = 32768
	};

	meta int gutsoffset;
	meta int falladjustz;
	meta int aimoffset;
	meta int strength;
	meta double autoaimangle;
	meta double sparkoffset;

	property prefix: none;
	property gutsoffset: gutsoffset;
	property falladjustz: falladjustz;
	property aimoffset: aimoffset;
	property strength: strength;
	property autoaimangle: autoaimangle;
	property sparkoffset: sparkoffset;

	
	native void SetSpritesetImage(int index);
	native int GetSpritesetSize();

	native class<DukeActor> attackertype;
	native DukeActor ownerActor, hitOwnerActor;
	native uint8 cgg;
	native uint8 spriteextra;	// moved here for easier maintenance. This was originally a hacked in field in the sprite structure called 'filler'.
	native int16 hitextra, movflag;
	native int16 tempval; /*, dispicnum;*/
	native int16 timetosleep;
	native bool mapSpawned;
	native double floorz, ceilingz, hitang;
	native int saved_ammo;
	native int palvals;
	native int counter;
	native int temp_data[5];
	native private int flags1, flags2, flags3;
	native walltype temp_walls[2];
	native sectortype temp_sect, actorstayput;

	native DukeActor temp_actor, seek_actor;
	native Vector3 temp_pos, temp_pos2;
	native double temp_angle;

	// flags are implemented natively to avoid the prefixes.
	
	native void getglobalz();
	native DukePlayer, double findplayer();
	native DukePlayer GetPlayer();
	native int ifhitbyweapon();
	native int domove(int clipmask);
	native int PlayActorSound(Sound snd, int chan = CHAN_AUTO, int flags = 0);
	native int CheckSoundPlaying(Sound snd, int chan = CHAN_AUTO);
	native void StopSound(Sound snd, int flags = 0);
	native DukeActor spawn(class<DukeActor> type);
	native DukeActor spawnsprite(int type);	// for cases where the map has a picnum stored. Avoid when possible.
	native DukeActor spawnweaponorammo(int type);
	native void lotsofglass(int count, walltype wal = null);
	native void lotsofcolourglass(int count, walltype wal = null);
	native void makeitfall();
	native void detonate(class<DukeActor> type);
	native void checkhitdefault(DukeActor proj);
	native void operatesectors(sectortype sec);
	native int SpriteWidth();
	native int SpriteHeight();
	native DukeActor aim(readonly<DukeActor> weapon);

	virtual native void Tick();

	
	virtual void BeginPlay() {}
	virtual void StaticSetup() {}
	virtual void onHit(DukeActor hitter) { checkhitdefault(hitter); }
	virtual void onHurt(DukePlayer p) {}
	virtual bool onUse(DukePlayer user) { return false; }
	virtual void onTouch(DukePlayer toucher) {}
	virtual void onMotoSmash(DukePlayer toucher) {}
	virtual void onRespawn(int tag) { }
	virtual bool animate(tspritetype tspr) { return false; }
	virtual void RunState() {}	// this is the CON function.
	virtual void PlayFTASound() {}
	virtual void StandingOn(DukePlayer p) {}
	virtual bool TriggerSwitch(DukePlayer activator) { return false; }
	virtual bool shootthis(DukeActor actor, DukePlayer p, Vector3 pos, double ang) const // this gets called on the defaults.
	{
		return false;
	}
	virtual class<DukeActor> GetRadiusDamageType(int targhealth)
	{
		return 'DukeRadiusExplosion';
	}
	
	native void RandomScrap();
	native void hitradius(int r, int hp1, int hp2, int hp3, int hp4);
	native double, DukeActor hitasprite();
	native int badguy();
	native int scripted();
	native int isplayer();
	native void lotsofstuff(class<DukeActor> type, int count);
	native int movesprite(Vector3 move, int clipmask);
	native int movesprite_ex(Vector3 move, int clipmask, CollisionData coll);
	native void shoot(class<DukeActor> spawnclass);
	native void setClipDistFromTile();
	native void insertspriteq();
	native void operateforcefields(int tag);
	native void restoreloc();
	native void addkill();
	native void subkill();
	

	
	
	virtual void Initialize()
	{
		if (!self.badguy() && self.scripted())
		{
			if (!self.mapSpawned) self.lotag = 0;

			if (self.scale.X == 0 || self.scale.Y == 0)
			{
				self.scale = (REPEAT_SCALE, REPEAT_SCALE);
			}

			self.clipdist = 10;
			self.ownerActor = self;
			self.ChangeStat(STAT_ACTOR);
		}
	}
	
	
	void commonItemSetup(Vector2 scale = (0.5, 0.5), int usefloorshade = -1, bool noinitialmove = false)
	{
		let owner = self.ownerActor;
		if (owner != self)
		{
			self.lotag = 0;
			if (!noinitialmove)
			{
				self.pos.Z -= 32;
				self.vel.Z = -4;
			}
			else
				self.vel.Z = 0;
			self.DoMove(CLIPMASK0);
			self.cstat |= randomxflip();
		}
		else
		{
			self.cstat = 0;
		}

		if (ud.multimode < 2 && self.pal != 0)
		{
			self.scale = (0, 0);
			self.ChangeStat(STAT_MISC);
			return;
		}

		self.pal = 0;
		self.shade = -17;
		self.scale = scale;

		if (owner != self) self.ChangeStat(STAT_ACTOR);
		else
		{
			self.ChangeStat(STAT_ZOMBIEACTOR);
			self.makeitfall();
		}

		// RR defaults to using the floor shade here, let's make this configurable.
		if (usefloorshade == 1 || (usefloorshade == -1 && Raze.isRR()))
		{
			self.shade = self.sector.floorshade;
		}
		
	}

	
	int checkLocationForFloorSprite(double radius)
	{
		bool away = self.isAwayFromWall(radius);
		
		if (!away)
		{
			self.scale = (0, 0); 
			self.ChangeStat(STAT_MISC); 
			return false;
		}

		if (self.sector.lotag == ST_1_ABOVE_WATER)
		{
			self.scale = (0, 0); 
			self.ChangeStat(STAT_MISC);
			return false;
		}
		return true;
	}
	
}

extend struct _
{
	native @DukeGameInfo gs;
	native @DukeUserDefs ud;
	native DukeLevel dlevel;
	native DukeActor camsprite;
}

// The level struct is a wrapper to group all level related global variables and static functions into one object.
// On the script side we do not really want scattered global data that is publicly accessible.
struct DukeLevel
{
	enum animtype_t
	{
		anim_floorz,
		anim_ceilingz,
		anim_vertexx,
		anim_vertexy,
	};

	native DukeActor SpawnActor(sectortype sect, Vector3 pos, class<DukeActor> type, int shade, Vector2 scale, double angle, double vel, double zvel, DukeActor owner, int stat = -1);
	native static int check_activator_motion(int lotag);
	native static void operatemasterswitches(int lotag);
	native static void operateactivators(int lotag, DukePlayer p);
	native static int floorsurface(sectortype s);
	native static int floorflags(sectortype s);
	native static int ceilingsurface(sectortype s);
	native static int ceilingflags(sectortype s);
	native static int wallflags(walltype s, int which);
	native static void AddCycler(sectortype sector, int lotag, int shade, int shade2, int hitag, int state);
	native static void addtorch(sectortype sector, int shade, int lotag);
	native static void addlightning(sectortype sector, int shade);
	native static int addambient(int hitag, int lotag);
	native static void resetswitch(int tag);	// 
	native static bool isMirror(walltype wal);
	native static void checkhitwall(walltype wal, DukeActor hitter, Vector3 hitpos);
	native static void checkhitceiling(sectortype wal, DukeActor hitter);
	native static DukeActor LocateTheLocator(int n, sectortype sect);
	native static int getanimationindex(int type, sectortype sec);
	native static int setanimation(sectortype animsect, int type, sectortype sec, double target, double vel);
}

struct DukeStatIterator
{
	private DukeActor nextp;
	native DukeActor Next();
	native DukeActor First(int stat);
}

struct DukeSectIterator
{
	private DukeActor nextp;
	native DukeActor Next();
	native DukeActor First(sectortype sect);
}

struct DukeSpriteIterator
{
	private DukeActor nextp;
	private int stat;
	native DukeActor Next();
	native DukeActor First();
}


