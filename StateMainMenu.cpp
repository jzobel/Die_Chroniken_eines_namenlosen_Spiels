#include "StateMainMenu.h"
#include "Configuration.h"
#include "Constants.h"
#include "Eventreceiver.h"
#include "GameStateManager.h"
#include "GenericHelperMethods.h"
#include "Mauspfeil.h"
#include "Logfile.h"
#include "Ton.h"



StateMainMenu::StateMainMenu( IrrlichtDevice* device )
: GameState(),
  driver_(0),
  guienv_(0),
  menuScreenImageCatalogue_(0),
  mainMenuTexture_(0),
  mainMenuBgColor_(video::SColor( 255, 248, 245, 240 )),
  hover_(false),
  currentMenu_(NONE)
{
    if ( device == 0 )
        Logfile::getInstance().emergencyExit(
                "Entchen in [StateMainMenu] nicht mehr gefunden! Abbruch." );
    driver_ = device->getVideoDriver();
    guienv_ = device->getGUIEnvironment();
    device->setWindowCaption( L"Die Chroniken eines namenlosen Spiels" );
    loadTextures();
    extractImagesFromCatalogue();
    createMainMenu();
    createNewPlayerMenu();
    createLoadGameMenu();
    createPreferencesMenu();
    createAboutMenu();
    switchToMenu( MAIN );
    transitTo( STARTING );
}



StateMainMenu::~StateMainMenu()
{
    // Niemals droppen, wenn Objekt nicht durch "create" erzeugt wurde!
    guienv_->clear();
}



void StateMainMenu::start( f32 frameDeltaTime )
{
    transitTo( RUNNING );
#pragma GCC diagnostic ignored "-Wunused-parameter" // ==> frameDeltaTime
}
#pragma GCC diagnostic error "-Wunused-parameter"



void StateMainMenu::update( f32 frameDeltaTime )
{
    if ( Eventreceiver::getInstance().hasKeyJustBeenReleased( KEY_ESCAPE ) )
    {
        GameStateManager::getInstance().requestNewState(
                            GameStateManager::SHUTDOWN );
        transitTo( STOPPING );
    }
#pragma GCC diagnostic ignored "-Wunused-parameter" // ==> frameDeltaTime
}
#pragma GCC diagnostic error "-Wunused-parameter"



void StateMainMenu::shutdown( f32 frameDeltaTime )
{
    transitTo( STOPPED );
#pragma GCC diagnostic ignored "-Wunused-parameter" // ==> frameDeltaTime
}
#pragma GCC diagnostic error "-Wunused-parameter"



void StateMainMenu::draw()
{
    driver_->beginScene( true, false, mainMenuBgColor_ );
    guienv_->drawAll();
    Mauspfeil::getInstance().draw();
    driver_->endScene();
}



bool StateMainMenu::handleGuiEvents( const irr::SEvent& event )
{
    bool result = false;
    gui::IGUIElement* caller = event.GUIEvent.Caller;
    gui::EGUI_ELEMENT_TYPE callerType = caller->getType();
    s32 callerId = caller->getID();
    switch ( event.GUIEvent.EventType )
    {
        case gui::EGET_ELEMENT_HOVERED:
            if ( callerType == gui::EGUIET_BUTTON )
            {
                focusButton( static_cast<gui::IGUIButton*>( caller ) );
                Ton::getInstance().playGUISound( Ton::SND_GUI_HOVER );
            }
            hover_ = true;
            break;
        case gui::EGET_ELEMENT_LEFT:
            if ( callerType == gui::EGUIET_BUTTON )
            {
                normalizeButton( static_cast<gui::IGUIButton*>( caller ) );
            }
            hover_ = false;
            break;
        case gui::EGET_BUTTON_CLICKED:
            switch ( currentMenu_ )
            {
                case MAIN:
                    result = mainMenuButtonHandler( callerId );
                    break;
                case NEW:
                    result = newPlayerMenuButtonHandler( callerId );
                    break;
                case LOAD:
                    result = loadGameMenuButtonHandler( callerId );
                    break;
                case PREFS:
                    result = preferencesMenuButtonHandler( callerId );
                    break;
                case ABOUT:
                    result = aboutMenuButtonHandler( callerId );
                    break;
                default:
                    break;
            }
            break;
        case gui::EGET_ELEMENT_FOCUSED:
            //printf("InFocus, Caller: %i\n", this->callerID_);
            break;
        case gui::EGET_ELEMENT_FOCUS_LOST:
            //printf("OutOfFocus, Caller: %i\n", this->callerID_);
            break;
        default:
            Logfile::getInstance().writeLine( Logfile::DEBUG,
                    "unbehandelter GUI-Event, Caller: %i\n", callerId );
            break;
    }
    return result;
}



