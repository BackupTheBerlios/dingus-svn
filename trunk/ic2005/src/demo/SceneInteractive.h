#ifndef __SCENE_INTERACTIVE_H
#define __SCENE_INTERACTIVE_H

#include "Scene.h"


class CControllableCharacter;
class CThirdPersonCameraController;
class CSceneSharedStuff;


// --------------------------------------------------------------------------

class CSceneInteractive : public CScene {
public:
	CSceneInteractive( CSceneSharedStuff* sharedStuff );
	~CSceneInteractive();

	virtual void	update( time_value demoTime, float dt );
	virtual void	render( eRenderMode renderMode );
	virtual const SMatrix4x4* getLightTargetMatrix() const;

	void	processInput( float mov, float rot, bool attack, time_value demoTime );

private:
	CSceneSharedStuff*	mSharedStuff;
	
	CControllableCharacter*			mCharacter;
	int								mSpineBoneIndex;
	CThirdPersonCameraController*	mCamController;
	
	// outside room
	std::vector<CRoomObjectEntity*>	mRoom;
};


#endif
