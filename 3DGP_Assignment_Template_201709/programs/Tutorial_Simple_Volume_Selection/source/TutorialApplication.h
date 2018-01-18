//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_
 
#include "BaseApplication.h"
#include "selection_rectangle.h"

using namespace Ogre;
class BasicTutorial_00 : public BaseApplication
{
private:
	PlaneBoundedVolumeListSceneQuery *mVolQuery;
	Real left, top, right, bottom;
	SelectionRectangle *mSelectionRect;
	SceneNode *mCurrentObject;
	SceneNode *mSceneNodeArr[128];
	Entity *mEntityArr[128];

virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
virtual bool frameStarted(const FrameEvent &evt);
public:
	BasicTutorial_00(void);
	virtual void createScene(void);
};
 
#endif // #ifndef __BasicTutorial_00_h_