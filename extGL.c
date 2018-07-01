
#include <cgx.h>



#define     MY_COLORS       0
#define     MAX_LABELS      32

/* color model */
void define_rgb(float v, float *r, float *g, float *b)
{
   /* v,r,g,b == 0->1 */

#if MY_COLORS
        *r = v;
        *g = cos( (v-0.5) * 3.1415 );
        *b = 1. - v;
#else
     *r = 1.06003425e+04*pow(v,12) -5.93255252e+04 *pow(v,11)+ 1.42724241e+05*pow(v,10) -1.92794141e+05*pow(v,9)+  1.60324698e+05*pow(v,8) -8.47422762e+04*pow(v,7) + 2.84479798e+04*pow(v,6) -5.91464879e+03*pow(v,5)+ 7.27424586e+02*pow(v,4) -4.63780022e+01*pow(v,3) -1.41424635e+00*pow(v,2)+  4.28905389e-01*v + 2.65086499e-01;

     *g = 4.75186109e+00*pow(v,6) -1.41325308e+01*pow(v,5)+  1.46522357e+01*pow(v,4) -6.05303857e+00 *pow(v,3)+  2.86102681e-01*pow(v,2)+  1.39998447e+00*v+ 2.95264880e-04;

     *b = -1.07778548e+02*pow(v,10)+7.69836033e+02*pow(v,9)-2.03733532e+03*pow(v,8)+2.75032397e+03*pow(v,7)-2.09069223e+03*pow(v,6)+9.01078559e+02*pow(v,5)-2.06460578e+02*pow(v,4)+2.32768173e+01*pow(v,3)-4.05970173e+00*pow(v,2)+1.63907216e+00*v+3.22602707e-01;


#endif
}


/* schreibt einen Text (x,y,z linkes unteres Eck)  */
void text(double x, double y, double z,char *msg, void *glut_font)
{
  glRasterPos3d(x, y, z);
  while (*msg) {
    glutBitmapCharacter(glut_font, *msg);
    msg++;
  }
}


/* scala verbindet Farben mit Zahlenwerten (rgb-mode)*/
/* kb, kh Kaestchenhoehe und Breite                  */
/* dx, dy  Ursprung der Scala (linkes unteres Eck)   */
/* divisions anzahl der Kaestchen                    */
void scala_rgb( double dx, double dy, int divisions, double bmin, double bmax, double *col, void *glut_font, char format)
{
  int  j, i;
  float r, g, b;
  double df, f;
  static char string[13];
  double     kb, kh;
  int flag=0;

  df = (bmax-bmin)/divisions;
  f  = bmin;

  kh= 1.0/divisions*1.5;
  kb= 1.0/20.;


  for (i=0; i<divisions; i++)
  {
    glBegin ( GL_POLYGON );
      define_rgb( (float)(f-bmin)/(bmax-bmin), &r,&g,&b);
      glColor3d ( r, g, b );
      glVertex2d ( dx-kb*0., dy+kh*0. );
      glVertex2d ( dx-kb*1., dy+kh*0. );
      define_rgb( (float)(f+df-bmin)/(bmax-bmin), &r,&g,&b);
      glColor3d ( r, g, b );
      glVertex2d ( dx-kb*1., dy+kh*1. );
      glVertex2d ( dx-kb*0., dy+kh*1. );
    glEnd();

    if(flag==0)
    {
      if(format=='f') sprintf ( string, "%-10f ", f);
      else if(format=='i') sprintf ( string, "%-10d ", (int)f);
      else sprintf ( string, "%-10.2e ", f);
      glColor3dv ( col );
      glRasterPos2d( (dx+kb*0.2), dy-kh*0.1 );
      for ( j=0; j<10; j++) glutBitmapCharacter(glut_font, string[j]);
    }
    flag++;
    if(flag>divisions/MAX_LABELS) flag=0;

    dy+=kh;
    f+=df;
  }
  if(flag==0)
  {
    if(format=='f') sprintf ( string, "%-10f ", bmax);
    else if(format=='i') sprintf ( string, "%-10d ", (int)bmax);
    else sprintf ( string, "%-10.2e ", bmax);
    glColor3dv ( col );
    glRasterPos2d( (dx+kb*0.2), dy-kh*0.1 );
    for ( j=0; j<10; j++) glutBitmapCharacter(glut_font, string[j]);
  }
}


