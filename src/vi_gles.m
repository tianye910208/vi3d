#import <UIKit/UIKit.h>
#import "vi_gles.h"
#import "vi_log.h"

#ifdef VI3D_GLES_AGL

CAEAGLLayer* eaglLayer = NULL;
EAGLContext* eaglContext = NULL;

int vi_gles_agl_init(void* layer, void* size) {
	eaglLayer = (CAEAGLLayer*)CFBridgingRelease(layer);
    
    Size* psize = (Size*) size;
		
	eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	if (!eaglContext) {
		vi_log("[E]Failed to get OpenGLES context");
		return 101;
	}
	
	if (![EAGLContext setCurrentContext:eaglContext]) {
		vi_log("[E]Failed to set OpenGLES context");
		return 102;
	}

    GLuint depthRenderBuffer;
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, psize->width, psize->height);

    GLuint colorRenderBuffer;
    glGenRenderbuffers(1, &colorRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
	[eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];

    
    GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);


	return 0;
}

int vi_gles_agl_exit() {
	return 0;
}

int vi_gles_agl_swap() {
	[eaglContext presentRenderbuffer:GL_RENDERBUFFER];
	return 0;
}

#endif//VI3D_GLES_AGL











