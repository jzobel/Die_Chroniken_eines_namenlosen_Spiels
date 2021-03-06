#include "StatePlayTheGame.h"
#include "Camera.h"
#include "Collision.h"
#include "Eventreceiver.h"
#include "GameStateManager.h"
#include "Ground.h"
#include "Hero.h"
#include "Logfile.h"
#include "Mauspfeil.h"
#include "ObjectManager.h"
#include "Weather.h"
#ifdef _DEBUG_MODE
#include "DebugShapesManager.h"
#include "Debugwindow.h"
#endif


StatePlayTheGame::StatePlayTheGame( IrrlichtDevice* device )
: GameState(),
  device_(device)
{
    if ( device_ == 0 )
        Logfile::getInstance().emergencyExit( "Entchen in [StatePlayTheGame] nicht mehr gefunden! Abbruch." );
    device_->setWindowCaption( L"Die Chroniken eines namenlosen Spiels" );
    transitTo( STARTING );
}



StatePlayTheGame::~StatePlayTheGame()
{
    // Niemals droppen, wenn Objekt nicht durch "create" erzeugt wurde!
}



void StatePlayTheGame::start( f32 frameDeltaTime )
{
    (void)frameDeltaTime; // game state does no real-time graphics on startup
    transitTo( RUNNING );
}



void StatePlayTheGame::update( f32 frameDeltaTime )
{
    //u32 x = device_->getTimer()->getRealTime();
    checkInputForGame( frameDeltaTime );
    if ( Eventreceiver::getInstance().hasKeyJustBeenReleased( KEY_ESCAPE ) )
    {
        GameStateManager::getInstance().requestNewState( GameStateManager::UNLOAD );
        transitTo( STOPPING );
    }
#ifdef _DEBUG_MODE
    if ( Eventreceiver::getInstance().hasKeyJustBeenPressedDown( KEY_F1 ) )
        Debugwindow::getInstance().toggle();
    if ( Eventreceiver::getInstance().isKeyDown( KEY_TAB ) )
        Camera::getInstance().startZooming( 1.0f );
    if ( Eventreceiver::getInstance().isKeyDown( KEY_KEY_R ) )
        Camera::getInstance().startZooming( -1.0f );
    Debugwindow::getInstance().addLine( "StatePlayTheGame::update", L"Dreiecke: ", device_->getVideoDriver()->getPrimitiveCountDrawn() );
#endif
    Weather::getInstance().update();
    Ground::getInstance().update();
    ObjectManager::getInstance().update( frameDeltaTime );
    Hero::getInstance().current()->update( frameDeltaTime );
    Camera::getInstance().update( frameDeltaTime );
    //Logfile::getInstance().dbg( "logic(ms): ", device_->getTimer()->getRealTime() - x);
}



void StatePlayTheGame::shutdown( f32 frameDeltaTime )
{
    (void)frameDeltaTime; // game state does no real-time graphics on shutdown
    transitTo( STOPPED );
}



void StatePlayTheGame::draw()
{
    //u32 x = device_->getTimer()->getRealTime();
    device_->getVideoDriver()->beginScene( true, true, Weather::getInstance().getSkyColor() );
    device_->getSceneManager()->drawAll();
    device_->getGUIEnvironment()->drawAll();
#ifdef _DEBUG_MODE
    DebugShapesManager::getInstance().drawAll();
    Debugwindow::getInstance().show();
#endif
    Mauspfeil::getInstance().draw();
    device_->getVideoDriver()->endScene();
    //Logfile::getInstance().dbg( "draw(ms): ", device_->getTimer()->getRealTime() - x);
}



bool StatePlayTheGame::handleGuiEvents( const irr::SEvent& event )
{
    (void)event; // no event handling necessary here
    return false;
}



/* private */



void StatePlayTheGame::transitTo( internalState state )
{
    switch ( state )
    {
        case STARTING:
            currentInternalState_ = STARTING;
            break;
        case RUNNING:
            currentInternalState_ = RUNNING;
            Eventreceiver::getInstance().setEventReactionActive( true, true, true );
            Mauspfeil::getInstance().setCurrentArrow( Mauspfeil::MAUSPFEIL_SELECT );
            break;
        case STOPPING:
            currentInternalState_ = STOPPING;
            break;
        default:
            currentInternalState_ = STOPPED;
            break;
    }
}



void StatePlayTheGame::checkInputForGame( f32 frameDeltaTime )
{
    Eventreceiver& receiver = Eventreceiver::getInstance();
#ifdef _DEBUG_MODE
    Camera& camera = Camera::getInstance();
    Hero& hero = Hero::getInstance();
    if ( ( receiver.isKeyDown( KEY_CAPITAL ) && !receiver.wasKeyDown( KEY_CAPITAL ) )
            || ( !receiver.isKeyDown( KEY_CAPITAL ) && ( receiver.isShiftDown() ^ receiver.wasShiftDown() ) ) )
    {
        hero.current()->toggleSpeed();
        camera.toggleSpeed();
    }
    if ( receiver.hasKeyJustBeenReleased( KEY_F2 ) )
        Ground::getInstance().switchDebugMode();
    if ( receiver.hasKeyJustBeenReleased( KEY_F3 ) )
        ObjectManager::getInstance().switchStaticsDebugMode();
    if ( receiver.hasKeyJustBeenReleased( KEY_F4 ) )
        ObjectManager::getInstance().switchNPCsDebugMode();
#endif
    checkCameraSpecificInput( frameDeltaTime );
    checkGameSpecificInput();
}



void StatePlayTheGame::checkCameraSpecificInput( f32 frameDeltaTime )
{
    Eventreceiver& receiver = Eventreceiver::getInstance();
    Camera& camera = Camera::getInstance();
    if ( !core::equals( receiver.getMouseWheel(), 0.0f ) )
        camera.startZooming( receiver.getMouseWheel() );
    if ( receiver.isMMBDown() )
    {
        register s32 delta = receiver.getMouseLastX() - receiver.getMouseX();
        if ( delta != 0 )
        {
            if ( frameDeltaTime < 0.003f )
                camera.rotate( static_cast<f32>( delta ) * 0.003f );
            else
                camera.rotate( static_cast<f32>( delta ) * frameDeltaTime );
        }
    }
    if ( receiver.isKeyDown( KEY_KEY_Q ) )
        camera.rotate( frameDeltaTime );
    if ( receiver.isKeyDown( KEY_KEY_E ) )
        camera.rotate( -frameDeltaTime );
}



void StatePlayTheGame::checkGameSpecificInput()
{
    Eventreceiver& receiver = Eventreceiver::getInstance();
    Collision& collision = Collision::getInstance();
    Hero& hero = Hero::getInstance();
#ifdef _DEBUG_MODE
    if ( receiver.isRMBDown() )
        if ( collision.isMouseIntersectingWithWorld() )
        {
            Debugwindow::getInstance().addLine( "StatePlayTheGame::checkGameSpecificInput1", L"name: ", collision.collisionNode->getName() );
            Debugwindow::getInstance().addLine( "StatePlayTheGame::checkGameSpecificInput2", L"id: ", collision.collisionNode->getID() );
        }
#endif
    if ( receiver.isLMBDown() )
        if ( collision.isMouseIntersectingWithWorld() )
            hero.current()->moveTo( collision.collisionPoint, true );
    if ( !receiver.isLMBDown() && receiver.wasLMBDown() )
        if ( collision.isMouseIntersectingWithWorld() )
            hero.current()->moveTo( collision.collisionPoint, false );
}
