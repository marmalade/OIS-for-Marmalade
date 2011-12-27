#ifndef OIS_MarmaladeAccelerometer_H
#define OIS_MarmaladeAccelerometer_H

namespace OIS
{
	class MarmaladeAccelerometer : public JoyStick
	{
	public:
		MarmaladeAccelerometer(InputManager* creator, bool buffered);
		~MarmaladeAccelerometer();
		
		/** @copydoc Object::setBuffered */
		virtual void setBuffered(bool buffered);
		
		/** @copydoc Object::capture */
		virtual void capture();

		/** @copydoc Object::queryInterface */
		virtual Interface* queryInterface(Interface::IType type) { return 0; }

		/** @copydoc Object::_initialize */
		virtual void _initialize();
	};
}

#endif	// OIS_MarmaladeAccelerometer_H