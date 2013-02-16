#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>

int x, y;
int count = 0;
char inbuf[256];
char pass[256];

int sdltask();


int sdltask() {
	uint8_t key;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				break;
			case SDL_KEYDOWN:
				key = event.key.keysym.sym;
				switch(key) {			
					case SDLK_ESCAPE:
					case SDLK_RETURN:
						inbuf[count] = '\0';
						count = 0;
						goto out;
						break;
				}
				inbuf[count] = key;
				count++;
				inbuf[count] = '\0';
				if(!strcmp(pass, inbuf)) exit(0);
out:
				break;
			case SDL_QUIT: 
				return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
	SDL_Surface *surface;
	char buf[256];
	int text_x, text_y;
    	
	time_t now = time(NULL);
	struct tm *localtm = localtime(&now);
	char buffer[256];
	
	SDL_Init(SDL_INIT_VIDEO);
	surface = SDL_SetVideoMode(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), 
			SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN
	);
	
	sprintf(buf, "%s/.sdlock.png", getenv("HOME"));

	text_x = atoi(argv[4]);
	text_y = atoi(argv[5]);

	sprintf(pass, "%s", argv[6]);

	SDL_Surface *cursor = IMG_Load(buf);
	SDL_ShowCursor(0);
	SDL_Rect dst;
	dst.w = cursor->w;
	dst.h = cursor->h;

	while(sdltask()) {
        	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
        	dst.x = x;
        	dst.y = y;
	        now = time(NULL);
        	localtm = localtime(&now);

		sprintf(buffer, "locked - %02d:%02d:%02d - %02d.%02d.%04d", 
			localtm->tm_hour, localtm->tm_min, localtm->tm_sec,
			localtm->tm_mday, localtm->tm_mon+1, 
			localtm->tm_year+1900);

		stringRGBA(surface, text_x, text_y, buffer, 0xaa, 0xaa, 0xaa, 0xbb);
        	SDL_BlitSurface(cursor, NULL, surface, &dst);
        	SDL_Flip(surface);
    	}

	SDL_FreeSurface(surface);
	SDL_Quit();
	return 0;  
}

