# Import a library of functions called 'pygame'
import sys
import pygame
from math import pi
from operator import itemgetter
# Initialize the game engine
pygame.init()
 
# Define the colors we will use in RGB format
BLACK = (  0,   0,   0)
WHITE = (255, 255, 255)
BLUE =  (  0,   0, 255)
GREEN = (  0, 255,   0)
RED =   (255,   0,   0)

COLORS = [BLACK, BLUE, GREEN, RED]
 
# Set the height and width of the screen
size = [601, 401]
screen = pygame.display.set_mode(size)
 
pygame.display.set_caption("Python script to draw treemap")
 
#Loop until the user clicks the close button.
done = False
clock = pygame.time.Clock()


rects = []

n = 0
for line in sys.stdin:
    n = n + 1    
    print line
    line = line.split(" ")
    rects.append(line)

rects = sorted(rects, key=itemgetter(0))
print rects
 
while not done:
 
    # This limits the while loop to a max of 10 times per second.
    # Leave this out and we will use all CPU we can.
    clock.tick(10)
     
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop
 
    # All drawing code happens after the for loop and but
    # inside the main while done==False loop.
     
    # Clear the screen and set the screen background
    screen.fill(WHITE)
 
   # Draw a solid rectangle
    for i in range (0, n):
        lvl = int(rects[i][0])
        pygame.draw.rect(screen, COLORS[lvl], [float(rects[i][2])+lvl, float(rects[i][3]), float(rects[i][4])-float(rects[i][2]), float(rects[i][5])-float(rects[i][3])], 4-int(rects[i][0]))
     
  
    # Go ahead and update the screen with what we've drawn.
    # This MUST happen after all the other drawing commands.
    pygame.display.flip()
 
# Be IDLE friendly
pygame.quit()