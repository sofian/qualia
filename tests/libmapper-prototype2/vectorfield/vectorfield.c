// This code was modified from
// http://fabiensanglard.net/shadowmapping/index.php

#ifdef _WIN32
#include "windows.h"
#endif

#include "GL/glew.h"
#include "GL/glut.h"
#include "GL/glu.h"

#ifdef _WIN32
#include "glext.h"
#endif

#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef _WIN32
// As microsoft did not maintain openGL after version 1.1, Windows platform need to go throught this crap ; macosX and Linux are fine.
// This block simply retries openGL function needed for this example.
// I recommend to use GLEW instead of going this way. This is done this way only to ease beginner's compilation and portability


PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;

// FrameBuffer (FBO) gen, bin and texturebind
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT ;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT ;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT ;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT ;

// Shader functions
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB ;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB   ;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB ;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB        ;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB       ;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB        ;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB         ;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB  ;
PFNGLUNIFORM1IARBPROC            glUniform1iARB           ;
PFNGLACTIVETEXTUREARBPROC		  glActiveTextureARB;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB          ;

void getOpenGLFunctionPointers(void)
{
	// FBO
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		wglGetProcAddress("glGenFramebuffersEXT");
	glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		wglGetProcAddress("glBindFramebufferEXT");
	glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
	glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
	
	//Shaders
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB"); 
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB"); 
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB"); 
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB"); 
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	
}
#endif

// Expressed as float so gluPerspective division returns a float and not 0 (640/480 != 640.0/480.0).
#define RENDER_WIDTH 640.0
#define RENDER_HEIGHT 480.0

// Hold id of the framebuffer for light POV rendering
GLuint fboId;

// colour values will be rendered to this texture when using fboId framebuffer
GLuint fieldTexIds[2] = {0,0};

GLhandleARB fieldShaderId;
GLuint fieldUniform;

GLuint src = 0, dest = 1;
int update_rate = 30;

// Loading shader function
GLhandleARB loadShader(char* filename, unsigned int type)
{
	FILE *pfile;
	GLhandleARB handle;
	const GLcharARB* files[1];
	
	// shader Compilation variable
	GLint result;				// Compilation code result
	GLint errorLoglength ;
	char* errorLogText;
	GLsizei actualErrorLogLength;
	
	char buffer[400000];
	memset(buffer,0,400000);
	
	// This will raise a warning on MS compiler
	pfile = fopen(filename, "rb");
	if(!pfile)
	{
		printf("Sorry, can't open file: '%s'.\n", filename);
		exit(0);
	}
	
	fread(buffer,sizeof(char),400000,pfile);
	//printf("%s\n",buffer);
	
	
	fclose(pfile);
	
	handle = glCreateShaderObjectARB(type);
	if (!handle)
	{
		//We have failed creating the vertex shader object.
		printf("Failed creating vertex shader object from file: %s.",filename);
		exit(0);
	}
	
	files[0] = (const GLcharARB*)buffer;
	glShaderSourceARB(
					  handle, //The handle to our shader
					  1, //The number of files.
					  files, //An array of const char * data, which represents the source code of theshaders
					  NULL);
	
	glCompileShaderARB(handle);
	
	//Compilation checking.
	glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	
	// If an error was detected.
	if (!result)
	{
		//We failed to compile.
		printf("Shader '%s' failed compilation.\n",filename);
		
		//Attempt to get the length of our error log.
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);
		
		//Create a buffer to read compilation error message
		errorLogText = malloc(sizeof(char) * errorLoglength);
		
		//Used to get the final length of the log.
		glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, errorLogText);
		
		// Display errors.
		printf("%s\n",errorLogText);
		
		// Free the buffer malloced earlier
		free(errorLogText);

        exit(1);
	}
	
	return handle;
}

void loadFieldShader()
{
	GLhandleARB vertexShaderHandle;
	GLhandleARB fragmentShaderHandle;
	
	vertexShaderHandle   = loadShader("VertexShader.c",GL_VERTEX_SHADER);
	fragmentShaderHandle = loadShader("FragmentShader.c",GL_FRAGMENT_SHADER);
	
	fieldShaderId = glCreateProgramObjectARB();
	
	glAttachObjectARB(fieldShaderId,vertexShaderHandle);
	glAttachObjectARB(fieldShaderId,fragmentShaderHandle);
	glLinkProgramARB(fieldShaderId);
	
	fieldUniform = glGetUniformLocationARB(fieldShaderId, "field");
}

