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
 
#include "Marmalade/MarmaladeCompass.h"
#include "Marmalade/MarmaladeInputManager.h"

using namespace OIS;

MarmaladeCompass::MarmaladeCompass(InputManager* creator, bool buffered)
	: JoyStick(creator->inputSystemName(), buffered, 0, creator)
{
	MarmaladeInputManager* man = static_cast<MarmaladeInputManager*>(creator);
	man->_setCompassUsed(true);
}

MarmaladeCompass::~MarmaladeCompass()
{
	// Stop accelerometer
	s3eCompassStop();
	
	MarmaladeInputManager *man = static_cast<MarmaladeInputManager*>(mCreator);
    man->_setCompassUsed(false);
}

void MarmaladeCompass::_initialize()
{
	// Clear old joy state
    mState.mVectors.resize(1);
	mState.clear();
	mTempState.clear();
	
	// Start accelerometer
	if(	s3eCompassStart() != S3E_RESULT_SUCCESS )
	{
	
	}
}

void MarmaladeCompass::setBuffered(bool buffered)
{
	mBuffered = buffered;
}

void MarmaladeCompass::capture()
{
	mState.clear();
	
	s3eCompassHeading heading = { 0, 0, 0 };
	if(s3eCompassGetHeading(&heading) == S3E_RESULT_SUCCESS)
	{
		mState.mVectors[0].x = heading.m_X;
		mState.mVectors[0].y = heading.m_Y;
		mState.mVectors[0].z = heading.m_Z;
		
		if(mListener && mBuffered)
			mListener->axisMoved(JoyStickEvent(this, mState), 0);
	}
}

bool MarmaladeCompass::_isSupported()
{
	return (s3eCompassAvailable() != S3E_FALSE);
}