/* 2012,2013 Nils Stec <nils.stec@gmail.com> 
 *
 * v0.1 - added Xlib support to get screen resolution
 *
 * */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <X11/Xlib.h>

uint32_t x, y, count = 0;
char inbuf[256], pass[256];

int32_t sdltask() {
	uint8_t key;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				break;
			case SDL_KEYDOWN:			// key pressed
				key = event.key.keysym.sym;
				switch(key) {			
					case SDLK_ESCAPE:	// restart password typing, delete already entered chars
					case SDLK_RETURN:
						inbuf[count] = '\0';	// terminate string
						count = 0;
						goto out;
						break;
				}
				inbuf[count] = key;		// keypress wasn't RETURN or ESCAPE, so add char to string
				count++;
				inbuf[count] = '\0';		// terminate string
				if(!strcmp(pass, inbuf)) exit(0);	// check if string == password, if so exit sdlock
out:
				break;
			case SDL_QUIT: // we cant leave on SDL_QUIT event, whole sdlock would be a farce ;)
     				break; // so simply do NOTHING here
     		}
    }
    return 1;
}

int format_time(char *buffer) {
	time_t now = time(NULL);
	struct tm *localtm = localtime(&now);

	sprintf(buffer, "locked - %02d:%02d:%02d - %02d.%02d.%04d",
			localtm->tm_hour, localtm->tm_min, localtm->tm_sec,
			localtm->tm_mday, localtm->tm_mon+1,
			localtm->tm_year+1900
	);
	return 0;
}

int x11_screenres(int *x, int *y) {
	Display* pdsp = XOpenDisplay(NULL);
	Window wid = DefaultRootWindow(pdsp);
	
	XWindowAttributes xwAttr;
	XGetWindowAttributes(pdsp,wid,&xwAttr);

	*x = xwAttr.width;
	*y = xwAttr.height;

	XCloseDisplay(pdsp);

	return 0;
}



int main(int argc, char **argv) {
	SDL_Surface *surface;
	char buf[256];
	uint32_t old_x = 0xfffffa, old_y = 0xfffffa;
	int text_x, text_y;
	int usecs = 0;
    	
	int x11win_width;
	int x11win_height;

	x11_screenres(&x11win_width,&x11win_height);

//	printf("%d %d\n", x11win_width, x11win_height);

	SDL_Init(SDL_INIT_VIDEO);
	surface = SDL_SetVideoMode(x11win_width, x11win_height, 16, 
			SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN
	);

	text_x = x11win_width - (31*8);
	text_y = x11win_height - 15;

	sprintf(pass, "%s", argv[1]);		// get password from cmd line	
	// TODO change to hash instead of plain text password !!!!

	sprintf(buf, "%s/.sdlock.png", getenv("HOME"));
	SDL_Surface *cursor = IMG_Load(buf);
	SDL_ShowCursor(0);
	SDL_Rect dst;
	dst.w = cursor->w;
	dst.h = cursor->h;

	while(sdltask()) {
		if( (old_x != x) || (old_y != y) || (usecs > 500000) ) {
        		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

        		dst.x = x;
        		dst.y = y;
        		format_time(buf);
			stringRGBA(surface, text_x, text_y, buf, 0xaa, 0xaa, 0xaa, 0xbb);
        	
        		SDL_BlitSurface(cursor, NULL, surface, &dst);
        		SDL_Flip(surface);

			old_x = x;
			old_y = y;
			usecs = 0;
		}
		usleep(20000);
		usecs += 20000;
    	}

	SDL_FreeSurface(surface);
	SDL_Quit();
	return 0;  
}
