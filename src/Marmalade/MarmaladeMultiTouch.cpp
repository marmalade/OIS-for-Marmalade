/*
 The zlib/libpng License
 
 Copyright (c) 2005-2007 Phillip Castaneda (pjcast -- www.wreckedgames.com)
 
 This software is provided 'as-is', without any express or implied warranty. In no event will
 the authors be held liable for any damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose, including commercial 
 applications, and to alter it and redistribute it freely, subject to the following
 restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that 
 you wrote the original software. If you use this software in a product, 
 an acknowledgment in the product documentation would be appreciated but is 
 not required.
 
 2. Altered source versions must be plainly marked as such, and must not be 
 misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source distribution.
 */
#include "Marmalade/MarmaladeMultiTouch.h"
#include "Marmalade/MarmaladeInputManager.h"

using namespace OIS;

//-------------------------------------------------------------------//
MarmaladeMultiTouch::MarmaladeMultiTouch( InputManager* creator, bool buffered )
	: MultiTouch(creator->inputSystemName(), buffered, 0, creator)
{
	MarmaladeInputManager *man = static_cast<MarmaladeInputManager*>(mCreator);
    man->_setMultiTouchUsed(true);
}

MarmaladeMultiTouch::~MarmaladeMultiTouch()
{
	s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)&MarmaladeInputManager::_touchCallback);
    s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)&MarmaladeInputManager::_touchMotionCallback);
	
	MarmaladeInputManager *man = static_cast<MarmaladeInputManager*>(mCreator);
    man->_setMultiTouchUsed(false);
}

void MarmaladeMultiTouch::_initialize()
{
	s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)&MarmaladeInputManager::_touchCallback, this);
	s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)&MarmaladeInputManager::_touchMotionCallback, this);
}

void MarmaladeMultiTouch::setBuffered( bool buffered )
{
	mBuffered = buffered;
}

void MarmaladeMultiTouch::capture()
{
	
}

void MarmaladeMultiTouch::_touchBegan(s3ePointerTouchEvent *touch)
{
	// Record touch (for touchMoved event)
	mTouchHistory[touch->m_TouchID] = LastTouch(touch->m_x, touch->m_y);

    MultiTouchState newState;
    newState.X.abs = touch->m_x;
    newState.Y.abs = touch->m_y;
    newState.touchType |= 1 << MT_Pressed;

    if( mListener && mBuffered )
    {
        mListener->touchPressed(MultiTouchEvent(this, newState));
    }
    else
    {
        mStates.push_back(newState);
    }
}

void MarmaladeMultiTouch::_touchEnded(s3ePointerTouchEvent *touch)
{
	// Erase touch (is this really necessary??)
    //mTouchHistory.erase(touch->m_TouchID);

    MultiTouchState newState;
    newState.X.abs = touch->m_x;
    newState.Y.abs = touch->m_y;
    newState.touchType |= 1 << MT_Released;

    if( mListener && mBuffered )
    {
        mListener->touchReleased(MultiTouchEvent(this, newState));
    }
    else
    {
        mStates.push_back(newState);
    }
}

void MarmaladeMultiTouch::_touchMoved(s3ePointerTouchMotionEvent *touch)
{
    LastTouch previousLocation = mTouchHistory.find(touch->m_TouchID)->second;

    MultiTouchState newState;
    newState.X.rel = (touch->m_x - previousLocation.x);
    newState.Y.rel = (touch->m_y - previousLocation.y);
    newState.X.abs = touch->m_x;
    newState.Y.abs = touch->m_y;
    newState.touchType |= 1 << MT_Moved;

    if( mListener && mBuffered )
    {
        mListener->touchMoved(MultiTouchEvent(this, newState));
    }
    else
    {
        mStates.push_back(newState);
    }
}

bool MarmaladeMultiTouch::_isSupported()
{
	return (s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) != S3E_FALSE);
}