#include <vector>
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;
#pragma once
class ChouquetteEngine : public ApplicationContext, public InputListener
{
public:
    // Constructors and destructors
    ChouquetteEngine();
    virtual ~ChouquetteEngine();

    //Main functions
    void setup();
    bool keyPressed(const KeyboardEvent& evt);
    bool mouseMoved(const MouseMotionEvent& evt);


    
private:
    //------------------------
    // Attributes
    //------------------------
    // Main attributes
    Root* root;
    SceneManager* scnMgr;
    RenderSystem* rendSys;

    // Camera attributes
    SceneNode* camNode;
    Camera* cam;

    // Viewport
    Viewport* vp;

    // Lights
    std::vector<Light*> sceneLights;

    // Meshes
    std::vector<Entity*> sceneMeshes;
    

    //------------------------
    // Functions
    //------------------------
    // Setup functions
    void MainSetup();
    void LightSetup();
    void MeshSetups();
};

