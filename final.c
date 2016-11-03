// Michael McRoskey
// SpitWad Willy Simulation

#include <stdio.h>
#include <math.h>
#define ANGLE 0.017453
#include "gfx5.h"

void gradient(double x, double y, double wd, double ht, double r1, double g1, double b1, double r2, double g2, double b2);
void background(int wd, int ht, int baseline);
void displayCharacter (int x_willy, int willy[26][15]);
void baselinerec (int x, int baseline, int width, int height);
void drawbullet (int x, int y);
void startscreen(int wd, int ht);
void loading(int wd, int ht);

struct Balls {
	int mass;
	double x, y, vx, vy, ax, ay, r;
};

int main(){
	
	// Declarations
	int wd=800, ht=600;  				// Window dimensions 
	double baseline =  5*ht/6;			// Where the "ground" is
	int margin = 20;					// Ensure Willy doesn't hit wall
	
  	int loop=1;							// Boolean truth
	
  	char c;								// Keyboard-entered character
	
	int x_willy = wd/2;					// Initial position
	int i = 1, b = 1, d = 1, u = 0;
	int hit = 0;
	int bullet[50][2] = {0};
	
	double t = 1.0000;
	
	struct Balls ball;
	
	ball.mass = 0.8;
	ball.x = wd/2;
	ball.y = ht/4;
	ball.vx = 4;
	ball.vy = 0;
	ball.ax = 0;
	ball.ay = 9.81 * ball.mass;
	ball.r = 30;
	
	ball.vx = ball.vx + 10*cos(90*ANGLE);
	ball.vy = ball.vy + 10*sin(90*ANGLE);

	// 2D Array with color values for Willy
	int willy[26][15] = {
	  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0  },
	  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0  },
	  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0  },
	  {0,0,0,0,0,0,0,2,2,0,0,0,0,0,0  },
	  {0,0,0,0,0,0,0,3,0,2,5,0,0,0,0  },
	  {0,0,0,0,0,0,3,3,3,0,5,5,0,0,0  },
	  {0,0,0,0,0,1,3,3,3,1,5,5,5,0,0  },
	  {0,0,0,0,0,3,1,3,1,3,0,5,5,5,0  },
	  {0,0,0,0,0,3,3,4,3,3,0,0,5,5,5  },
	  {0,0,0,0,0,3,1,3,1,3,0,5,5,5,0  },
	  {0,0,0,0,5,5,1,1,1,5,5,5,5,0,0  },
	  {0,0,5,5,5,5,3,3,3,5,5,5,0,0,0  },
	  {0,5,5,5,5,5,5,3,5,5,5,0,0,0,0  },
	  {5,5,5,6,5,5,5,5,5,5,5,0,0,0,0  },
	  {0,5,5,5,5,5,5,5,5,5,5,0,0,0,0  },
	  {0,0,5,5,5,5,5,5,5,5,5,0,0,0,0  },
	  {0,0,2,2,7,7,7,7,7,7,7,0,0,0,0  },
	  {0,0,0,2,8,8,8,8,8,8,8,0,0,0,0  },
	  {0,0,2,0,8,8,8,8,8,8,8,0,0,0,0  },
	  {0,0,0,0,8,8,8,8,8,8,8,0,0,0,0  },
	  {0,0,0,8,8,8,8,8,8,8,8,8,0,0,0  },
	  {0,0,0,8,8,8,8,8,8,8,8,8,0,0,0  },
	  {0,0,0,8,8,8,8,0,8,8,8,8,0,0,0  },
	  {0,0,0,9,9,9,3,0,3,9,9,9,0,0,0  },
	  {0,3,9,9,9,9,3,0,3,9,9,9,9,3,0  },
	  {0,9,9,9,9,4,4,0,4,4,9,9,9,9,0  }
	};
	
	// Open window
  	gfx_open(wd, ht, "Final.c");  		// Open a new window for drawing.
	gfx_clear_color(20,20,20);			// Background color

	// Start Screen
	while(loop) {
		gfx_clear();
		startscreen(wd, ht);
		displayCharacter (360, willy);
		
		gfx_color(52, 66, 60);
		gfx_changefont( "-misc-fixed-medium-r-normal--14-130-75-75-c-70-iso8859-1" );
		
		if (i == 1){ // Only activates on the first iteration
			loading(wd, ht);
		}
		
		if(i%25 == 0){ // Blinking
			gfx_text( 0.39*wd,  0.5*ht, "[Press enter to play]" );
			gfx_flush();
			usleep(1000000);
		}
		
		if(gfx_event_waiting()) { 
			c = gfx_wait();
			if (c == 13){ break; } // Enter breaks the loop
		}

		gfx_flush();
		usleep(10000);
		i++;
			
	}
	
	i = 1; // Redefine i

	// Game play
	while(loop) {
	    gfx_clear();
		
		// Each loop, clear the screen, redraw the background, Willy, and the ball
		background(wd, ht, baseline);
		displayCharacter (x_willy, willy);
		
		// Calculate the ball position based on time t
		ball.x = ball.x + ball.vx*(t) + 0.5*(ball.ax)*(t*t);
		ball.y = ball.y + ball.vy*(t) + 0.5*(ball.ay)*(t*t);
		
		gfx_color(20,143,116);
		gfx_fill_circle((int)ball.x, (int)ball.y, (int)ball.r);
		
		// Pause for instructions
		if(i == 1){
			gfx_color(20, 20, 20);
			gfx_text( 0.38*wd,  0.5*ht, "Use the arrow keys and spacebar!" );
			gfx_flush();
			usleep(5000000);
		}
		
		// Boundary Detection
		// Right
		if (ball.x >= wd-ball.r){				// If out of bounds, reset by flipping direction
			ball.x = wd-ball.r;
			ball.vx = -ball.vx;
			ball.ax = -ball.ax;
			hit++;
		}
		// Baseline
		if (ball.y >= baseline-ball.r){
			ball.y = baseline-ball.r;
			ball.vy = -ball.vy;
			ball.ay = -ball.ay;
			hit++;
		}
		// Left
		if (ball.x <= ball.r) {
			ball.x = ball.r;
			ball.vx = -ball.vx;
			ball.ax = -ball.ax;
			hit++;
		}
		// Top
		if (ball.y <= ball.r) {
			ball.y = ball.r;
			ball.vy = -ball.vy;
			ball.ay = -ball.ay;
			hit++;
		}
		
		// Each hit increases acceleration 
		for (i=1; i<hit; i++){
			ball.ay+=0.075;
		}
		
		// Stop the ball after it has hit for a while
		if(hit > 30){
			t = 0; // Stop time
			gfx_color(47, 60, 50);
			gfx_changefont( "-sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1");
			gfx_text( 0.38*wd,  0.935*ht, "0" );
			gfx_text( 0.45*wd,  0.935*ht, "YOU LOSE!" );
		}
		
		// If the ball hits Willy, game over
		if (ball.x < x_willy+30 && ball.x > x_willy-30 && ball.y > 400){
			while (1) {
				gfx_color(47, 60, 50);
				gfx_changefont( "-sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1");
				gfx_text( 0.38*wd,  0.935*ht, "0" );
				gfx_text( 0.5*wd,  0.935*ht, "YOU LOSE!" );
				if(gfx_event_waiting()) { 
					c = gfx_wait();
					if (c == 'q'){ return 0; }
				}
			}
		}
		
		// Update bullets in each while loop
		for (b = 1; b<50; b++){
			bullet[b][2] -= 4;
			drawbullet(bullet[b][1], bullet[b][2]);
			if (bullet[b][1] < ball.x+ball.r && bullet[b][1] > ball.x-ball.r && bullet[b][2] < ball.y+ball.r && bullet[b][2] > ball.y-ball.r ){
				ball.r = 0;
				// Score
			
				while (1) { // Stop the game
					gfx_color(47, 60, 50);
					gfx_changefont( "-sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1");
					gfx_text( 0.38*wd,  0.935*ht, "50000" );
					gfx_text( 0.5*wd,  0.935*ht, "YOU WIN!" );
					if(gfx_event_waiting()) { 
						c = gfx_wait();
						if (c == 'q'){ return 0; }
					}
				}
			}
		}
				
		if(gfx_event_waiting()) { 
	      	c = gfx_wait();
			if (c == 'S'){ x_willy += 10; } // Right
			if (c == 'Q'){ x_willy -= 10; } // Left
			//if (c == 'R'){ y[1] += 5; } // Up
			//if (c == 'T'){ y[1] -= 0; } // Down
			
			if (c == 32){
				
				// Spacebar initializes a new bullet in bullet array
				// with x position (y will always increase)
				d++;
				bullet[d][1] = x_willy+30;
				bullet[d][2] = 400;
				if (d>50) d = 1;
			
			}  // Spacebar
			
			// Willy's bounds
			if (x_willy < 0){ x_willy = margin; }
			if (x_willy > wd-60 ){ x_willy = wd-60;}
			// Quit
	      	if (c == 'q'){ break; }
	    }
		
		gfx_flush();
		usleep(10000);					// Pause for smooth animation
		t += 0.00001;					// Increment t each loop
		i++;							// i will not be 1 again
		ball.r *= 0.999;				// Ball decreases radius each loop
	}
 	return 0;
}

