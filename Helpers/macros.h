/*
 * macros.h
 *
 *  Created on: 26.12.2019
 *      Author: p.grus
 *      Purpose: Contains some useful methods
 *      source: https://github.com/ProductiveGrus2/CNC/edit/master/Helpers/macros.h
 */
#ifndef __MACROS_H__
#define __MACROS_H__

#include <Arduino.h>
struct Callback
{
    Callback(void (*f)(void *) = 0, void *d = 0)
        : function(f), data(d) {}
    void (*function)(void *);
    void *data;


    bool Execute(void *d=0)
    {
        if (function != nullptr)
        {
            if (d == 0)
            {
                function(data);
            }
            else
            {
                function(d);
            }  
            return true;
        }
        return false;
    }
};



template<typename T=int>
struct SleepState
{
    public:
        bool IsSleeping()
        {
            return (mUntil > 0 && mUntil > micros());
        }

        void Enable(unsigned long amount, T pos)
        {
            mUntil = micros() + amount;
            mPosition = pos;
        }
        void Reset()
        {
            mUntil = 0;
            mPosition = mDefaultPosition;
        }

        bool StateFit(T state)
        {
            return (mPosition == state);
        }

        SleepState(T defaultPosition=0)
        : mUntil(0)
        , mDefaultPosition(defaultPosition)
        , mPosition(mDefaultPosition)
        {}

        T sleepPos()
        {
            return mPosition;
        }

    private:
        unsigned long mUntil;
        T mDefaultPosition;
        T mPosition;
};

#endif //!__MACROS_H__
