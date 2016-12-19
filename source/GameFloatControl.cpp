#include "GameFloatControl.h"
#include "Configuration.h"
#include "Constants.h"
#include "Eventreceiver.h"
#include "GameStateManager.h"
#include "Logfile.h"
#include "TimerManager.h"
#ifdef _DEBUG_MODE
#include "Debugwindow.h"
#endif



// TODO as soon as there are no singletons using anything from device or device itself, refactor this class!
GameFloatControl& GameFloatControl::getInstance()
{
    static GameFloatControl instance;
    return instance;
}



void GameFloatControl::run()
{
    if ( !start() )
        return;
    Eventreceiver& eventreceiver = Eventreceiver::getInstance();
    GameStateManager& game = GameStateManager::getInstance();

    const irr::f32 FRAME_DELTA_TIME = 0.008f;  // 0.008s ~= 125 FPS fixed
    const irr::u32 FRAME_DELTA_TIME_IN_MS = static_cast<irr::u32>( FRAME_DELTA_TIME * 1000 );  // for performance.

    irr::u32 loops;
    bool we_must_draw;
    irr::u32 next = device_->getTimer()->getTime();
    while ( device_->run() )
    {
        if ( !device_->isWindowActive() )
            device_->yield();
        loops = 0;
        we_must_draw = false;
        while ( device_->getTimer()->getTime() > next && loops < 10 ) // Time will slow down if FPS<12.5 (125FPS / 10)
        {
            TimerManager::getInstance().tick( FRAME_DELTA_TIME );
            game.update( FRAME_DELTA_TIME );
            if ( !device_->run() )
            {
                we_must_draw = false;
                break;
            }
            eventreceiver.setKeysLastState();
            next += FRAME_DELTA_TIME_IN_MS;
            we_must_draw = true;
            if ( game.allFramesMustBeShown() )
                break;
            ++loops;
        }
        if ( we_must_draw )
        {
#ifdef _DEBUG_MODE
            printFPS();
#endif
            game.draw();
        }
    }
    stop();
}



irr::s32 GameFloatControl::status()
{
    return status_;
}



/* private */



GameFloatControl::GameFloatControl()
: device_(0)
  ,status_(1)
#ifdef _DEBUG_MODE
  ,fps_(0)
  ,lastFPS_(0)
#endif
{
    ;
}



GameFloatControl::~GameFloatControl()
{
    if ( device_ )
    {
        device_->drop();
        device_ = 0;
    }
}



bool GameFloatControl::start()
{
    device_ = irr::createDevice( irr::video::EDT_NULL );
    if ( device_ == 0 )
        return false;
    createLogfile();
    readConfig();
    device_->drop();
    if ( !createDeviceFromConfig() )
        return false;
    GameStateManager::getInstance( device_ );
#ifdef _DEBUG_MODE
    Debugwindow::getInstance( device_ );
#endif
    status_ = 0;
    return true;
}



void GameFloatControl::stop()
{
    Logfile::getInstance().writeLine( Logfile::INFO, "" );
    Logfile::getInstance().writeLine( Logfile::INFO, "'Die Chroniken eines namenlosen Spiels' wurde normal beendet." );
}



void GameFloatControl::createLogfile()
{
    Logfile::getInstance( device_->getFileSystem(),
#ifdef _DEBUG_MODE
    "debug.log", Logfile::DEBUG );
#else
    "game.log", Logfile::INFO );
#endif
}



void GameFloatControl::readConfig()
{
    Configuration::getInstance( device_->getFileSystem() ).readConfigFile( "CONFIG.CFG" );
}



bool GameFloatControl::createDeviceFromConfig()
{
    Logfile& logfile = Logfile::getInstance();
    Configuration& config = Configuration::getInstance();
    Eventreceiver::getInstance().setEventReactionActive( false, false, false );

    irr::SIrrlichtCreationParameters params = irr::SIrrlichtCreationParameters();
    params.AntiAlias = 1; // TODO konfigurierbar machen
    params.Stencilbuffer = true; // TODO konfigurierbar machen
    params.Bits = static_cast<irr::u8>( config.getColorDepht() ); // TODO refactor to u8!
    params.DriverType = config.getRenderMode();
    params.Fullscreen = config.isFullScreen();
    params.Vsync = params.Fullscreen; // TODO konfigurierbar machen
    params.EventReceiver = &Eventreceiver::getInstance();
    params.WindowSize = config.getScreenSize();

    device_ = irr::createDeviceEx( params );
    if ( device_ == 0 )
        return false;
    logfile.setNewFilesystem( device_->getFileSystem() );
    config.setNewFilesystem( device_->getFileSystem() );
    logfile.writeLine( Logfile::DETAIL, "3D-Entchen erfolgreich erstellt." );
    logfile.writeLine( Logfile::DETAIL, "    Version: IrrLicht ", device_->getVersion() );
    logfile.writeLine( Logfile::DETAIL, "    Treiber: ", device_->getVideoDriver()->getName() );
    device_->getVideoDriver()->disableFeature(irr::video::EVDF_BILINEAR_FILTER, false);
    device_->getVideoDriver()->disableFeature(irr::video::EVDF_STENCIL_BUFFER, false);
    device_->getCursorControl()->setVisible( false );
    device_->getVideoDriver()->beginScene( true, false, COL_BLACK );
    device_->getVideoDriver()->endScene();
    return true;
 }



#ifdef _DEBUG_MODE
void GameFloatControl::printFPS()
{
    fps_ = device_->getVideoDriver()->getFPS();
    {
        irr::core::stringw str = L"FPS:";
        str += fps_;
        Debugwindow::getInstance().addLine( "GameFloatControl::printFPS()", str );
        lastFPS_ = fps_;
    }
}
#endif
