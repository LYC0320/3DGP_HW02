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
	SceneNode *mSceneNode;
	Entity *mEntity;
	int mCounterOfObjs;
	SceneNode *mSceneNodeArr[128];
	Entity *mEntityArr[128];

	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
public:
	BasicTutorial_00(void);
	virtual void chooseSceneManager(void);
	virtual void createScene(void);
};
 
#endif // #ifndef __BasicTutorial_00_h_