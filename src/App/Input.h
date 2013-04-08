/*
 * InputHandler.h
 *
 *  Created on: 14-03-2013
 *      Author: _CORE7
 */

#include <SFML/Window.hpp>

using namespace sf;
namespace OGLPool {
	namespace IO {
		class Input : public sf::Keyboard {
		protected:
			static Input* instance;
		private:
			sf::Window* window;
			sf::Vector2i mousePos, oldMousePos, mouseDisp;
			bool hasFocus;

			Input(){}
			void updateImpl(){
				if( !hasFocus ) return;
				mousePos = sf::Mouse::getPosition( *window );
				mouseDisp = mousePos - oldMousePos;
				oldMousePos = mousePos;
			}
		public:
			Input( Window* window ){
				this->window = window;
				mousePos = oldMousePos = sf::Mouse::getPosition( *window );
			}
			static void init( Window* window ){
				instance = new Input( window );
			}

			static void update(){ instance->updateImpl(); }
			static int getMouseX(){ return instance->mousePos.x; }
			static int getMouseY(){ return instance->mousePos.y; }
			static Vector2i getMousePosition(){ return instance->mousePos; }

			static int getMouseDX(){ return instance->mouseDisp.x; }
			static int getMouseDY(){ return instance->mouseDisp.y; }
			static Vector2i getMouseDisplacement(){ return instance->mouseDisp; }
			static void setFocus( bool focus ){ instance->hasFocus = focus; }
		};
	}
} /* namespace OGLPool */