// Creates a gradient from one rgb value to another using a uniform distribution
void gradient(double x, double y, double wd, double ht, double r1, double g1, double b1, double r2, double g2, double b2){
	
	int rate = 1; // Frequency of rectangles
	int i;
	
	// Find rate of change
	double r_dist = r2-r1;
	double g_dist = g2-g1;
	double b_dist = b2-b1;
	
	double delta_r = r_dist/ht;
	double delta_g = g_dist/ht;
	double delta_b = b_dist/ht;
	
	// Uniform distribution
	for (i=0; i<ht; i++){
		gfx_color(r1, g1, b1);
		gfx_fill_rectangle(x, y, wd, rate);
		y += rate;
		r1 += delta_r*rate;
		g1 += delta_g*rate;
		b1 += delta_b*rate;
	}
	
}

// Draws the background scene (similar look to vintage "SpitWad Willy")
void background(int wd, int ht, int baseline){
	
	int x, topsoil = 10;
	double x1 = 0.25*wd, x2 = 0.5*wd, x3 = 0.75*wd, x4 = 1*wd;
	
	// Sky
	gradient(0, 0, wd, baseline, 102, 128, 112, 167, 187, 167);
//	Activate this in case gradient is too slow
//	gfx_color(167, 187, 167);
//	gfx_fill_rectangle(0, 0, wd, baseline);
	
	// Lines behind
	gfx_color(199, 221, 211);
	gfx_fill_rectangle(x3, 0.5*baseline, 0.2*wd, 6);
	gfx_fill_rectangle(x3, 0.65*baseline, 0.2*wd, 6);
	gfx_fill_rectangle(x3, 0.8*baseline, 0.2*wd, 6);
	
	// Dark structures
	gfx_color(100, 113, 105);
	baselinerec (0, baseline, x1-0, 449);
	baselinerec (x1, baseline, x2-x1, 402);
	
	// Light structures
	gfx_color(158, 175, 166);
	baselinerec (x2, baseline, x3-x2, 414);
	baselinerec (x3, baseline, x4-x3, 308);
	
	// Big Dots
	gfx_color(80, 126, 85);
	gfx_fill_arc( 0.2*wd, 0.3*baseline, 40, 30, 0, 360);
	gfx_color(80, 126, 85);
	gfx_fill_arc( 0.2*wd, 0.45*baseline, 40, 30, 0, 360);
	gfx_color(80, 126, 85);
	gfx_fill_arc( 0.2*wd, 0.6*baseline, 40, 30, 0, 360);
	gfx_color(80, 126, 85);
	gfx_fill_arc( 0.2*wd, 0.75*baseline, 40, 30, 0, 360);
	
	// Big Dots Shadow
	gfx_color(187, 206, 197);
	gfx_arc( 0.2*wd, 0.3*baseline, 40, 30, -115, 80);
	gfx_color(177, 196, 187);
	gfx_arc( 0.2*wd, 0.45*baseline, 40, 30, -115, 80);
	gfx_color(167, 186, 177);
	gfx_arc( 0.2*wd, 0.6*baseline, 40, 30, -115, 80);
	gfx_color(157, 176, 167);
	gfx_arc( 0.2*wd, 0.75*baseline, 40, 30, -115, 80);
	
	// Small Dots
	gfx_color(115, 129, 123);
	gfx_fill_arc( 0.45*wd, 0.3*baseline, 10, 7, 0, 360);
	gfx_fill_arc( 0.62*wd, 0.48*baseline, 10, 7, 0, 360);
	gfx_fill_arc( 0.51*wd, 0.63*baseline, 10, 7, 0, 360);
	gfx_fill_arc( 0.74*wd, 0.79*baseline, 10, 7, 0, 360);
	
	// Small Dots Shadow
	gfx_color(150, 170, 150);
	gfx_fill_arc( 0.45*wd, 0.3*baseline, 10, 7, -115, 80);
	gfx_fill_arc( 0.62*wd, 0.48*baseline, 10, 7, -115, 80);
	gfx_fill_arc( 0.51*wd, 0.63*baseline, 10, 7, -115, 80);
	gfx_fill_arc( 0.74*wd, 0.79*baseline, 10, 7, -115, 80);
		
	// Topsoil
	gfx_color(196, 182, 136);
	gfx_fill_rectangle(0, baseline, wd, topsoil);
	
	// Shadow
	gfx_color(147, 141, 96);
	gfx_line(0, baseline, wd, baseline);
	
	// Ground
	gfx_color(35, 45, 47);
	gfx_fill_rectangle(0, baseline+topsoil, wd, ht-(baseline+topsoil));
	
	// Light Shadows
	gfx_color(179, 201, 191);
	gfx_fill_rectangle(100, ht-502, 0.2*wd, 5);
	
	// Lines
	gfx_color(83, 88, 95);
	for (x=-100; x<wd; x+=10) {
		gfx_line(x, baseline+topsoil, ht-baseline + x, ht);
	}
	
	// Score Box
	gfx_color(150, 166, 151);
	gfx_fill_rectangle(wd/2 -117, baseline+18, 234, 68);
	gfx_color(178, 195, 178);
	gfx_fill_rectangle(wd/2 -110, baseline+23, 220, 58);
	
	// Text
	gfx_color(178, 195, 178);
	gfx_changefont( "-misc-fixed-medium-r-normal--20-200-75-75-c-100-iso8859-1" );
	gfx_text( 0.72*wd,  0.92*ht, "SpitWad Willy" );
	gfx_text( 0.26*wd,  0.93*ht, "SCORE:" );
	gfx_changefont( "-misc-fixed-medium-r-normal--14-130-75-75-c-70-iso8859-1" );
	gfx_text( 0.72*wd,  0.95*ht, "// Michael McRoskey" );

}

