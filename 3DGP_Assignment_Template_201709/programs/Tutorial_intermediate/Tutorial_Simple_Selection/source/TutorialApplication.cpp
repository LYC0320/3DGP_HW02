//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#include "TutorialApplication.h"

using namespace Ogre;
BasicTutorial_00::BasicTutorial_00(void) {
	mCurrentObject = 0;
}

void BasicTutorial_00::createScene(void) 
{
	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	Light *light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(0, 150, 250)); 
	light->setDiffuseColour(1.0, 1.0, 1.0);		//red
	light->setSpecularColour(1.0, 1.0, 1.0);	//red


	int i;
	i = 0;
	mEntityArr[i] 
	= mSceneMgr
		->createEntity( "Robot0", "robot.mesh" ); 

	mSceneNodeArr[i]  
	= mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		"RobotNode0", Vector3( 50, 0, 0 ) ); 

	mSceneNodeArr[i]->attachObject( mEntityArr[i] );
	//
	i = 1;
	mEntityArr[i] 
	= mSceneMgr
		->createEntity( "Robot3", "robot.mesh" ); 

	mSceneNodeArr[i]  
	= mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		"RobotNode3", Vector3( 50, 0, 0 ) ); 

	mSceneNodeArr[i]->attachObject( mEntityArr[i] );
	//

	i = 2;
	mEntityArr[i] 
	= mSceneMgr
		->createEntity( "Robot1", "sphere.mesh" ); 

	mSceneNodeArr[i]  
	= mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		"RobotNode1", Vector3( 150, 0, 150 ) ); 

	mSceneNodeArr[i]->scale (0.3, 0.3, 0.3);
	mSceneNodeArr[i]->attachObject( mEntityArr[i] );

	//
	//mCurrentObject = mSceneNodeArr[i];

	//
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 		// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

}

bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Ray mRay =mTrayMgr->getCursorRay(mCamera);

	RaySceneQuery *mRaySceneQuery = mSceneMgr->createRayQuery ( Ray() ); 

	mRaySceneQuery->setSortByDistance(true); 

	mRaySceneQuery->setRay(mRay);
	// Perform the scene query
	RaySceneQueryResult &result = mRaySceneQuery->execute();
	RaySceneQueryResult::iterator itr = result.begin();

	// Get the results, set the camera height
	// We are interested in the first intersection. It is ok to traverse all the results.
	for (itr = result.begin(); itr != result.end(); itr++)
	{

		if (itr->movable && itr->movable->getName().substr(0, 5) != "tile[")
		{
			mCurrentObject = itr->movable->getParentSceneNode();
			bool flgShow = mCurrentObject->getShowBoundingBox();
			mCurrentObject->showBoundingBox(!flgShow);
			break;
		} // if
		else if (itr->worldFragment) {
			//
		}
	}


	return BaseApplication::mousePressed( arg, id );
	
}

bool BasicTutorial_00::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mCurrentObject!=0) {
		Vector3 velocity(1, 0, 1);
		Real speedFactor = 20;
		Real t = evt.timeSinceLastFrame;
		mCurrentObject->translate(t*speedFactor*velocity);
	}
	return BaseApplication::frameRenderingQueued(evt);
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
