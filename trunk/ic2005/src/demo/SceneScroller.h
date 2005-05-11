#ifndef __SCENE_SCROLLER_H
#define __SCENE_SCROLLER_H

#include "Scene.h"


// --------------------------------------------------------------------------

class CSceneScroller : public CScene {
public:
	CSceneScroller();
	~CSceneScroller();

	virtual void	update( time_value demoTime, float dt );
	virtual void	render( eRenderMode renderMode );
	virtual const SMatrix4x4* getLightTargetMatrix() const;
	
	void	start( time_value demoTime );
	bool	isEnded() const { return mPlayedTime >= SCROLLER_DURATION; }

private:
	static const float SCROLLER_DURATION;

	// timing
	time_value	mStartTime;
	time_value	mLocalTime; // time since started
	float		mPlayedTime;
	float		mByeAnimDuration;

	// anims
	std::vector<CAnimationBunch*>	mAnims;
	std::vector<int>				mAnimPlayCount;
	CAnimationBunch*				mByeAnim;

	// character
	CComplexStuffEntity*	mCharacter;
	int			mSpineBoneIndex;
};



#endif