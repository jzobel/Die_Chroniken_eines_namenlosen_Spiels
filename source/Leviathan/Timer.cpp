#include "Timer.h"

namespace leviathan
{
    namespace core
    {

        Timer::Timer( const irr::f32 maxValue )
        : maxValue_(maxValue),
          currentValue_(0.0f),
          timerIsRunning_(false),
          timerIsPaused_(false),
          timerIsFull_(false)
        {
            // nop
        }

        Timer& Timer::tick( const irr::f32 frameDeltaTime )
        {
            if ( timerIsRunning_ && !timerIsPaused_ )
                currentValue_ += frameDeltaTime;
            return *this;
        }

        void Timer::start()
        {
            timerIsPaused_ = false;
            timerIsRunning_ = true;
            currentValue_ = 0.0f;
        }

        void Timer::restart()
        {
            timerIsFull_ = false;
            start();
        }

        void Timer::stop()
        {
            timerIsPaused_ = false;
            timerIsRunning_ = false;
            currentValue_ = 0.0f;
        }

        void Timer::pause()
        {
            if ( timerIsRunning_ )
                timerIsPaused_ = true;
        }

        void Timer::resume()
        {
            if ( timerIsPaused_ )
            {
                timerIsRunning_ = true;
                timerIsPaused_ = false;
            }
        }

        bool Timer::isRunning() const
        {
            return timerIsRunning_;
        }

        bool Timer::isPaused() const
        {
            return timerIsPaused_;
        }

        bool Timer::isFull()
        {
            if ( currentValue_ >= maxValue_ )
                timerIsFull_ = true;
            return timerIsFull_;
        }

        irr::f32 Timer::getMaxValue() const
        {
            return maxValue_;
        }
    }
}