// Make a regtangle a certain height above the baseline
void baselinerec (int x, int baseline, int width, int height){
	
	gfx_fill_rectangle(x-(width/2), baseline-height, width, height);
	
}

// Read in character array and assign colors based on numbers
void displayCharacter (int x_willy, int willy[26][15]){

	int x, y;
	int r = 4; // Scaling factor - couldn't just do gfx_point, would be too small
	int color = 0;
	int shift = 400;
	for (x=0; x<26; x++) {
		for (y=0; y<15; y++) {
			if (willy[x][y] != 0){
				color = willy[x][y];
				if (color == 1) gfx_color(239, 239, 239); // Gray
				if (color == 2) gfx_color(252, 236, 205); // Skin
				if (color == 3) gfx_color(255, 255, 255); // White
				if (color == 4) gfx_color( 67,  67,  67); // Dark Gray
				if (color == 5) gfx_color( 65, 212,  68); // Green
				if (color == 6) gfx_color(121, 132, 117); // Dark Green
				if (color == 7) gfx_color( 16,  81, 135); // Dark Blue
				if (color == 8) gfx_color( 59, 152, 230); // Blue
				if (color == 9) gfx_color(203,  65,   1); // Red
				gfx_fill_rectangle(r*y-1+x_willy, r*x-1+shift, r, r);
			}
		}
	}
	
	
}

