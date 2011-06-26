/*
This file is part of "TW-Light"
					http://tw-light.berlios.de/
Copyright (C) 2001-2004  TimeWarp development team

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifndef TW_GOB_H_INCLUDES
#define TW_GOB_H_INCLUDES

#include "melee.h"
#include "melee/mframe.h"
#include "melee/mgame.h"
#include "melee/mitems.h"

class Upgrade;
class GobStation;

/// Player possession (ship, money, upgrades)
class GobPlayer
{
	public:
		int channel;
		~GobPlayer();
		Ship *ship;
		Control *control;
		ShipPanel *panel;
		struct pair
		{
			char *id;
			int value;
		};
		pair *pair_list;
		int num_pairs;
		void _add_pair(const char *id, int value);
		pair *_get_pair(const char *id);
		void write_pair(const char *id, int value);
		int read_pair(const char *id);
		int total;				 ///<total upgrades purchased, used in calculating price of future upgrades
		int starbucks;
		int buckazoids;
		int kills;
		int value_starbucks;
		int value_buckazoids;
		TeamCode team;
		void init(Control *c, TeamCode team);
		void died(SpaceLocation *killer);
		void new_ship(ShipType *type);
		int charge (char *name, int price_starbucks, int price_buckazoids) ;
		Upgrade **upgrade_list;
} ;

/// Enemy starship, reward for it
class GobEnemy
{
	public:
		Ship *ship;
		int starbucks;
		int buckazoids;
		void init(Ship *ship, int kill_starbucks, int kill_buckazoids);
		void died (SpaceLocation *what);
} ;

/// Asteroid that add money to it's killer
class GobAsteroid : public Asteroid
{
	public:
		virtual int handle_damage (SpaceLocation *source, double normal, double direct);
		virtual void death();
};

/// Full future adventure game
class GobGame : public Game
{
	public:
		virtual ~GobGame();

		TeamCode enemy_team;

		virtual void calculate();
		virtual void ship_died(Ship *who, SpaceLocation *source);
		virtual void preinit();
		virtual void init (Log *log);

		virtual void play_sound (SAMPLE *sample, SpaceLocation *source, int vol = 256, int freq = 1000);

		int gobplayers;
		GobPlayer **gobplayer;
		virtual void add_gobplayer(Control *control);
		virtual GobPlayer *get_player(SpaceLocation *what);
		int gobenemies, max_enemies;
		GobEnemy **gobenemy;
		virtual int get_enemy_index(SpaceLocation *what);

		//	protected:
		virtual void fps ();

		void add_new_enemy();

		int next_add_new_enemy_time;

		SpaceSprite *stationSprite[3];
		char *station_pic_name[3];
		char *station_build_name[3];
		SpaceSprite *defenderSprite;

	public:
		int num_planets;
		Planet *planet[16];
		GobStation *station[16];
		void add_planet_and_station ( SpaceSprite *planet_sprite, int planet_index, SpaceSprite *station_sprite, const char *builds, const char *background);
};

/// Station where player can buy upgrades and repair his ship
class GobStation : public Orbiter
{
	public:
		const char *build_type;
		const char *background_pic;
		GobStation ( SpaceSprite *pic, SpaceLocation *orbit_me, const char *ship, const char *background);
		virtual void buy_new_ship_menu(GobPlayer *s) ;
		virtual void inflict_damage(SpaceObject *other);
		virtual void station_screen (GobPlayer *s);
		virtual void upgrade_menu(GobStation *station, GobPlayer *gs) ;
};

/// Custom upgrade to ship
class Upgrade
{
	public:
		enum {
			active, inactive
		};
		char *name;
		int starbucks;
		int buckazoids;
		int status;
		int num;
		int index;
		virtual bool update(Ship *ship, GobStation *station, GobPlayer *gp) = 0;
		//true if listed
		virtual void execute(Ship *ship, GobStation *station, GobPlayer *gp) = 0;
		virtual void charge(GobPlayer *gp);
		virtual void clear(Ship *oship, Ship *nship, GobPlayer *gp);
		virtual Upgrade *duplicate() = 0;
};

/// Strange space location
class RainbowRift : public SpaceLocation
{
	public:
		enum { n = 2 };
		float p[n * 6 + 2];
		RGB c[n];
		int next_time, next_time2;
		RainbowRift ();
		virtual void animate ( Frame *frame );
		virtual void calculate () ;
		void squiggle();
};
#endif							 // TW_GOB_H_INCLUDES