void generateFBO()
{
	//GLfloat borderColor[4] = {0,0,0,0};
	
	GLenum FBOstatus;

    int i;
    for (i=0; i<2; i++)
    {
        GLuint *id = &fieldTexIds[i];
        glGenTextures(1, id);
        glBindTexture(GL_TEXTURE_2D, *id);
	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
        // Remove artefact on the edges
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        //glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );
	
        // No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available 
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16,
                      RENDER_WIDTH, RENDER_HEIGHT, 0, GL_RGBA,
                      GL_UNSIGNED_BYTE, 0);
    }

	glBindTexture(GL_TEXTURE_2D, 0);
	
	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
	
	// attach the texture to FBO color attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, fieldTexIds[0], 0);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                              GL_TEXTURE_2D, fieldTexIds[1], 0);

	// check FBO status
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT) {
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
        exit(1);
    }

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glClear( GL_COLOR_BUFFER_BIT);

	glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
	glClear( GL_COLOR_BUFFER_BIT);
	
	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void setupMatrices()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(0, RENDER_WIDTH, 0, RENDER_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void update(void)
{
    // Nothing to do.
}

void drawObjects(int i)
{
	// Square
	glColor4f(0.3f,0.3f,0.3f,1);
	glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);
	glVertex2f(0,0);
    glTexCoord2f(0,1);
	glVertex2f(0, RENDER_HEIGHT/i);
    glTexCoord2f(1,1);
	glVertex2f(RENDER_WIDTH/i, RENDER_HEIGHT/i);
    glTexCoord2f(1,0);
	glVertex2f(RENDER_WIDTH/i, 0);
	glEnd();
}

void renderScene(void) 
{
	update();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
	
	glViewport(0,0, RENDER_WIDTH, RENDER_HEIGHT);

    // Draw quad to the screen in the corner
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

    static int a = 1;
    if (1) {
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + src);

        setupMatrices();

        // Draw points
        glColor4f(1,1,1,1);
        glBegin(GL_POINTS);
        glVertex2f(RENDER_WIDTH/2, RENDER_HEIGHT/2);
        glVertex2f(RENDER_WIDTH/2-50, RENDER_HEIGHT/2-50);
        glEnd();
        a = 0;
    }

    if (1)
    {
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + dest);

        glClear(GL_COLOR_BUFFER_BIT);

        setupMatrices();

        //Using the field shader
        glUseProgramObjectARB(fieldShaderId);
        glUniform1iARB(fieldUniform, 7);
        glActiveTextureARB(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, fieldTexIds[src]);

        drawObjects(1);

        glUseProgramObjectARB(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (1)
    {
        // Draw quad to the screen in the corner
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        glClear( GL_COLOR_BUFFER_BIT);

        glActiveTextureARB(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fieldTexIds[dest]);

        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
        glEnable( GL_TEXTURE_2D );
        drawObjects(1);
        glDisable( GL_TEXTURE_2D );

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    static int cnt = 0;
    if (cnt++ > 50) {
        float data[50*50*4];
        glReadBuffer(GL_COLOR_ATTACHMENT0_EXT + dest);
        glReadPixels(RENDER_WIDTH/2-2, RENDER_HEIGHT/2-2, 3, 3, GL_RGBA, GL_FLOAT, data);
        fprintf(stderr, "observations: %f, %f, %f, %f\n",
                data[2*4+0+1*3*4],
                data[1*4+1+2*3*4],
                data[0*4+2+1*3*4],
                data[1*4+3+0*3*4]);
        exit(0);
    }
	
	glutSwapBuffers();

    src = 1-src;
    dest = 1-dest;
}

void processNormalKeys(unsigned char key, int x, int y) {
	
	if (key == 27) 
		exit(0);
}

void onTimer(int value)
{
    renderScene();
    glutTimerFunc((int)(1000.0/update_rate), onTimer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(RENDER_WIDTH,RENDER_HEIGHT);
	glutCreateWindow("GLSL Shadow mapping");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
	
	// This call will grab openGL extension function pointers.
	// This is not necessary for macosx and linux
#ifdef _WIN32
	getOpenGLFunctionPointers();
#endif
	generateFBO();
	loadFieldShader();
	
	glClearColor(0,0,0,1.0f);

	glutDisplayFunc(renderScene);
	//glutIdleFunc(renderScene);
	glutTimerFunc((int)(1000.0/update_rate), onTimer, 0);
	
	glutKeyboardFunc(processNormalKeys);

    //glutFullScreen();
	glutMainLoop();

    return 0;
}