/* private */



void StateMainMenu::transitTo( internalState state )
{
    switch ( state )
    {
        case STARTING:
            currentInternalState_ = STARTING;
            break;
        case RUNNING:
            Eventreceiver::getInstance().setEventReactionActive(
                    true, true, true );
            Mauspfeil::getInstance().setCurrentArrow(
                    Mauspfeil::MAUSPFEIL_SELECT );
            currentInternalState_ = RUNNING;
            break;
        case STOPPING:
            currentInternalState_ = STOPPING;
            break;
        default:
            currentInternalState_ = STOPPED;
            break;
    }
}



void StateMainMenu::loadTextures()
{
    GenericHelperMethods& helper = GenericHelperMethods::getInstance();
    driver_->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, false );
    helper.validateFileExistence( "GFX/menues1.bmp" );
    menuScreenImageCatalogue_ = driver_->getTexture( "GFX/menues1.bmp" );
    driver_->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, true );
}



void StateMainMenu::extractImagesFromCatalogue()
{
    driver_->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, false );
    driver_->disableFeature( video::EVDF_BILINEAR_FILTER, true );
    video::IImage* wholeImage = driver_->createImageFromFile(
            menuScreenImageCatalogue_->getName() );
    if ( !wholeImage )
        Logfile::getInstance().emergencyExit( "Bild nicht geladen!" );
    driver_->makeColorKeyTexture( menuScreenImageCatalogue_, COL_MAGICPINK );
    // Ausschneiden des mainMenue-Hintergrundbildes als eigene Textur
    core::dimension2du menueTextureSize = core::dimension2du( 436, 555 );
    video::IImage* partialImage = driver_->createImage(
            wholeImage->getColorFormat(), menueTextureSize );
    wholeImage->copyTo(
            partialImage,
            core::position2di( 0, 0 ),
            core::recti( core::position2di( 588, 212 ), menueTextureSize )
    );
    mainMenuTexture_ = driver_->addTexture(
            "GFX/mainMenueTexture.virtual", partialImage );
    partialImage->drop();
    driver_->makeColorKeyTexture( mainMenuTexture_, COL_MAGICPINK );
    wholeImage->drop();
    driver_->disableFeature( video::EVDF_BILINEAR_FILTER, false );
    driver_->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, true );
}



void StateMainMenu::createMainMenu()
{
    core::dimension2du textureSize = mainMenuTexture_->getSize();
    core::dimension2du screenSize = Configuration::getInstance().getScreenSize();
    core::position2di imagePosition = core::position2di(
            screenSize.Width - 20 - textureSize.Width,
            screenSize.Height - 20 - textureSize.Height
    );
    core::dimension2du buttonSize = core::dimension2du( 313, 88 );
    gui::IGUIImage* menueBgImage = guienv_->addImage(
            core::recti( imagePosition, textureSize ),
            guienv_->getRootGUIElement(), ID_MAIN_BGIMAGE, L"Hauptmenü"
    );
    menueBgImage->setImage( mainMenuTexture_ );
    gui::IGUIButton* newButton = guienv_->addButton(
            core::recti( core::position2di( 85, 63 ), buttonSize ),
            menueBgImage, ID_MAIN_NEWBUTTON, L"Neues Spiel"
    );
    changeStyleOfButton( newButton );
    gui::IGUIButton* resumeButton = guienv_->addButton(
            core::recti( core::position2di( 85, 134 ), buttonSize ),
            menueBgImage, ID_MAIN_RESUMEBUTTON, L"Fortsetzen"
    );
    changeStyleOfButton( resumeButton );
    gui::IGUIButton* loadButton = guienv_->addButton(
            core::recti( core::position2di( 85, 207 ), buttonSize ),
            menueBgImage, ID_MAIN_LOADBUTTON, L"Laden"
    );
    changeStyleOfButton( loadButton );
    gui::IGUIButton* prefsButton = guienv_->addButton(
            core::recti( core::position2di( 85, 288 ), buttonSize ),
            menueBgImage, ID_MAIN_PREFSBUTTON, L"Einstellungen"
    );
    changeStyleOfButton( prefsButton );
    gui::IGUIButton* aboutButton = guienv_->addButton(
            core::recti( core::position2di( 85, 356 ), buttonSize ),
            menueBgImage, ID_MAIN_ABOUTBUTTON, L"Mitwirkende"
    );
    changeStyleOfButton( aboutButton );
    gui::IGUIButton* exitButton = guienv_->addButton(
            core::recti( core::position2di( 85, 416 ), buttonSize ),
            menueBgImage, ID_MAIN_EXITBUTTON, L"Beenden"
    );
    changeStyleOfButton( exitButton );
    hideMenu( MAIN );
}



