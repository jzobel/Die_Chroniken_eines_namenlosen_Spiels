#include "StateStartup.h"
#include "Configuration.h"
#include "Constants.h"
#include "Logfile.h"
//#include "Camera.h"
//#include "Collision.h"
//#include "Eventreceiver.h"
//#include "GenericHelperMethods.h"
//#include "Ground.h"
//#include "Hero.h"
//#include "Mauspfeil.h"
//#include "ObjectManager.h"
//#include "Scripting.h"
//#include "TimerManager.h"
//#include "Ton.h"
//#include "Weather.h"
//#include "World.h"
#include "Zufall.h"
#ifdef _DEBUG_MODE
//#include "DebugShapesManager.h"
//#include "Debugwindow.h"
#endif



StateStartup::StateStartup( IrrlichtDevice* device)
: GameState(),
  device_(device),
  classCounter_(0)
{
    if ( device_ == 0 )
        Logfile::getInstance().emergencyExit(
                "Entchen in [StateStartup] nicht mehr gefunden! Abbruch." );
    device_->setWindowCaption( L"Loading..." );
    video::ITexture* loadingScreenImage = device_->getVideoDriver()->getTexture(
            "GFX/Spiellogo.png" );
    // todo replace with generic helper method!
    if ( loadingScreenImage == 0 )
        Logfile::getInstance().emergencyExit(
                "Datei 'GFX/Spiellogo.png' nicht gefunden! Abbruch." );
    gui::IGUIImage* loadingScreenImageFrame =
            device_->getGUIEnvironment()->addImage(
                    core::recti( core::dimension2di( 0, 0),
                            Configuration::getInstance().getScreenSize() ) );
    loadingScreenImageFrame->setImage( loadingScreenImage );
    loadingScreenImageFrame->setScaleImage( true );
}



StateStartup::~StateStartup()
{
    // Niemals droppen, wenn Objekt nicht durch "create" erzeugt wurde!
}



#pragma GCC diagnostic ignored "-Wunused-parameter"
void StateStartup::update( f32 frameDeltaTime )
{
    switch( classCounter_ )
    {
        case 0:
            // print stuff on the screen
            Zufall::getInstance().start( device_->getTimer()->getRealTime() );
            break;
            //#ifdef _DEBUG_MODE
            //    DebugShapesManager::getInstance( driver );
            //    Debugwindow::getInstance( device );
            //#endif
            //    GenericHelperMethods::getInstance( device );
            //    TimerManager::getInstance();
            //    Scripting::getInstance();
            //    ObjectManager::getInstance( device );
            //    Collision::getInstance( scenemanager );
            //    Ton::getInstance( filesystem );
            //    Mauspfeil::getInstance( device );
            //    Weather::getInstance( scenemanager );
            //    Ground::getInstance( device );
            //    Camera::getInstance( scenemanager );
            //    Hero::getInstance( scenemanager );
            //    World::getInstance();
            // todo refactor or even remove Menues!
                //Menues::getInstance( device );
                // ...
                //Menues::getInstance().transitTo( Menues::MENUE_HAUPTMENUE );
        default:
            classCounter_ = 0;
            // change active gamestate
            break;
    }
    classCounter_++;
    //frameDeltaTime += 1.0f;
}
#pragma GCC diagnostic error "-Wunused-parameter"



void StateStartup::draw()
{
    device_->getVideoDriver()->beginScene( true, true, COL_BLACK );
    device_->getGUIEnvironment()->drawAll();
    device_->getVideoDriver()->endScene();
}



bool StateStartup::handleGuiEvents()
{
    return false;
}



/* private */
