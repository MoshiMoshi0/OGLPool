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
			bool bState[ sf::Keyboard::KeyCount ];
			bool bPressed[ sf::Keyboard::KeyCount ];
			bool bReleased[ sf::Keyboard::KeyCount ];
		private:
			sf::Window* window;
			sf::Vector2i mousePos, oldMousePos, mouseDisp;

			Input(){}
			void updateImpl(){
				mousePos = sf::Mouse::getPosition( *window );
				mouseDisp = mousePos - oldMousePos;
				oldMousePos = mousePos;

				for( int i = 0; i < sf::Keyboard::KeyCount; i++ ){
					bool current = isKeyPressed( (Key)i );
					bPressed[ i ] = !bState[ i ] && current;
					bReleased[ i ] = bState[ i ] && !current;
					bState[ i ] = current;
				}
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

			static bool onKeyPressed(Key key){ return instance->bPressed[ key ]; }
			static bool onKeyReleased(Key key){ return instance->bReleased[ key ]; }
		};
	}
} /* namespace OGLPool */
