//CHARACTER SYMBOLS
#define PLAYER_SYMBOL 64	// @
#define KOBOLD 75		// K
#define ORC 79			// O
#define RAT 114			// r
#define FILLER 63		// ?

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
#define BOUNDS 32		// whitespace
#define FLOOR 46		// .
#define WALL 35			// #
#define DOOR 43			// +
#define STAIRCASE 62		// >

//MAP TILE INTS
enum tile {
	NBOUNDS,
	NFLOOR,
	NWALL,
	NDOOR,
	NSTAIRCASE,
	NMONSTER
};