bool StateMainMenu::mainMenuButtonHandler( s32 callerId )
{
    bool result = true;
    switch ( callerId )
    {
        case ID_MAIN_NEWBUTTON:
            switchToMenu( NEW );
            break;
        case ID_MAIN_RESUMEBUTTON:
            //( MENUE_SPIEL_LAEUFT );
            break;
        case ID_MAIN_LOADBUTTON:
            switchToMenu( LOAD );
            break;
        case ID_MAIN_PREFSBUTTON:
            switchToMenu( PREFS );
            break;
        case ID_MAIN_ABOUTBUTTON:
            switchToMenu( ABOUT );
            break;
        case ID_MAIN_EXITBUTTON:
            GameStateManager::getInstance().requestNewState(
                                GameStateManager::SHUTDOWN );
            transitTo( STOPPING );
            break;
        default:
            unknownCaller( callerId );
            result = false;
            break;
    }
    Ton::getInstance().playGUISound( Ton::SND_GUI_CLICKBUTTON );
    return result;
}



void StateMainMenu::createNewPlayerMenu()
{
    core::dimension2du textureSize = mainMenuTexture_->getSize();
    core::dimension2du screenSize = Configuration::getInstance().getScreenSize();
    core::position2di imagePosition = core::position2di(
            screenSize.Width - 20 - textureSize.Width,
            screenSize.Height - 20 - textureSize.Height
    );
    core::dimension2du buttonSize = core::dimension2du( 313, 88 );
    gui::IGUIImage* menueBgImage = guienv_->addImage(
            core::recti( imagePosition, textureSize ),
            guienv_->getRootGUIElement(), ID_NEW_BGIMAGE, L"Neuer Spieler"
    );
    menueBgImage->setImage( mainMenuTexture_ );
    gui::IGUIButton* exitButton = guienv_->addButton(
            core::recti( core::position2di( 85, 416 ), buttonSize ),
            menueBgImage, ID_NEW_EXITBUTTON, L"Zurück"
    );
    changeStyleOfButton( exitButton );
    hideMenu( NEW );
}



bool StateMainMenu::newPlayerMenuButtonHandler( s32 callerId )
{
    bool result = true;
    switch ( callerId )
    {
        case ID_NEW_EXITBUTTON:
            switchToMenu( MAIN );
            break;
        default:
            unknownCaller( callerId );
            result = false;
            break;
    }
    Ton::getInstance().playGUISound( Ton::SND_GUI_CLICKBUTTON );
    return result;
}



void StateMainMenu::createLoadGameMenu()
{
    core::dimension2du textureSize = mainMenuTexture_->getSize();
    core::dimension2du screenSize = Configuration::getInstance().getScreenSize();
    core::position2di imagePosition = core::position2di(
            screenSize.Width - 20 - textureSize.Width,
            screenSize.Height - 20 - textureSize.Height
    );
    core::dimension2du buttonSize = core::dimension2du( 313, 88 );
    gui::IGUIImage* menueBgImage = guienv_->addImage(
            core::recti( imagePosition, textureSize ),
            guienv_->getRootGUIElement(), ID_LOAD_BGIMAGE, L"Spiel laden"
    );
    menueBgImage->setImage( mainMenuTexture_ );
    gui::IGUIButton* exitButton = guienv_->addButton(
            core::recti( core::position2di( 85, 416 ), buttonSize ),
            menueBgImage, ID_LOAD_EXITBUTTON, L"Zurück"
    );
    changeStyleOfButton( exitButton );
    hideMenu( LOAD );
}



bool StateMainMenu::loadGameMenuButtonHandler( s32 callerId )
{
    bool result = true;
    switch ( callerId )
    {
        case ID_LOAD_EXITBUTTON:
            switchToMenu( MAIN );
            break;
        default:
            unknownCaller( callerId );
            result = false;
            break;
    }
    Ton::getInstance().playGUISound( Ton::SND_GUI_CLICKBUTTON );
    return result;
}



