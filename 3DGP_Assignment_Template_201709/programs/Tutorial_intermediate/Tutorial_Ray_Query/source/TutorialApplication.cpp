//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// //mTrayMgr->hideCursor();
//
#include "TutorialApplication.h"

using namespace Ogre;
BasicTutorial_00::BasicTutorial_00(void) {}

void BasicTutorial_00::chooseSceneManager(void)
{
	mSceneMgr = mRoot
		->createSceneManager(ST_EXTERIOR_CLOSE); 
}

bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Ray mRay =mTrayMgr->getCursorRay(mCamera);

	RaySceneQuery *mRaySceneQuery = 
		mSceneMgr->createRayQuery ( Ray() ); 

	mRaySceneQuery->setRay(mRay);
	// Perform the scene query
	RaySceneQueryResult &result = 
		mRaySceneQuery->execute();
	RaySceneQueryResult::iterator itr = result.begin();

	// Get the results, set the camera height
	// We are interested in the first intersection. It is ok to traverse all the results.
	if (itr != result.end() && itr->worldFragment)
	{
		mSceneNode->setPosition(itr->worldFragment->singleIntersection);
		
	}

	return true;
	//return BaseApplication::mousePressed( arg, id );
}
void BasicTutorial_00::createScene(void) 
{
	mSceneMgr->setAmbientLight(ColourValue(1.0, 1.0, 1.0));
	mSceneMgr->setWorldGeometry("terrain.cfg");  

	mEntity 
		= mSceneMgr
		->createEntity( "Robot", "robot.mesh" ); 

	mSceneNode 
		= mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		"RobotNode", Vector3( 50, 0, 0 ) ); 

	mSceneNode->attachObject( mEntity ); 
}


int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