// Draws a bullet given an x and y
void drawbullet (int x, int y){
	gfx_color(254, 203, 67);
	if (x!=0 && y!=0){ // Won't draw bullets that haven't been called yet (array initiallized to zero)
		gfx_fill_circle(x, y, 5);	
	}
}

// Draws the startup screen
void startscreen(int wd, int ht){
	
	gradient(0, 0, wd, ht, 102, 128, 112, 167, 187, 167);
	gfx_changefont( "-sony-fixed-medium-r-normal--24-170-100-100-c-120-iso8859-1");
	gfx_color(52, 66, 60);
	gfx_text( 0.38*wd,  0.9*ht, "SpitWad Willy!" );
	
	gfx_changefont( "-misc-fixed-medium-r-normal--14-130-75-75-c-70-iso8859-1" );
	gfx_color(100, 130, 100);
	gfx_text( 0.37*wd,  0.93*ht, "(c) 2015 Michael McRoskey" );
	
}

void loading(int wd, int ht){
	
	int x;
	
	gfx_rectangle(280,ht/2,220,8);
	gfx_text( 0.45*wd,  0.54*ht, "LOADING" );
	// Build line
	for (x=280; x<500; x++) {
		gfx_line(x, ht/2, x, ht/2 +8);
		gfx_flush();
		usleep(10000);
	}
	// Pause rectangle before moving on
	gfx_fill_rectangle(280,ht/2,220,8);
	gfx_flush();
	usleep(1000000);
	
}