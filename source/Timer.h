/*! \file Timer.h
 *  \brief Klasse stellt millisekundengenaue Timer bereit.
 */

#ifndef _TIMER_HEADER
#define _TIMER_HEADER

#include <irrlicht.h>

/*! \class Timer Timer.h "Timer.h"
 *  \brief Schnittstelle für millisekundengenaue Timer.
 *  \note Instanzierung: `Timer* myTimer = new Timer( maxValue );` \n Benutzen: `myTimer->update( frameDeltaTime );`
 */
class Timer
{

public:

    /*! \brief Konstruktor.
     *  \param maxValue (\a const \a f32) Maximalwert in Sekunden
     */
    Timer( const irr::f32 maxValue );

    /*! \brief Destruktor.
     */
    ~Timer();

    /*! \brief Erhöht den Timer um bestimmten Wert (in Sekunden).
     *  \attention Muss nicht aufgerufen werden, wenn der Timer von TimerManager verwaltet wird!
     *  \param frameDeltaTime (\a const \a f32) Wert in Sekunden
     *  \return \a Timer* Zeiger auf sich selbst
     */
    Timer* tick( const irr::f32 frameDeltaTime );

    /*! \brief Startet den Timer bei null.
     */
    void start();
    void restart();

    /*! \brief Stoppt und resettet den Timer.
     */
    void stop();

    /*! \brief Pausiert den Timer.
     */
    void pause();

    /*! \brief Lässt den Timer weiterlaufen.
     */
    void resume();

    /*! \brief Läuft der Timer gerade?
     *  \return `true` wenn Timer läuft, ansonsten `false`
     */
    bool isRunning() const;

    /*! \brief Ist der Timer gerade pausiert?
     *  \return `true` wenn Timer pausiert ist, ansonsten `false`
     */
    bool isPaused() const;

    /*! \brief Hat der Timer den Endwert erreicht/überschritten?
     *  \attention Timer wird bei Erreichen des Endwertes nicht automatisch zurückgesetzt oder angehalten!
     *             Dies könnte z.B. nützlich sein um herauszufinden, wie lange der Endwert schon überschritten ist.
     *  \return `true` wenn Timer fertig ist, ansonsten `false`
     */
    bool isFull() const;

    /*! \brief Gibt den eingestellten Endwert zurück.
     *  \return \a f32 Endwert in Sekunden
     */
    irr::f32 getMaxValue() const;

private:

    irr::f32 maxValue_;
    irr::f32 currentValue_;
    bool timerIsRunning_;
    bool timerIsPaused_;

    Timer( const Timer& );  // Objekt ist kopiergeschützt
    Timer& operator=( const Timer& );  // Objekt ist vor Zuweisung geschützt

};

/*! \class AlwaysRunningTimer
 *  \brief Schnittstelle für millisekundengenaue Timer, welche nicht zu stoppen sind.
 *  \note Instanzierung: `AlwaysRunningTimer* myTimer = new AlwaysRunningTimer( maxValue );`
 *        \n Benutzen: `myTimer->update( frameDeltaTime );`
 *  \attention Einmal gestartet, sind diese Timer durch nichts zu stoppen oder zu pausieren.
 */
class AlwaysRunningTimer : public Timer
{

public:

    AlwaysRunningTimer( const irr::f32 maxValue ) : Timer(maxValue) {}

    ~AlwaysRunningTimer() {}

    void stop() {}
    void pause() {}
    void resume() {}

private:

    AlwaysRunningTimer( const AlwaysRunningTimer& );  // Objekt ist kopiergeschützt
    AlwaysRunningTimer& operator=( const AlwaysRunningTimer& );  // Objekt ist vor Zuweisung geschützt

};

#endif