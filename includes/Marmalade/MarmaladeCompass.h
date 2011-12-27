#ifndef OIS_MarmaladeCompass_H
#define OIS_MarmaladeCompass_H

namespace OIS
{
	class MarmaladeCompass : public JoyStick
	{
	public:
		MarmaladeCompass(InputManager* creator, bool buffered);
		~MarmaladeCompass();
		
		/** @copydoc Object::setBuffered */
		virtual void setBuffered(bool buffered);
		
		/** @copydoc Object::capture */
		virtual void capture();

		/** @copydoc Object::queryInterface */
		virtual Interface* queryInterface(Interface::IType type) { return 0; }

		/** @copydoc Object::_initialize */
		virtual void _initialize();
		
		//! 
		static bool _isSupported();
	};
}

#endif	// OIS_MarmaladeCompass_H