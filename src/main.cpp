#include "ofMain.h"
#include "ofxGif.h"
#include "ofxMd2.h"
#include "ofxGui.h"

class xApp : public ofBaseApp {
	public:
		ofxPanel gui;
		ofxSlider<int> tz;
                ofxGIF::fiGifLoader gifloader;

		ofImage color,mat;
		ofShader lamb1,splay;
		int w,h;
		int posPlay=500;
		int LposPlay=0;
                ofxMd2 rat,weap;
		bool animstepR=true;
		bool animstepL=true;
		bool animjump=true;
		float radius;
		float xp,yp;
		ofRectangle zonePlay;
		vector<ofRectangle> zoneEnemy;
		int salto = 0;
		int pos=0;

		void setup(){
                        ofSetVerticalSync(true);
			ofSetFrameRate(60);

			gui.setup();
			gui.add(tz.setup("transZ",-5000,0,5000));
			w=ofGetScreenWidth();
			h=ofGetScreenHeight();

			rat.loadModel("ofMd2/src/ratamahatta.md2","",180,23);
                        weap.loadModel("ofMd2/src/weapon.md2","",180,23);

			lamb1.load("shaders/lamb2.vert","shaders/lamb2.frag");
			splay.load("shaders/sem.vert","shaders/sem.frag");

			ofDisableArbTex();
			color.loadImage("img/bg.jpg");
			mat.loadImage("mat/matcapblack1.jpeg");
			salto=240;
		        gifloader.load("enemy/anim_trasp.gif");
		}

		bool insidePlayer(int x, int y){
		    if(zonePlay.inside(x, y)) {
			  return true;
		    }
		    return false;
		}

		bool insideEnemy(int x, int y){
			for( int i = 0; i < zoneEnemy.size(); i++ ){
				if(zoneEnemy[i].inside(x,y)){
					return true;
				}
			}
			return false;
		}

		void cmd(){
			/* ----------------- */
			if(ofGetKeyPressed(OF_KEY_LEFT)){
    				 posPlay-=4; LposPlay-=14;
				 if(posPlay<500) posPlay=500;
				 if(LposPlay<20) LposPlay=20;
				 if( animstepL == true && weap.getAnim() != 13 ){
                                        animstepL=false;
					weap.setAnim(13); rat.setAnim(13);
				 }				
			} 
			else{
				if( animstepL == false && weap.getAnim() != 0 ){
					animstepL=true;
					weap.setAnim(0); rat.setAnim(0);
				}
			}
			if(ofGetKeyPressed(OF_KEY_RIGHT)){
				 posPlay+=4; LposPlay+=14;
                                 if(posPlay>w-500) posPlay=w-500;
                                 if(LposPlay>w-20) LposPlay=w-20;
				 if( animstepR == true && weap.getAnim() != 1 ){
                                        animstepR=false;
					weap.setAnim(1); rat.setAnim(1);
				 }				
		        }
			else{
				if( animstepR == false && weap.getAnim() != 0 ){
					animstepR=true;
					weap.setAnim(0); rat.setAnim(0);
				}
			}

			if(ofGetKeyPressed(' ')){
				 if(animjump==true) {
					weap.setAnim(6); rat.setAnim(6);
					animjump=false;
					salto=330;
				 }
			}
			else{
				 if(animjump==false) {
					weap.setAnim(0); rat.setAnim(0);
					animjump=true;
					salto=240;
				 }
			}
		}

		int posEnemy=w+10;

               	void update(){
			ofSetWindowTitle(ofToString(ofGetFrameRate()));
			/* animazione enemy */
			posEnemy-=5;
			if(posEnemy<-300) 
				posEnemy=w+100;
			

			/*collisione con player*/
                        if(insidePlayer( posEnemy, h-300)){
				cout<<"enemy touch player!!!!"<<endl;
			}
			if (ofGetElapsedTimeMillis() % 3) {
		                pos++;
		                if (pos > gifloader.pages.size()-1) pos = 0;
		        }
			cmd();
		}

