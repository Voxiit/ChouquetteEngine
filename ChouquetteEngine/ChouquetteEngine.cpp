#include "ChouquetteEngine.h"

#pragma region Constructor and Destructor
//constructor
ChouquetteEngine::ChouquetteEngine() : ApplicationContext("Chouquette Engine")
{
}

//destructor
ChouquetteEngine::~ChouquetteEngine() {}
#pragma endregion



#pragma region Getters and Setters

#pragma endregion



#pragma region Main Functions
// setup
void ChouquetteEngine::setup(void)
{
    MainSetup();
    LightSetup();
    MeshSetups();
}
#pragma endregion

 
#pragma region Movement
// key_handler
bool ChouquetteEngine::keyPressed(const KeyboardEvent& evt)
{
    // Quit
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }

    // Move camera Position
    if (evt.keysym.sym == SDLK_LEFT)
    {
        Vector3 pos = camNode->getPosition();
        pos.x -= 3;
        camNode->setPosition(pos);
    }
    else if (evt.keysym.sym == SDLK_RIGHT)
    {
        Vector3 pos = camNode->getPosition();
        pos.x += 3;
        camNode->setPosition(pos);
    }

    if (evt.keysym.sym == SDLK_UP)
    {
        Vector3 pos = camNode->getPosition();
        pos.y += 3;
        camNode->setPosition(pos);
    }
    else if (evt.keysym.sym == SDLK_DOWN)
    {
        Vector3 pos = camNode->getPosition();
        pos.y -= 3;
        camNode->setPosition(pos);
    }



    // Move camera Rotation

    return true;
}


// mouse moved
bool ChouquetteEngine::mouseMoved(const MouseMotionEvent& evt)
{
    printf("Relx = %d\nRely = %d", evt.xrel, evt.yrel);
    
    camNode->yaw(Degree(evt.xrel) * -0.2f, Ogre::Node::TS_WORLD);
    camNode->pitch(Degree(evt.yrel) * -0.2f, Ogre::Node::TS_LOCAL);

    return true;
}
#pragma endregion




#pragma region Setup Functions
void ChouquetteEngine::MainSetup()
{
    ApplicationContext::setup();

    addInputListener(this);
    root = getRoot();
    scnMgr = root->createSceneManager();

    // Screen options
    rendSys = root->getRenderSystem();
    rendSys->setConfigOption("Full Screen", "Yes");
    rendSys->setConfigOption("Video Mode", "1920 x 1080 @ 32-bit colour");

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    
    //Camera
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 150, 500);
    camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
    cam = scnMgr->createCamera("MainCamera");
    cam->setNearClipDistance(5);
    camNode->attachObject(cam);

    //Viewport
    vp = getRenderWindow()->addViewport(cam);
    vp->setBackgroundColour(ColourValue(0, 0, 0));

    //Set camera ratio
    cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

    //CameraMan::mousePressed();
}


void ChouquetteEngine::LightSetup()
{
    // without light we would just get a black screen
    //turnlights
    
    //Create directional Light
    Light* directionalLight = scnMgr->createLight("DirectionalLight");
    directionalLight->setType(Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(0.6, 0.6, 0.6);
    directionalLight->setSpecularColour(0.6, 0.6, 0.6);
    sceneLights.push_back(directionalLight);

    //Setting directional light direction
    SceneNode* directionalLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    directionalLightNode->attachObject(directionalLight);
    directionalLightNode->setDirection(Vector3(0, -1, -1));



    //Set Ambient lights and shadow techniques
    scnMgr->setAmbientLight(ColourValue(0, 0, 0));
    scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
}


void ChouquetteEngine::MeshSetups()
{
    //Entity 1
    Entity* ogreEntity1 = scnMgr->createEntity("ogrehead.mesh");
    SceneNode* ogreNode1 = scnMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode1->attachObject(ogreEntity1);
    sceneMeshes.push_back(ogreEntity1);


    //Plane 
    Plane plane(Vector3::UNIT_Y, 0);
    //Create plane's mesh
    MeshManager::getSingleton().createPlane(
        "ground", RGN_DEFAULT,
        plane,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);
    //Create plane entity
    Entity* groundEntity = scnMgr->createEntity("ground");
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    groundEntity->setCastShadows(false);
    groundEntity->setMaterialName("Examples/Rockwall");
    sceneMeshes.push_back(groundEntity);
}

#pragma endregion
