#import numpy as np
# 
#def mandelbrot(c,maxiter):
#    z = c
#    for n in range(maxiter):
#        if abs(z) > 2:
#            return n
#        z = z*z + c
#    return 0
#
#def mandelbrot_set(xmin,xmax,ymin,ymax,width,height,maxiter):
#    r1 = np.linspace(xmin, xmax, width)
#    r2 = np.linspace(ymin, ymax, height)
#    n3 = np.empty((width,height))
#    for i in range(width):
#        for j in range(height):
#            n3[i,j] = mandelbrot(r1[i] + 1j*r2[j],maxiter)
#    return (r1,r2,n3)


import numpy as np
#from pylab import imshow, show
#from timeit import default_timer as timer
import pdb
#from graphics import Image, ImageTk
#import Tkinter
from matplotlib import cm
import sys
#import time

class Mandelbrot():
    
    def __init__(self, iters,height,width):
        self.iters = iters
        self.image = np.zeros((height, width), dtype = np.float64)
        
        self.height = height
        self.width = width
        
        self.center_x = 0.
        self.center_y = 0.
        
        self.zoom_x = 1.5
        self.zoom_y = 1.
        self.color_func = cm.gist_earth
        self.color_options = [cm.gist_earth, cm.gist_gray, cm.autumn,
                              cm.seismic, cm.bwr, cm.RdGy]
        self.color_opt = 0;
    def create_fractal(self,min_x, max_x, min_y, max_y):
        
        self.pixel_size_x = (max_x - min_x) / self.width
        self.pixel_size_y = (max_y - min_y) / self.height
        
        for x in range(self.width):
            real = min_x + x * self.pixel_size_x
            for y in range(self.height):
                imag = min_y + y * self.pixel_size_y
                color = mandel(real, imag, self.iters)
                self.image[y, x] = color

def move(self,e):
    self.center_x = (e.x - self.width/2.) * self.pixel_size_x +self.center_x
    self.center_y = (e.y - self.height/2.) * self.pixel_size_y + self.center_y
        #pdb.set_trace()
    min_x = self.center_x - self.zoom_x
    max_x = self.center_x + self.zoom_x
    min_y = self.center_y - self.zoom_y
    max_y = self.center_y + self.zoom_y
        
        
    self.create_fractal(min_x, max_x, min_y, max_y)
        
        #normalise
    self.image = self.image / self.image.max()
    im = Image.fromarray(self.color_func(self.image,bytes=True))
    photo_im = ImageTk.PhotoImage(im)
    w.config(image=photo_im)
    w.image = photo_im
    
    def zoom_in(self,e=None):
        self.zoom_x = self.zoom_x / 2.
        self.zoom_y = self.zoom_y / 2.
        min_x = self.center_x - self.zoom_x
        max_x = self.center_x + self.zoom_x
        min_y = self.center_y - self.zoom_y
        max_y = self.center_y + self.zoom_y
        
        
        self.create_fractal(min_x, max_x, min_y, max_y)
        
        #normalise
        self.image = self.image / self.image.max()
        im = Image.fromarray(self.color_func(self.image,bytes=True))
        photo_im = ImageTk.PhotoImage(im)
        w.config(image=photo_im)
        w.image = photo_im

def zoom_out(self, e=None):
    self.zoom_x = self.zoom_x * 2.
    self.zoom_y = self.zoom_y * 2.
    min_x = self.center_x - self.zoom_x
    max_x = self.center_x + self.zoom_x
    min_y = self.center_y - self.zoom_y
    max_y = self.center_y + self.zoom_y
        
        
    self.create_fractal(min_x, max_x, min_y, max_y)
        
        #normalise
    self.image = self.image / self.image.max()
    im = Image.fromarray(self.color_func(self.image,bytes=True))
    photo_im = ImageTk.PhotoImage(im)
    w.config(image=photo_im)
    w.image = photo_im
    
    def change_color(self, e=None):
        min_x = self.center_x - self.zoom_x
        max_x = self.center_x + self.zoom_x
        min_y = self.center_y - self.zoom_y
        max_y = self.center_y + self.zoom_y
        
        self.color_opt = (self.color_opt + 1) % len(self.color_options)
        self.color_func = self.color_options[self.color_opt]
        
        #self.create_fractal(min_x, max_x, min_y, max_y)
        
        #normalise
        self.image = self.image / self.image.max()
        im = Image.fromarray(self.color_func(self.image,bytes=True))
        photo_im = ImageTk.PhotoImage(im)
        w.config(image=photo_im)
        w.image = photo_im

def change_color_back(self, e=None):
    min_x = self.center_x - self.zoom_x
    max_x = self.center_x + self.zoom_x
    min_y = self.center_y - self.zoom_y
    max_y = self.center_y + self.zoom_y
        
    self.color_opt = (self.color_opt - 1) % len(self.color_options)
    self.color_func = self.color_options[self.color_opt]
        
        #self.create_fractal(min_x, max_x, min_y, max_y)
        
        #normalise
    self.image = self.image / self.image.max()
    im = Image.fromarray(self.color_func(self.image,bytes=True))
    photo_im = ImageTk.PhotoImage(im)
    w.config(image=photo_im)
    w.image = photo_im


def mandel(x, y, max_iters):
    """
        Given the real and imaginary parts of a complex number,
        determine if it is a candidate for membership in the Mandelbrot
        set given a fixed number of iterations.
        """
    c = complex(x, y)
    z = 0.0j
    for i in range(max_iters):
        z = z*z + c
        if (z.real*z.real + z.imag*z.imag) >= 4:
            return i

    return max_iters






if __name__ == '__main__':
    
    mb = Mandelbrot(100,240,320)
    mb.create_fractal(-2.0,1.0,-1.0,1.0)
    
    
    root = Tkinter.Tk()
    
    
    mb.image = mb.image / mb.image.max()
    im = Image.fromarray(mb.color_func(mb.image,bytes=True))
    
    photo_im = ImageTk.PhotoImage(im)
    
    
    w = Tkinter.Label(root, image=photo_im)
    w.image = photo_im
    w.pack()
    """
        b1 = Tkinter.Button(root, text="exit")
        b1.configure(command=root.quit)
        b1.pack()
        b2 = Tkinter.Button(root, text="+")
        b2.configure(command=mb.zoom_in)
        b2.pack()
        b3 = Tkinter.Button(root, text="-")
        b3.configure(command=mb.zoom_out)
        b3.pack()
        """
    wid, hei = root.winfo_screenwidth(), root.winfo_screenheight()
    root.overrideredirect(True)
    root.geometry("%dx%d+0+0" % (wid, hei))
    root.focus_set() # <-- move focus to this widget
    w.focus_set()
    
    root.bind("<Escape>", lambda e: root.quit())
    w.bind("<Button-2>", lambda e: sys.exit())
    
    w.bind("<Button-1>", lambda event: mb.move(event))
    w.bind("<Double-Button-1>",lambda event: mb.zoom_in(event))
    w.bind("<Double-Button-3>",lambda event: mb.zoom_out(event))
    w.bind("<Button-4>", lambda event: mb.change_color(event))
    w.bind("<Button-5>", lambda event: mb.change_color_back(event))
    root.mainloop()
