#ifndef __GAME_ENTITY_H
#define __GAME_ENTITY_H

#include <dingus/utils/ringdeque.h>


class CGameEntity {
public:
	enum {
		HISTORY_SIZE = 4,
		LAG_BEHIND = 3,
		MAX_NAME_LEN = 8,
	};

	/// State info
	struct SState {
		short	posx, posy;		// entity position
		short	targx, targy;	// target position (if state requires one)
		short	stock;			// quantity of azn
		short	state;			// state
		short	health;			// hit points
		SVector3	pos;		// position in 3D
		char	name[MAX_NAME_LEN+1]; // friendly name
	};

public:
	CGameEntity( unsigned short eid, eEntityType type, int owner, int bornTurn );
	~CGameEntity();

	int		getLastUpdateTurn() const { return mLastUpdateTurn; }

	int		getID() const { return mID; }
	eEntityType	getType() const { return mType; }
	const std::string& getTypeName() const { return mTypeName; }
	int		getOwner() const { return mOwner; }
	int		getMaxHealth() const { return mMaxHealth; }

	const SState& getState() const { assert(!mStates.empty()); return mStates[LAG_BEHIND]; }
	const SState& getStateCurr() const { assert(!mStates.empty()); return mStates[LAG_BEHIND-1]; }

	bool	isAlive() const { assert(!mStates.empty()); return mStates[LAG_BEHIND].state != ENTSTATE_DEAD; }
	int		getDeathTurn() const { return mDeathTurn; }

	void	updateState( int turn, SState& state );
	void	markDead( int turn );

protected:
	void	adjustPosition( int turn, SState& s, bool height );

private:
	/// Last turn number that this entity was received
	int		mLastUpdateTurn;

	/// Unique entity's ID
	int		mID;
	/// Entity type.
	eEntityType	mType;
	/// Entity's type name
	std::string	mTypeName;
	/// Entity's owner: player index (0..count-2 are players; count-1 is AI).
	int		mOwner;

	/// Hit points when entity is born
	int		mMaxHealth;

	/// Turn the entity is born.
	int		mBornTurn;
	/// Turn the entity died (-1 if alive)
	int		mDeathTurn;

	/// Previous states history. Current is [0], previous is [1] etc.
	ringdeque<SState,HISTORY_SIZE>	mStates;

	// Related to final 3D positions
	bool	mOnGround;
	bool	mOnAir;
	bool	mOnSine;
	float	mBaseAltitude;
};



#endif