void StateMainMenu::createPreferencesMenu()
{
    core::dimension2du textureSize = mainMenuTexture_->getSize();
    core::dimension2du screenSize = Configuration::getInstance().getScreenSize();
    core::position2di imagePosition = core::position2di(
            screenSize.Width - 20 - textureSize.Width,
            screenSize.Height - 20 - textureSize.Height
    );
    core::dimension2du buttonSize = core::dimension2du( 313, 88 );
    gui::IGUIImage* menueBgImage = guienv_->addImage(
            core::recti( imagePosition, textureSize ),
            guienv_->getRootGUIElement(), ID_PREFS_BGIMAGE, L"Einstellungen"
    );
    menueBgImage->setImage( mainMenuTexture_ );
    gui::IGUIButton* exitButton = guienv_->addButton(
            core::recti( core::position2di( 85, 416 ), buttonSize ),
            menueBgImage, ID_PREFS_EXITBUTTON, L"Zurück"
    );
    changeStyleOfButton( exitButton );
    hideMenu( PREFS );
}



bool StateMainMenu::preferencesMenuButtonHandler( s32 callerId )
{
    bool result = true;
    switch ( callerId )
    {
        case ID_PREFS_EXITBUTTON:
            switchToMenu( MAIN );
            break;
        default:
            unknownCaller( callerId );
            result = false;
            break;
    }
    Ton::getInstance().playGUISound( Ton::SND_GUI_CLICKBUTTON );
    return result;
}



void StateMainMenu::createAboutMenu()
{
    core::dimension2du textureSize = mainMenuTexture_->getSize();
    core::dimension2du screenSize = Configuration::getInstance().getScreenSize();
    core::position2di imagePosition = core::position2di(
            screenSize.Width - 20 - textureSize.Width,
            screenSize.Height - 20 - textureSize.Height
    );
    core::dimension2du buttonSize = core::dimension2du( 313, 88 );
    gui::IGUIImage* menueBgImage = guienv_->addImage(
            core::recti( imagePosition, textureSize ),
            guienv_->getRootGUIElement(), ID_ABOUT_BGIMAGE, L"Über..."
    );
    menueBgImage->setImage( mainMenuTexture_ );
    gui::IGUIButton* exitButton = guienv_->addButton(
            core::recti( core::position2di( 85, 416 ), buttonSize ),
            menueBgImage, ID_ABOUT_EXITBUTTON, L"Zurück"
    );
    changeStyleOfButton( exitButton );
    hideMenu( ABOUT );
}



bool StateMainMenu::aboutMenuButtonHandler( s32 callerId )
{
    bool result = true;
    switch ( callerId )
    {
        case ID_ABOUT_EXITBUTTON:
            switchToMenu( MAIN );
            break;
        default:
            unknownCaller( callerId );
            result = false;
            break;
    }
    Ton::getInstance().playGUISound( Ton::SND_GUI_CLICKBUTTON );
    return result;
}



void StateMainMenu::changeStyleOfButton( gui::IGUIButton* button)
{
    normalizeButton( button );
    button->setPressedImage(  menuScreenImageCatalogue_,
            core::recti( 661, 115, 976, 204 ) );
    button->setIsPushButton( false );
    button->setDrawBorder( false );
    button->setUseAlphaChannel( true );
}



void StateMainMenu::normalizeButton( gui::IGUIButton* button )
{
    button->setImage( menuScreenImageCatalogue_,
            core::recti( 654, 22, 966, 110 ) );
}



void StateMainMenu::focusButton( gui::IGUIButton* button )
{
    button->setImage( menuScreenImageCatalogue_,
            core::recti( 661, 115, 976, 204 ) );
}



void StateMainMenu::unknownCaller( s32 callerId )
{
    Logfile::getInstance().writeLine( Logfile::DEBUG,
                        "unbekannter Knopf geklickt, CallerID: ", callerId );
}



void StateMainMenu::switchToMenu( MENU menu )
{
    if ( menu )
    {
        hideMenu( currentMenu_ );
        currentMenu_ = menu;
        displayMenu( currentMenu_ );
    }
}



void StateMainMenu::displayMenu( MENU menu )
{
    updateWithChildren( guienv_->getRootGUIElement()->getElementFromId( menu ),
            true );
}



void StateMainMenu::hideMenu( MENU menu )
{
    updateWithChildren( guienv_->getRootGUIElement()->getElementFromId( menu ),
            false );
}



void StateMainMenu::updateWithChildren( gui::IGUIElement* element, bool enable )
{
    if ( !element )
        return;
    element->setVisible( enable );
    element->setEnabled( enable );
    core::list<gui::IGUIElement*> children = element->getChildren();
    if ( children.empty() )
        return;
    core::list<gui::IGUIElement*>::Iterator it = children.begin();
    for ( ; it != children.end(); ++it )
        updateWithChildren( (*it), enable );
}