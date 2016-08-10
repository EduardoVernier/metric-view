Research Project Report

https://github.com/EduardoVernier/metric-view/raw/master/docs/report.pdf

Installing dependencies:

sudo apt-get install freeglut3-dev

sudo apt-get install libxmu-dev libxmu-headers

sudo apt-get install libxi-dev

make

./bin/runner data/exo-d/ data/exo-p/

On Mac:

The include file is GLUT/glut.h instead of the usual GL/glut.h
If you're building on the command-line, you'll need to use -framework GLUT -framework OpenGL -framework Cocoa to link your program instead of -L/usr/X11R6/lib -lGL -lGLU -lglut -lX11 that Linux uses.
