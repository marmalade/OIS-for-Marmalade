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
#include "Marmalade/MarmaladeInputManager.h"
#include "Marmalade/MarmaladeJoyStick.h"
#include "Marmalade/MarmaladeCompass.h"
#include "Marmalade/MarmaladeTouchPad.h"
#include "Marmalade/MarmaladeMultiTouch.h"
#include "OISException.h"

using namespace OIS;

//--------------------------------------------------------------------------------//
MarmaladeInputManager::MarmaladeInputManager() : InputManager("MarmaladeInputManager")
{
	//Setup our internal factories
	mFactories.push_back(this);
}

//--------------------------------------------------------------------------------//
MarmaladeInputManager::~MarmaladeInputManager()
{
	
}

//--------------------------------------------------------------------------------//
void MarmaladeInputManager::_initialize( ParamList &paramList )
{
	// Check support to input types
	bMultitouchSupported = MarmaladeMultiTouch::_isSupported();
	bAccelerometerSupported = MarmaladeAccelerometer::_isSupported();
	bCompassSupported = MarmaladeCompass::_isSupported();
	bTouchPadSupported = MarmaladeTouchPad::_isSupported();
	
	// Parse whatever extra settings were sent to us
	_parseConfigSettings( paramList );	
}

//--------------------------------------------------------------------------------//
void MarmaladeInputManager::_parseConfigSettings( ParamList &paramList )
{
	
}

//----------------------------------------------------------------------------//
DeviceList MarmaladeInputManager::freeDeviceList()
{
	DeviceList ret;

	if( bAccelerometerSupported == true && bAccelerometerUsed == false )
		ret.insert(std::make_pair(OISJoyStick, mInputSystemName));

	if( bMultiTouchSupported == true && bMultiTouchUsed == false )
		ret.insert(std::make_pair(OISMultiTouch, mInputSystemName));
		
	if( bTouchPadSupported == true && bTouchPadUsed == false )
		ret.insert(std::make_pair(OISTouchPad, mInputSystemName));
		
	if( bCompassSupported == true && bCompassUsed == false )
		ret.insert(std::make_pair(OISCompass, mInputSystemName));
		
	return ret;
}

//----------------------------------------------------------------------------//
int MarmaladeInputManager::totalDevices(Type iType)
{
	switch(iType)
	{
	//case OISKeyboard: return 1;
	case OISMultiTouch: return bMultiTouchSupported ? 1 : 0;
	case OISJoyStick: return bAccelerometerSupported ? 1 : 0;
	case OISCompass: return bCompassSupported ? 1 : 0;
	case OISTouchPad: return bTouchPadSupported ? 1 : 0;
	default: return 0;
	}
}

//----------------------------------------------------------------------------//
int MarmaladeInputManager::freeDevices(Type iType)
{
	switch(iType)
	{
	//case OISKeyboard: return keyboardUsed ? 0 : 1;
	case OISMultiTouch: return bMultiTouchUsed ? 0 : 1;
	case OISJoyStick: return bAccelerometerUsed ? 0 : 1;
	case OISCompass: return bCompassUsed ? 0 : 1;
	case OISTouchPad: return bTouchPadUsed ? 0 : 1;
	default: return 0;
	}
}

//----------------------------------------------------------------------------//
bool MarmaladeInputManager::vendorExist(Type iType, const std::string & vendor)
{
	if( ( iType == OISMultiTouch || iType == OISJoyStick || iType == OISCompass || iType == OISTouchPad) 
		&& vendor == mInputSystemName )
		return true;

	return false;
}

//----------------------------------------------------------------------------//
Object* MarmaladeInputManager::createObject(InputManager* creator, Type iType, bool bufferMode, const std::string & vendor)
{
	Object *obj = 0;

	switch(iType)
	{
	/*
	case OISKeyboard: 
	{
		
		break;
	}
	*/
	case OISMultiTouch:
	{
		if( bMultiTouchSupported == true && bMultiTouchUsed == false )
		{
			obj = new MarmaladeMultiTouch(this, true);
		}
		break;
	}
	case OISJoyStick:
	{
		if( bAccelerometerSupported == true && bAccelerometerUsed == false )
		{
			obj = new MarmaladeAccelerometer(this, true);
		}
		break;
	}
	case OISTouchPad:
	{
		if( bTouchPadSupported == true && bTouchPadUsed == false )
		{
			obj = new MarmaladeTouchPad(this, true);
		}
		break;
	}
	case OISCompass:
	{
		if( bCompassSupported == true && bCompassUsed == false )
		{
			obj = new MarmaladeCompass(this, true);
		}
		break;
	}
	default:
		break;
	}

	if( obj == 0 )
		OIS_EXCEPT(E_InputDeviceNonExistant, "No devices match requested type.");

	return obj;
}

//----------------------------------------------------------------------------//
void MarmaladeInputManager::destroyObject(Object* obj)
{
	delete obj;
}

//----------------------------------------------------------------------------//
int32 MarmaladeInputManager::_touchCallback(s3ePointerTouchEvent* systemData, MarmaladeMultiTouch* touchObject)
{
	if(touchObject)
	{
		if(systemData->m_Pressed)
			touchObject->_touchBegan(systemData);
		else
			touchObject->_touchEnded(systemData);
	}
	
	return 0;
}

//----------------------------------------------------------------------------//
int32 MarmaladeInputManager::_touchMotionCallback(s3ePointerTouchMotionEvent* systemData, MarmaladeMultiTouch* touchObject)
{
	if(touchObject)
	{
		touchObject->_touchMoved(systemData);
	}
	
	return 0;
}

//----------------------------------------------------------------------------//
int32 MarmaladeInputManager::_touchPadCallback(s3eTouchpadEvent* systemData, MarmaladeTouchPad* touchPad)
{
	if(touchPad)
	{
		if(systemData->m_Pressed)
			touchPad->_touchBegan(systemData);
		else
			touchPad->_touchEnded(systemData);
	}
	
	return 0;
}

//----------------------------------------------------------------------------//
int32 MarmaladeInputManager::_touchPadMotionCallback(s3eTouchpadMotionEvent* systemData, MarmaladeTouchPad* touchPad)
{
	if(touchPad)
	{
		touchPad->_touchMoved(systemData);
	}
	
	return 0;
}