/* scalai verbindet Farben mit Zahlenwerten         */
/* kb, kh Kaestchenhoehe und Breite                */
/* dx, dy  Ursprung der Scala (linkes unteres Eck) */
/* divisions anzahl der Kaestchen                  */
void scala_indx( double dx, double dy, int divisions, double bmin, double bmax, int offset, int col, void *glut_font, char format)
{
  int j, i;
  double     df, f;
  static char string[13];
  double     kb, kh;
  int flag=0;

  df = (bmax-bmin)/(divisions);
  f  = bmin;

  kh= 1.0/divisions*1.5;
  kb= 1.0/20.;

  for (i=0; i<divisions; i++)
  {
    glIndexi ( offset+i );
    glBegin ( GL_POLYGON );
      glVertex2d ( dx-kb*0., dy+kh*0. );
      glVertex2d ( dx-kb*1., dy+kh*0. );
      glVertex2d ( dx-kb*1., dy+kh*1. );
      glVertex2d ( dx-kb*0., dy+kh*1. );
    glEnd();

    if(flag==0)
    {
      if(format=='f') sprintf ( string, "%-10f ", f);
      else if(format=='i') sprintf ( string, "%-10d ", (int)f);
      else sprintf ( string, "%-10.2e ", f);
      glIndexi  ( col );
      glRasterPos2d( (dx+kb*0.2), dy-kh*0.1 );
      for ( j=0; j<10; j++) glutBitmapCharacter(glut_font, string[j]);
    }
    flag++;
    if(flag>divisions/MAX_LABELS) flag=0;

    dy+=kh;
    f+=df;
  }
  if(flag==0)
  {
    if(format=='f') sprintf ( string, "%-10f ", bmax);
    else if(format=='i') sprintf ( string, "%-10d ", (int)bmax);
    else sprintf ( string, "%-10.2e ", bmax);
    glIndexi  ( col );
    glRasterPos2d( (dx+kb*0.2), dy-kh*0.1 );
    for ( j=0; j<10; j++) glutBitmapCharacter(glut_font, string[j]);
  }
}


/* scalai verbindet Farben mit Zahlenwerten         */
/* kb, kh Kaestchenhoehe und Breite                */
/* dx, dy  Ursprung der Scala (linkes unteres Eck) */
/* divisions anzahl der Kaestchen                  */
void scala_tex( double dx, double dy, int divisions, double bmin, double bmax, double scale, double *col, void *glut_font, char format, char fnr, char lnr)
{
  int j, i;
  double     df, f;
  static char string[13];
  double     kb, kh;
  int flag=0;

  df = (bmax-bmin)/(divisions);
  f  = bmin;

  kh= 1.0/divisions*1.5;
  kb= 1.0/20.;

  for (i=0; i<divisions; i++)
  {
    /* enable all colors for the TEX_MODE */
    glColor3d( 1,1,1);
    glEnable(GL_TEXTURE_1D);
    glTexCoord1d    ( (GLdouble)i/(GLdouble)divisions*scale );
    glBegin ( GL_POLYGON );
      glVertex2d ( dx-kb*0., dy+kh*0. );
      glVertex2d ( dx-kb*1., dy+kh*0. );
      glVertex2d ( dx-kb*1., dy+kh*1. );
      glVertex2d ( dx-kb*0., dy+kh*1. );
    glEnd();
    glDisable(GL_TEXTURE_1D);

    if(flag==0)
    {
      if(format=='f') sprintf ( string, "%-10f ", f);
      else if(format=='i') sprintf ( string, "%-10d ", (int)f);
      else
      {
        if((!fnr)&&(i==1) && (abs(f)<df*1e-4)) sprintf ( string, "%-10.2e ", 0.);
        else sprintf ( string, "%-10.2e ", f);
      }
      glColor3dv ( col );
      glRasterPos2d( (dx+kb*0.2), dy-kh*0.1 );
      if((!fnr)&&(!i)); else { for ( j=0; j<10; j++) glutBitmapCharacter(glut_font, string[j]); }
    }
    flag++;
    if(flag>divisions/MAX_LABELS) flag=0;

    dy+=kh;
    f+=df;
  }
  if((flag==0)&&(lnr))
  {
    if(format=='f') sprintf ( string, "%-10f ", bmax);
    else if(format=='i') sprintf ( string, "%-10d ", (int)bmax);
    else sprintf ( string, "%-10.2e ", bmax);
    glColor3dv ( col );
    glRasterPos2d( (dx+kb*0.2), dy-kh*0.1 );
    for ( j=0; j<10; j++) glutBitmapCharacter(glut_font, string[j]);
  }
}

