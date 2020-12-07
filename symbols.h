//CONSTRAINTS
#define MAXMONSTERS 200 

//CHARACTER SYMBOLS
#define PLAYER_SYMBOL '@' 
#define KOBOLD 'K'
#define ORC 'O'
#define RAT 'r'
#define FILLER '?'

enum monsterType {
    NKOBOLD,
    NORC,
    NRAT,
    NFILLER
};

//MAP SYMBOLS
#define FLOOR '.'
#define WALL '#'
#define DOOR '+'
#define STAIRCASE '>'

//MAP TILE INTS
enum tile {
    NFLOOR,
    NWALL,
    NDOOR,
    NSTAIRCASE,
    NMONSTER
};
