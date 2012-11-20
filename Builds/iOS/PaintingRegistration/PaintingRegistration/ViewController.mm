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
#import "ViewController.h"
#include "App.h"

@interface ViewController ()
{

}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

PaintingRegistration::App *app;

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
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1] autorelease];

    if (!self.context)
    {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
    
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGSize screenSize = CGSizeMake(screenBounds.size.width, screenBounds.size.height);
    
    app = new PaintingRegistration::App(screenSize.width, screenSize.height, [resourcePath UTF8String]);
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
    app->update();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    app->render();
}

@end
