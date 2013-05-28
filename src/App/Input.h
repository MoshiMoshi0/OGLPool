/*
 * InputHandler.h
 *
 *  Created on: 14-03-2013
 *      Author: _CORE7
 */

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
using namespace glm;
using namespace sf;

using namespace sf;
namespace OGLPool {
	namespace IO {
		class Input : public Keyboard, public Mouse {
		protected:
			static Input* instance;

			bool kState[ Keyboard::KeyCount ];
			bool kPressed[ Keyboard::KeyCount ];
			bool kReleased[ Keyboard::KeyCount ];

			bool bState[ Mouse::ButtonCount ];
			bool bPressed[ Mouse::ButtonCount ];
			bool bReleased[ Mouse::ButtonCount ];

			Window* window;
			ivec2 mousePos, oldMousePos, mouseDisp;
			bool hasFocus;

			Input(){}
			void updateImpl(){
				if( !hasFocus ) return;

				mousePos = getPosition( *window );
				mouseDisp = mousePos - oldMousePos;
				oldMousePos = mousePos;

				for( int i = 0; i < Keyboard::KeyCount; i++ ){
					bool current = isKeyPressed( (Key)i );
					kPressed[ i ] = !kState[ i ] && current;
					kReleased[ i ] = kState[ i ] && !current;
					kState[ i ] = current;
				}

				for( int i = 0; i < Mouse::ButtonCount; i++ ){
					bool current = isButtonPressed( (Button)i );
					bPressed[ i ] = !bState[ i ] && current;
					bReleased[ i ] = bState[ i ] && !current;
					bState[ i ] = current;
				}
			}
		public:
			Input( Window* window ){
				this->window = window;
				mousePos = oldMousePos = getPosition( *window );
			}

			static void init( Window* window ){ instance = new Input( window ); }
			static void update(){ instance->updateImpl(); }

			static bool isKeyPressed( Key key ){ return instance->hasFocus && Keyboard::isKeyPressed(key); }
			static bool onKeyPressed(Key key){ return instance->kPressed[ key ]; }
			static bool onKeyReleased(Key key){ return instance->kReleased[ key ]; }

			static bool onButtonPressed(Button button){ return instance->bPressed[ button ]; }
			static bool onButtonReleased(Button button){ return instance->bReleased[ button ]; }

			static int getMouseX(){ return instance->mousePos.x; }
			static int getMouseY(){ return instance->mousePos.y; }
			static ivec2 getMousePosition(){ return ivec2(instance->mousePos.x, instance->mousePos.y); }
			static ivec2 getPosition(const Window& relativeTo){ Vector2i pos = Mouse::getPosition( relativeTo ); return ivec2( pos.x, pos.y ); }
			static int getMouseDX(){ return instance->mouseDisp.x; }
			static int getMouseDY(){ return instance->mouseDisp.y; }
			static ivec2 getMouseDisplacement(){ return ivec2(instance->mouseDisp.x, instance->mouseDisp.y); }

			static bool getFocus(){ return instance->hasFocus; }
			static void setFocus( bool focus ){
				instance->hasFocus = focus;
				if( focus ) instance->oldMousePos = instance->mousePos = instance->getPosition( *(instance->window) );
			}
			static vec3 getMouseRayDir(){
				int x = IO::Input::getMouseX();
				int y = IO::Input::getMouseY();

			    int viewport[4];
			    double modelview[16];
			    double projection[16];
			    double x0, y0, z0;
			    double x1, y1, z1;

			    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
			    glGetDoublev( GL_PROJECTION_MATRIX, projection );
			    glGetIntegerv( GL_VIEWPORT, viewport );

			    gluUnProject( x, viewport[3] - y, 0, modelview, projection, viewport, &x0, &y0, &z0);
			    gluUnProject( x, viewport[3] - y, 1, modelview, projection, viewport, &x1, &y1, &z1);

			    return normalize( vec3( x1 - x0, y1 - y0, z1 - z0 ) );
			}
		};
	}
} /* namespace OGLPool */
