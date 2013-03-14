/*
 * InputHandler.h
 *
 *  Created on: 14-03-2013
 *      Author: _CORE7
 */

#include <SFML/Window.hpp>

namespace OGLPool {
	namespace IO {
		class Input {
		protected:
			static Input* instance;
		private:
			sf::Window* window;
			sf::Vector2i mousePos, oldMousePos, mouseDisp;

			Input(){}
			void updateImpl(){
				mousePos = sf::Mouse::getPosition( *(instance->window) );
				mouseDisp = mousePos - oldMousePos;
				oldMousePos = mousePos;
			}
		public:
			Input( Window* window ){ this->window = window; }
			static void init( Window* window ){
				instance = new Input( window );
			}

			static void update(){ instance->updateImpl(); }
			static int getMouseX(){ return instance->mousePos.x; }
			static int getMouseY(){ return instance->mousePos.y; }
			static sf::Vector2i getMousePosition(){ return instance->mousePos; }

			static int getMouseDX(){ return instance->mouseDisp.x; }
			static int getMouseDY(){ return instance->mouseDisp.y; }
			static sf::Vector2i getMouseDisplacement(){ return instance->mouseDisp; }

			static bool isKeyPressed( sf::Keyboard::Key key ){ return sf::Keyboard::isKeyPressed( key ); }
		};

		Input* Input::instance = 0;
	}
} /* namespace OGLPool */
