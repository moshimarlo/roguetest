//CHARACTER SYMBOLS
#define PLAYER_SYMBOL '@'
#define KOBOLD 'K'	
#define ORC 'O'	
#define RAT 'r'
#define FILLER '?'

#define SKOBOLD "kobold"
#define SORC "orc"
#define SRAT "rat"

enum monsterType {
	NKOBOLD,
	NORC,
	NRAT,
	NFILLER
};

//MAP SYMBOLS
#define BOUNDS ' '
#define FLOOR '.'
#define WALL '#'
#define DOOR '+'
#define STAIRCASE '>'

//OBJECT SYMBOLS
#define GOLD '$'

//MAP TILE INTS
enum tile {
	NBOUNDS,
	NFLOOR,
	NWALL,
	NDOOR,
	NSTAIRCASE,
	NMONSTER
};
