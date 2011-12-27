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
#ifndef OIS_MarmaladeInputManager_H
#define OIS_MarmaladeInputManager_H

#include "OISInputManager.h"
#include "OISFactoryCreator.h"
#include <s3e.h>
#include "Marmalade/MarmaladePrereqs.h"

namespace OIS
{
	/**	MarmaladeInputManager specialization - Using S3E platform */
	class MarmaladeInputManager : public InputManager, public FactoryCreator
	{
	public:
		MarmaladeInputManager();
		virtual ~MarmaladeInputManager();

		//InputManager Overrides
		/** @copydoc InputManager::_initialize */
		void _initialize( ParamList &paramList );

		//FactoryCreator Overrides
		/** @copydoc FactoryCreator::deviceList */
		DeviceList freeDeviceList();

		/** @copydoc FactoryCreator::totalDevices */
		int totalDevices(Type iType);

		/** @copydoc FactoryCreator::freeDevices */
		int freeDevices(Type iType);

		/** @copydoc FactoryCreator::vendorExist */
		bool vendorExist(Type iType, const std::string & vendor);

		/** @copydoc FactoryCreator::createObject */
		Object* createObject(InputManager* creator, Type iType, bool bufferMode, const std::string & vendor = "");

		/** @copydoc FactoryCreator::destroyObject */
		void destroyObject(Object* obj);
		
		//Internal Items
		
		//! Internal method, used for flagging multi-touch as available/unavailable for creation
		void _setMultiTouchUsed(bool used) { bMultiTouchUsed = used; }

        //! Internal method, used for flagging accelerometer as available/unavailable for creation
		void _setAccelerometerUsed(bool used) { bAccelerometerUsed = used; }
		
		//! Internal method, used for flagging compass as available/unavailable for creation
		void _setCompassUsed(bool used) { bCompassUsed = used; }
		
		//! Internal method, used for flagging touchpad as available/unavailable for creation
		void _setTouchpadUsed(bool used) { bTouchpadUsed = used; }
		
		//! Callbacks
		static int32 _touchCallback(s3ePointerTouchEvent* systemData, MarmaladeMultiTouch* touchObject);
		static int32 _touchMotionCallback(s3ePointerTouchMotionEvent* systemData, MarmaladeMultiTouch* touchObject);
		static int32 _touchPadCallback(s3eTouchpadEvent* systemData, MarmaladeTouchPad* touchPad);
		static int32 _touchPadMotionCallback(s3eTouchpadMotionEvent* systemData, MarmaladeTouchPad* touchPad);
		
	protected:
		//! internal class method for dealing with param list
		void _parseConfigSettings( ParamList &paramList );

		//! Used to know if we support multitouch
		bool bMultiTouchSupported;
		bool bMultiTouchUsed;
		
		//! Used to know if we support accelerometer
		bool bAccelerometerSupported;
		bool bAccelerometerUsed;
		
		//! Used to know if we support compass
		bool bCompassSupported;
		bool bCompassUsed;
		
		//! Used to know if we support touchpad
		bool bTouchpadSupported;
		bool bTouchpadUsed;
	};
}
#endif
