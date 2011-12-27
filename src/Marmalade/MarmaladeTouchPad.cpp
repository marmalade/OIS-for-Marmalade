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
#include "Marmalade/MarmaladeTouchPad.h"
#include "Marmalade/MarmaladeInputManager.h"

using namespace OIS;

//-------------------------------------------------------------------//
MarmaladeTouchPad::MarmaladeTouchPad( InputManager* creator, bool buffered )
	: MultiTouch(creator->inputSystemName(), buffered, 0, creator)
{
	MarmaladeInputManager *man = static_cast<MarmaladeInputManager*>(mCreator);
    man->_setTouchPadUsed(true);
}

MarmaladeTouchPad::~MarmaladeTouchPad()
{
	s3eTouchpadUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)&MarmaladeInputManager::_touchCallback);
    s3eTouchpadUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)&MarmaladeInputManager::_touchMotionCallback);
	
	MarmaladeInputManager *man = static_cast<MarmaladeInputManager*>(mCreator);
    man->_setTouchPadUsed(false);
}

void MarmaladeTouchPad::_initialize()
{
	s3eTouchpadRegister(S3E_TOUCHPAD_EVENT, (s3eCallback)&MarmaladeInputManager::_touchPadCallback, this);
	s3eTouchpadRegister(S3E_TOUCHPAD_MOTION_EVENT, (s3eCallback)&MarmaladeInputManager::_touchPadMotionCallback, this);
}

void MarmaladeTouchPad::setBuffered( bool buffered )
{
	mBuffered = buffered;
}

void MarmaladeTouchPad::capture()
{
	
}

void MarmaladeTouchPad::_touchBegan(s3eTouchpadEvent *touch)
{
	// Record touch (for touchMoved event)
	mTouchHistory[touch->m_TouchID] = LastTouch(touch->m_x, touch->m_y);

    MultiTouchState newState;
    newState.X.abs = touch->m_x;
    newState.Y.abs = touch->m_y;
    newState.touchType |= 1 << MT_Pressed;

	/*
    if( mListener && mBuffered )
    {
        mListener->touchPressed(MultiTouchEvent(this, newState));
    }
    else
    {
        mStates.push_back(newState);
    }
	*/
}

void MarmaladeTouchPad::_touchEnded(s3eTouchpadEvent *touch)
{
	// Erase touch (is this really necessary??)
    //mTouchHistory.erase(touch->m_TouchID);

    MultiTouchState newState;
    newState.X.abs = touch->m_x;
    newState.Y.abs = touch->m_y;
    newState.touchType |= 1 << MT_Released;

	/*
    if( mListener && mBuffered )
    {
        mListener->touchReleased(MultiTouchEvent(this, newState));
    }
    else
    {
        mStates.push_back(newState);
    }
	*/
}

void MarmaladeTouchPad::_touchMoved(s3eTouchpadMotionEvent *touch)
{
    LastTouch previousLocation = mTouchHistory.find(touch->m_TouchID)->second;

    MultiTouchState newState;
    newState.X.rel = (touch->m_x - previousLocation.x);
    newState.Y.rel = (touch->m_y - previousLocation.y);
    newState.X.abs = touch->m_x;
    newState.Y.abs = touch->m_y;
    newState.touchType |= 1 << MT_Moved;

	/*
    if( mListener && mBuffered )
    {
        mListener->touchMoved(MultiTouchEvent(this, newState));
    }
    else
    {
        mStates.push_back(newState);
    }
	*/
}

bool MarmaladeTouchPad::_isSupported()
{
	return (s3eTouchpadAvailable() != S3E_FALSE);
}