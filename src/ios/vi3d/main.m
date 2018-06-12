//
//  main.m
//  vi3d
//
//  Created by tianye on 2017/4/21.
//  Copyright © 2017年 viware. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <pthread.h>
#import <sys/time.h>
#import "vi3d.h"

int runflag = 1;
int actived = 1;

CAEAGLLayer* eaglLayer = NULL;
EAGLContext* eaglContext = NULL;

void* _main(void* args)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    const char* savedir = [[paths objectAtIndex:0] UTF8String];
    const char* datadir = [[[NSBundle mainBundle] resourcePath] UTF8String];

    vi_app_init(datadir, savedir);
	vi_app_main();

	//loop------------------------------------------
    float dt;
	struct timeval t1, t2;
	struct timezone tz;
	gettimeofday(&t1, &tz);

    while(runflag){
        gettimeofday(&t2, &tz);
        dt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
        t1 = t2;
        if(actived && eaglContext) {
            vi_app_loop(dt);
            [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
        }
        usleep(10000);
    }

    //eixt------------------------------------------
    vi_app_exit();
    return NULL;
}



@interface OpenGLView : UIView {
    GLuint _defaultFrameBuffer;
	GLuint _colorRenderBuffer;
	GLuint _depthRenderBuffer;
}
@end


@implementation OpenGLView

- (id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];

	eaglLayer = (CAEAGLLayer*) self.layer;
	eaglLayer.opaque = YES;
		
	eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	if (!eaglContext) {
		NSLog(@"[VI3D]Failed to get OpenGLES context");
		exit(1);
	}
	
	if (![EAGLContext setCurrentContext:eaglContext]) {
		NSLog(@"[VI3D]Failed to set OpenGLES context");
		exit(1);
	}

    GLuint colorRenderBuffer;
    glGenRenderbuffers(1, &colorRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
	[eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];

    GLuint depthRenderBuffer;
    glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, self.frame.size.width, self.frame.size.height);

    GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	return self;
}


+ (Class)layerClass {
	return [CAEAGLLayer class];
}


- (void)setNeedsDisplay
{
    //need refresh
}

- (void)setNeedsDisplayInRect:(CGRect)rect
{
    //need refresh
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = (UITouch *)[[touches allObjects] objectAtIndex:0];
    CGPoint point = [touch locationInView:nil];
    
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touchesEnded:touches withEvent:event];
}

@end





@interface AppDelegate : UIResponder <UIApplicationDelegate>

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    CGRect rect = UIScreen.mainScreen.bounds;

    UIWindow* win = [[UIWindow alloc] initWithFrame:rect];
    [win setScreen:UIScreen.mainScreen];
	
    UIViewController *ctl = [[UIViewController alloc] init];
	[ctl setView:[[OpenGLView alloc] initWithFrame:[win bounds]]];
    [ctl prefersStatusBarHidden];

    [win setRootViewController:ctl];
	[win makeKeyAndVisible];

    vi_app_set_screen_size(rect.size.width, rect.size.height);

    pthread_t tid;
    pthread_create(&tid, 0, &_main, 0);
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    runflag = 0;
}

@end



int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
