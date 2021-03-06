//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_
 
#include "BaseApplication.h"

using namespace Ogre;
class BasicTutorial_00 : public BaseApplication
{
private:
	SceneNode *mCurrentObject;
	SceneNode *mSceneNodeArr[8];
	Entity *mEntityArr[8];
virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

public:
	BasicTutorial_00(void);
	virtual void createScene(void);
};
 
#endif // #ifndef __BasicTutorial_00_h_