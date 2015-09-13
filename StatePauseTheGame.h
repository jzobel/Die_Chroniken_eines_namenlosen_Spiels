/*! \file StatePauseTheGame.h
 *  \brief Klasse stellt Schnittstelle für Pausieren des Spiels bereit.
 */

#ifndef _STATEPAUSETHEGAME_HEADER
#define _STATEPAUSETHEGAME_HEADER

#include <irrlicht.h>
#include "GameState.h"

using namespace irr;

/*! \class StatePauseTheGame StatePauseTheGame.h "StatePauseTheGame.h"
 *  \brief Schnittstelle für das Pausieren des Spiels.
 *  \note Instanzierung: `StatePauseTheGame* myStatePauseTheGame = new StatePauseTheGame();`
 *  \n Benutzen: `myStatePauseTheGame->update(...);`
 */
class StatePauseTheGame : public GameState
{

public:

    /*! \brief Konstruktor.
     *  \param device (\a IrrlichtDevice*) Zeiger auf das Entchen
     *  \return Zeiger auf das instanzierte Klassenobjekt
     */
    StatePauseTheGame( IrrlichtDevice* device );

	/*! \brief Destruktor
     */
	~StatePauseTheGame();

    /*! \brief Aktualisiert die Startup-Phase des Status.
     *  \param frameDeltaTime (\a const \a f32) Dauer des letzten Frames in Sekunden
     *  \return -
     */
    void start( const f32 frameDeltaTime );

    /*! \brief Aktualisiert die Shutdown-Phase des Status.
     *  \param frameDeltaTime (\a const \a f32) Dauer des letzten Frames in Sekunden
     *  \return -
     */
    void shutdown( const f32 frameDeltaTime );

    /*! \brief Aktualisiert den Status.
     *  \param frameDeltaTime (\a const \a f32) Dauer des letzten Frames in Sekunden
     *  \return -
     */
    void update( const f32 frameDeltaTime );

    /*! \brief Zeichnet den Status auf den Bildschirm.
     *  \param -
     *  \return -
     */
    void draw();

    /*! \brief Erweiterung für Eventreceiver, behandelt GUI-Events des Status.
     *  \param event (\a irr::SEvent&) Event-Referenz
     *  \return `true` wenn GUI-Event behandelt wurde, ansonsten `false`
     */
    bool handleGuiEvents( const irr::SEvent& event );

private:

    IrrlichtDevice* device_;

    StatePauseTheGame( const StatePauseTheGame& );
    StatePauseTheGame& operator=( const StatePauseTheGame& );

    void transitTo( internalState state );

};

#endif
