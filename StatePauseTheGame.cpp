#include "StatePauseTheGame.h"
#include "Constants.h"
#include "Eventreceiver.h"
#include "GameStateManager.h"
#include "Logfile.h"
#include "Mauspfeil.h"



StatePauseTheGame::StatePauseTheGame( IrrlichtDevice* device )
: GameState(),
  device_(device)
{
    if ( device_ == 0 )
        Logfile::getInstance().emergencyExit( "Entchen in [StatePauseTheGame] nicht mehr gefunden! Abbruch." );
    device_->setWindowCaption( L"Die Chroniken eines namenlosen Spiels (pausiert)" );
    transitTo( STARTING );
}



StatePauseTheGame::~StatePauseTheGame()
{
    // Niemals droppen, wenn Objekt nicht durch "create" erzeugt wurde!
}



void StatePauseTheGame::start( f32 frameDeltaTime )
{
    (void)frameDeltaTime; // game state does no real-time graphics on startup
    transitTo( RUNNING );
}



void StatePauseTheGame::update( f32 frameDeltaTime )
{
    (void)frameDeltaTime; // game state does no real-time graphics so far
    if ( Eventreceiver::getInstance().hasKeyJustBeenReleased( KEY_ESCAPE ) )
    {
        GameStateManager::getInstance().requestNewState( GameStateManager::GAME );
        transitTo( STOPPING );
    }
}



void StatePauseTheGame::shutdown( f32 frameDeltaTime )
{
    (void)frameDeltaTime; // game state does no real-time graphics on shutdown
    transitTo( STOPPED );
}



void StatePauseTheGame::draw()
{
    //u32 x = device_->getTimer()->getRealTime();
    device_->getVideoDriver()->beginScene( true, true, COL_GREEN );
    device_->getSceneManager()->drawAll();
    device_->getGUIEnvironment()->drawAll();
    Mauspfeil::getInstance().draw();
    device_->getVideoDriver()->endScene();
    //Logfile::getInstance().dbg( "draw(ms): ", device_->getTimer()->getRealTime() - x);
}



bool StatePauseTheGame::handleGuiEvents( const irr::SEvent& event )
{
    (void)event; // no event handling necessary here
    return false;
}



/* private */



void StatePauseTheGame::transitTo( internalState state )
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
            Eventreceiver::getInstance().setEventReactionActive( false, false, false );
            Mauspfeil::getInstance().setCurrentArrow( Mauspfeil::MAUSPFEIL_UNSICHTBAR );
            break;
        default:
            currentInternalState_ = STOPPED;
            break;
    }
}