/* Button liefert 1 wenn maus im Button, sonst 0  */
/* dx,dy    Ursprung des Buttons (linkes unteres Eck)  */
/* bh, bb   Hoehe, Breite des Buttons  */
/* mx, my   Mauskoordinaten  */
int button(double dx, double dy, char *msg, double mx, double my, void *glut_font)
{
    register int   i, n;
    double bh, bb;

    i=0;
    bb=bh=0.05;

    while (*msg) {i++; msg++; }
    bb+=0.025*i;
    msg-=i;



    if ((mx > dx+bb*0.) & (mx < dx+bb*1.) & (my > dy+bh*0.) & (my < dy+bh*1.))
      {
      glColor3d ( 1., 0.0, 0.0 );
      glBegin ( GL_POLYGON );
        glVertex2d ( dx+bb*0., dy+bh*0. );
        glVertex2d ( dx+bb*1., dy+bh*0. );
        glVertex2d ( dx+bb*1., dy+bh*1. );
        glVertex2d ( dx+bb*0., dy+bh*1. );
      glEnd();
      glColor3d ( 0., 0., 0. );
      glRasterPos2d((dx+bh*0.3), (dy+bh*0.2));
      for (n=0; n<i; n++)
        glutBitmapCharacter(glut_font, msg[n]);
      glBegin ( GL_LINE_LOOP );
        glVertex2d ( dx+bb*0., dy+bh*0. );
        glVertex2d ( dx+bb*1., dy+bh*0. );
        glVertex2d ( dx+bb*1., dy+bh*1. );
        glVertex2d ( dx+bb*0., dy+bh*1. );
      glEnd();
      return (1);
      }
    else
      {
      glColor3d ( 0.7, 0.7, 0.7 );
      glBegin ( GL_POLYGON );
        glVertex2d ( dx+bb*0., dy+bh*0. );
        glVertex2d ( dx+bb*1., dy+bh*0. );
        glVertex2d ( dx+bb*1., dy+bh*1. );
        glVertex2d ( dx+bb*0., dy+bh*1. );
      glEnd();
      glColor3d ( 0., 0., 0. );
      glRasterPos2d((dx+bh*0.3), (dy+bh*0.2));
      for (n=0; n<i; n++)
        glutBitmapCharacter(glut_font, msg[n]);
      glBegin ( GL_LINE_LOOP );
        glVertex2d ( dx+bb*0., dy+bh*0. );
        glVertex2d ( dx+bb*1., dy+bh*0. );
        glVertex2d ( dx+bb*1., dy+bh*1. );
        glVertex2d ( dx+bb*0., dy+bh*1. );
      glEnd();
      return(0);
      }
}


/* Plaziert farbige Punkte  */
void polymark ( int n, double *col_r, double *col_g, double *col_b, double *x,
 double *y, double *z )
{
  GLint mode;
  int i;

  glGetIntegerv(GL_RENDER_MODE, &mode);

  if (mode == GL_RENDER)
    {
    for (i=1; i<=n; i++ )
      {
      glBegin ( GL_POINTS );
       glColor3d ( col_r[i], col_g[i], col_b[i] );
       glVertex3d ( x[i], y[i], z[i] );
      glEnd();
      }
    }
  if (mode == GL_SELECT)
    {
    for (i=1; i<=n; i++ )
      {
      glLoadName(i);
      glBegin ( GL_POINTS );
       glColor3d ( col_r[i], col_g[i], col_b[i] );
       glVertex3d ( x[i], y[i], z[i] );
      glEnd();
      }
    }
}



