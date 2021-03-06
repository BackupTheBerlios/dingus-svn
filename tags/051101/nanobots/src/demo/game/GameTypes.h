#ifndef __GAME_TYPES_H
#define __GAME_TYPES_H

enum {
	G_MAX_PLAYERS = 3,	///< 2 players and computer. Indices are: 0=computer, 1&2=players
}; 


/// Game server states.
enum eGameServerState {
	GST_NONE = 0,	///< None: server not started yet
	GST_WAITING,	///< Waiting for players
	GST_STARTING,	///< Starting (countdown)
	GST_STARTED,	///< Game itself
	GST_ENDED,		///< Game ended
	GST_READYTOSTART, ///< Ready to start
	GSTCOUNT
};

/// Mission states
enum eMissionState {
	MST_TOBEDONE = 0,
	MST_FAILED = 1,
	MST_DONE = 2,
	MSTCOUNT
};

/// Entity types.
enum eEntityType {
	ENTITY_NEEDLE = 0,
	ENTITY_EXPLORER,
	ENTITY_COLLECTOR,
	ENTITY_AI,
	ENTITY_CONTAINER,
	ENTITY_NEUROC,
	ENTITY_BLOCKER,
	ENTITYCOUNT
};

/// Entity states.
enum eEntityState {
	ENTSTATE_IDLE = 0,
	ENTSTATE_MOVE,
	ENTSTATE_ATTACK,
	ENTSTATE_COLLECT,
	ENTSTATE_TRANSFER,
	ENTSTATE_BUILD,
	ENTSTATE_DEAD, ///< Used only internally by the viewer
	ENTSTATECOUNT
};

/// Cell types
enum eCellType {
	CELL_BLOOD1 = 0,	///< Low density blood
	CELL_BLOOD2,		///< Medium density blood
	CELL_BLOOD3,		///< High density blood
	CELL_BONE,			///< Bone
	CELLCOUNT
};

// Cell color types
enum eCellColor {
	CCOLOR_BLOOD = 0,
	CCOLOR_BONE,
	CCOLOR_NEURON,
	CCOLORCOUNT
};

/// Special point types
enum ePointType {
	PT_AZN = 0,
	PT_HOSHIMI,
	PT_INJECTION,	// Is not in the maps
	PT_OBJECTIVE,	// Part of mission
	PT_DECORATIVE,	// Decorative element
	PTCOUNT
};


#endif