		void draw(){
			ofBackgroundGradient(255,0);
			float DEFAULT_LIGHT_Z = 1.;
			float x  = posPlay / (float)w;
			float xL = LposPlay / (float)w;
			float y  = ((h/2)-300) / (float)h;

			/* background */
			lamb1.begin();
			lamb1.setUniformTexture("u_texture",color.getTextureReference(),0);
			lamb1.setUniform2f("resImage",color.getWidth(),color.getHeight());
			lamb1.setUniform2f("Resolution",w,h);
			lamb1.setUniform3f("LightPos",xL,y,DEFAULT_LIGHT_Z);
			lamb1.setUniform3f("sColor",0.6,0.5,0.1);
			color.draw(0,0);
			lamb1.end();

			/* fake shadow */
			ofPushStyle();
			ofFill();
			float movex=0;
			ofSetColor(0,0,0,140);
			if(rat.getAnim()>0) {
				radius+=0.5;
	                        if(radius>5.5)
					radius=0.;
			} else { 
				radius=10.5;
                                movex=30;
			}
			ofEllipse( LposPlay+movex, h-40, 130+radius, 60+radius);
			ofPopStyle();

			/* marker collisione player */
			//ofPushMatrix();
			//ofNoFill();
			//ofSetColor(255,0,0);
		        //zonePlay.set( LposPlay, h-salto, 120, 190 );
			//ofRect(zonePlay);
			//ofPopMatrix();

			/* player */
		        splay.begin();
		        splay.setUniformTexture("tMatCap", mat, 1);
  			gamer(false);
			splay.end();
			
			/* nemico */
			//zoneEnemy[0].set(posEnemy, h-70, 0, 45 );
			//ofRect(zoneEnemy[0]);
		        gifloader.pages[pos].draw( posEnemy, h-300 );
		}

		void gamer(bool wire){
			ofPushMatrix();
		        ofTranslate(posPlay,h-310,475);
		        glRotatef( 95, 0.0, 1.0, 0.0 );
		        glRotatef( 165, 1.0, 0.0, 0.0 );
       			if(wire) {
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				rat.draw();
		                weap.draw();
			}
			else{
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				rat.draw();
		                weap.draw();
			}
			ofPopMatrix();
		}

		void keyReleased(int key){

		}

		void keyPressed(int key){
			if(key == 'f')	
				ofToggleFullscreen();
                        switch(key){
                                case '0': weap.setAnim(0); rat.setAnim(0); break;
                                case '1': weap.setAnim(1); rat.setAnim(1); break;
                                case '2': weap.setAnim(2); rat.setAnim(2); break;
                                case '3': weap.setAnim(3); rat.setAnim(3); break;
                                case '4': weap.setAnim(4); rat.setAnim(4); break;
                                case '5': weap.setAnim(5); rat.setAnim(5); break;
                                case '6': weap.setAnim(6); rat.setAnim(6); break;
                                case '7': weap.setAnim(7); rat.setAnim(7); break;
                                case '8': weap.setAnim(8); rat.setAnim(8); break;
                                case '9': weap.setAnim(9); rat.setAnim(9); break;
				case 'a': weap.setAnim(10); rat.setAnim(10); break;
                                case 's': weap.setAnim(11); rat.setAnim(11); break;
                                case 'd': weap.setAnim(12); rat.setAnim(12); break;
				case 'z': weap.setAnim(13); rat.setAnim(13); break;
                                case 'x': weap.setAnim(14); rat.setAnim(14); break;
                                case 'c': weap.setAnim(15); rat.setAnim(15); break;
				case 'v': weap.setAnim(16); rat.setAnim(16); break;
                                case 'b': weap.setAnim(17); rat.setAnim(17); break;
                                case 'n': weap.setAnim(18); rat.setAnim(18); break;
				case 'm': weap.setAnim(19); rat.setAnim(19); break;
				case 'q': weap.setAnim(20); rat.setAnim(20); break;
				case 'w': weap.setAnim(21); rat.setAnim(21); break;
                        }
		}
};

int main( ){
	ofSetupOpenGL( 1280, 720, OF_FULLSCREEN);
	ofRunApp(new xApp());
}
