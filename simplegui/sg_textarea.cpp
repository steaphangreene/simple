#include <GL/gl.h>

#include "sg_textarea.h"

#include <SDL/SDL_ttf.h>
static TTF_Font *font = NULL;


SG_TextArea::SG_TextArea(string mes,
	float red, float green, float blue,
	float tred, float tgreen, float tblue)
		: SG_Panel(red, green, blue) {
  message = mes;
  tx_r = tred;
  tx_g = tgreen;
  tx_b = tblue;
  xmargin = 0.0;
  ymargin = 0.0;

  char *fontfn = "fonts/Adventure Subtitles Normal.ttf";
  if(!font) {
    if(TTF_Init()) {
      fprintf(stderr, "ERROR: Unable to load font '%s' - %s\n", TTF_GetError());
      exit(1);
      }
    atexit(TTF_Quit);

    font = TTF_OpenFont(fontfn, 100);
    if(!font) {
      fprintf(stderr, "ERROR: Unable to load font '%s'!\n", fontfn);
      exit(1);
      }
    }
  }

SG_TextArea::~SG_TextArea() {
  TTF_CloseFont(font);
  font = NULL;
  }

bool SG_TextArea::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "TextArea/Handle: Button Down at (%f,%f)\n", x, y);
  return 0;	//This widget eats all events
  }

#include <math.h>

int nextpoweroftwo(int x)
{
        double logbase2 = log(x) / log(2);
        return int(round(pow(2,ceil(logbase2))));
}

bool SG_TextArea::Render() {
//  fprintf(stderr, "Rendering TextArea %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

//  SG_Panel::Render();

  glPushMatrix();
  glTranslatef(0.0, 0.0, 0.0625);		//Advance to next "layer"
  glScalef(1.0-xmargin, 1.0-ymargin, 1.0);	//Scale for margins

  SDL_Surface *initial;
  SDL_Surface *intermediary;
  int w,h;
  GLuint texture;
  SDL_Color bg_color = { (Uint8)(bg_r*255), (Uint8)(bg_g*255), (Uint8)(bg_b*255) };
  SDL_Color tx_color = { (Uint8)(tx_r*255), (Uint8)(tx_g*255), (Uint8)(tx_b*255) };
//  SDL_Color tx_color = { 255, 255, 255, 255 };

  /* Use SDL_TTF to render our text */
  initial = TTF_RenderText_Blended(font, message.c_str(), tx_color);

  if(!initial) {
    fprintf(stderr, "ERROR: Failed to render font: %s\n", TTF_GetError());
    exit(1);
    }

  /* Convert the rendered text to a known format */
  w = nextpoweroftwo(initial->w);
  h = nextpoweroftwo(initial->h);
        
  intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_FillRect(intermediary, NULL, SDL_MapRGB(intermediary->format,
	(Uint8)(bg_r*255), (Uint8)(bg_g*255), (Uint8)(bg_b*255)));

  SDL_BlitSurface(initial, 0, intermediary, 0);
        
  /* Tell GL about our new texture */
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
		GL_UNSIGNED_BYTE, intermediary->pixels );
        
  /* GL_NEAREST looks horrible, if scaled... */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       

  /* prepare to render our texture */
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glColor3f(1.0f, 1.0f, 1.0f);

  float xfactor = (float)(initial->w) / (float)(w);
  float yfactor = (float)(initial->h) / (float)(h);
        
//  glColor3f(bg_r, bg_g, bg_b);
  /* Draw a quad at location */
  glBegin(GL_QUADS);

  glTexCoord2f(0.0,	yfactor);
  glVertex3f(-1.0, -1.0, 0.0);

  glTexCoord2f(xfactor,	yfactor); 
  glVertex3f( 1.0, -1.0, 0.0);

  glTexCoord2f(xfactor,	0.0); 
  glVertex3f( 1.0, 1.0, 0.0);

  glTexCoord2f(0.0,	0.0); 
  glVertex3f(-1.0, 1.0,	0.0);

  glEnd();

  /* Bad things happen if we delete the texture before it finishes */
  glFinish();

  /* Clean up */
  SDL_FreeSurface(initial);
  SDL_FreeSurface(intermediary);
  glDeleteTextures(1, &texture);


//  glColor3f(bg_r, bg_g, bg_b);
//  glBegin(GL_QUADS);
//  glVertex3f(-1.0, -1.0, 0.0);
//  glVertex3f( 1.0, -1.0, 0.0);
//  glVertex3f( 1.0,  1.0, 0.0);
//  glVertex3f(-1.0,  1.0, 0.0);
//  glEnd();

  glPopMatrix();
  return 1;
  }


//  bool SG_TextArea::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_TextArea::Default_Mouse_Cursor = NULL;

void SG_TextArea::SetMargins(float xmar, float ymar) {
  xmargin = xmar;
  ymargin = ymar;
  }

