/**
 * This file is part of PaintingRegistration.
 *
 * Created by Jonathan Hook (jonathan.hook@ncl.ac.uk)
 * Copyright (c) 2012 Jonathan Hook. All rights reserved.
 *
 * PaintingRegistration is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PaintingRegistration is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PaintingRegistration.  If not, see <http://www.gnu.org/licenses/>.
 */
#import <opencv2/opencv.hpp>
#import "JDHUtility/CrossPlatformTime.h"
#import "ViewController.h"
#include "JDHUtility/stb_image.h"
#include "JDHUtility/FileLocationUtility.h"
#include "TargetConditionals.h"
#include "App.h"

@interface ViewController ()
{

}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

const unsigned int CAM_WIDTH = 480;
const unsigned int CAM_HEIGHT = 360;
const unsigned int BUFFER_SIZE = CAM_WIDTH * CAM_HEIGHT * 4;
const unsigned int PROCESSING_RENDER_RATE = 1000;

CGFloat winX = 1.0f;
CGFloat winY = 1.0f;
uchar *frameData = new uchar[BUFFER_SIZE];
PaintingRegistration::App *app;
unsigned int frameWidth = 0;
unsigned int frameHeight = 0;
unsigned int lastProcessingRender = 0;
bool loaded = false;

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context)
    {
        [EAGLContext setCurrentContext:nil];
    }
    
    [_context release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    if(!loaded)
    {
        self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1] autorelease];
    
        GLKView *view = (GLKView *)self.view;
        view.context = self.context;
        view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
        [self setupGL];
    
        NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
        CGRect screenBounds = [[UIScreen mainScreen] bounds];
        CGSize screenSize = CGSizeMake(screenBounds.size.width, screenBounds.size.height);
        winX = screenSize.width;
        winY = screenSize.height;
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsPath = [paths objectAtIndex:0];
    
        app = new PaintingRegistration::App(winX, winY, CAM_WIDTH, CAM_HEIGHT, [resourcePath UTF8String], [documentsPath UTF8String]);
        
#if TARGET_IPHONE_SIMULATOR
        std::string path = FileLocationUtility::getFileInResourcePath("debug_frame.png");
        int x, y, n;
        unsigned char *data = stbi_load(path.c_str(), &x, &y, &n, 0);
        memcpy(frameData, data, sizeof(unsigned char) * BUFFER_SIZE);
        app->setLatestFrame(frameData);
#else
        [self initVideoCapture];
#endif
    
        loaded = true;
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil))
    {
        self.view = nil;
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context)
        {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    if(!app->getIsProcessing())
    {
        app->update();
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if(!app->getIsProcessing())
    {
        app->render();
    }
    else
    {
        unsigned int now = CrossPlatformTime::getTimeMillis();
        if(now - lastProcessingRender > PROCESSING_RENDER_RATE)
        {
            app->render();
            lastProcessingRender = CrossPlatformTime::getTimeMillis();
        }
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet *allTouches = [event allTouches];
    
    // todo: multiple touches
    UITouch *touch = [[allTouches allObjects] objectAtIndex:(0)];
    CGPoint p = [touch locationInView:(self.view)];
    
    CGFloat x = p.x / winX;
    CGFloat y = p.y / winX;
    
    app->raiseEvent(0, x, y, FingerEventArgs::FINGER_ADDED);
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet *allTouches = [event allTouches];
    
    // todo: multiple touches
    UITouch *touch = [[allTouches allObjects] objectAtIndex:(0)];
    CGPoint p = [touch locationInView:(self.view)];
    
    CGFloat x = p.x / winX;
    CGFloat y = p.y / winX;
    
    app->raiseEvent(0, x, y, FingerEventArgs::FINGER_UPDATED);
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet *allTouches = [event allTouches];
    
    // todo: multiple touches
    UITouch *touch = [[allTouches allObjects] objectAtIndex:(0)];
    CGPoint p = [touch locationInView:(self.view)];
    
    CGFloat x = p.x / winX;
    CGFloat y = p.y / winX;
    
    app->raiseEvent(0, x, y, FingerEventArgs::FINGER_REMOVED);
}

- (void)initVideoCapture
{
    NSError *error;
    
    AVCaptureSession *session = [[AVCaptureSession alloc] init];
    session.sessionPreset = AVCaptureSessionPresetMedium;
    
    AVCaptureDevice *device = [self findBackFacingCamera];
    
    [device lockForConfiguration:&error];
    [device setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
    [device setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
    [device setWhiteBalanceMode:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance];
    [device unlockForConfiguration];
    
    AVCaptureDeviceInput *input =[AVCaptureDeviceInput deviceInputWithDevice:device error:&error];    
    [session addInput:input];
    
    AVCaptureVideoDataOutput *output = [[AVCaptureVideoDataOutput alloc] init];
    [output setAlwaysDiscardsLateVideoFrames:YES];
    
    dispatch_queue_t queue = dispatch_queue_create("cameraQueue", NULL);
    [output setSampleBufferDelegate:self queue:queue];
    dispatch_release(queue);
    
    NSDictionary *settings = @{ (NSString *)kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA) };
    [output setVideoSettings:settings];

    [session addOutput:output];
    [session startRunning];
}

-(AVCaptureDevice *)findBackFacingCamera
{
    NSArray *devices = [AVCaptureDevice devices];
    for (AVCaptureDevice *device in devices)
    {
        if ([device hasMediaType:AVMediaTypeVideo] && [device position] == AVCaptureDevicePositionBack)
        {
            return device;
        }
    }
    
    return nil;
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    if(!app->getIsProcessing())
    {
        NSAutoreleasePool* localpool = [[NSAutoreleasePool alloc] init];
        CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
        CGRect videoRect = CGRectMake(0.0f, 0.0f, CVPixelBufferGetWidth(pixelBuffer), CVPixelBufferGetHeight(pixelBuffer));
    
        CVPixelBufferLockBaseAddress(pixelBuffer, 0);
        void *baseaddress = CVPixelBufferGetBaseAddress(pixelBuffer);

        frameWidth = videoRect.size.width;
        frameHeight = videoRect.size.height;
        memcpy(frameData, baseaddress, sizeof(unsigned char) * BUFFER_SIZE);
    
        CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);

        [localpool drain];
    
        app->setLatestFrame(frameData);
    }
}
@end
