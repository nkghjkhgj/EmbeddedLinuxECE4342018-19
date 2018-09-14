#!/usr/bin/env python

import curses
import sys



#Helper Class to manage the drawings 
class SketchHelper():
    def __init__(self):
        self.pos = [0,0]
        self.path = list(self.pos)
        self.height = 20
        self.width = 20
        self.unpassed = "x"
        self.passed = " "
        self.stdscr = None
        
        
        
        
        # clear screen and reset cursor to upper left corner;
    def resetall(self):
        self.stdscr.clear()
        for y in xrange(0, self.height):
            for x in xrange(0, self.width):
                self.stdscr.addstr(y,x,self.passed)
        self.pos = [0,0]
    
    def main(self, stdscr):
        self.stdscr = stdscr
        stdscr.clear()
         # //user defined grid size , does not work!
        # while True:
        #     c = stdscr.getch(self.height - 1, self.width - 1)
        #     stdscr.addstr(0,0,"Welcome to Etch_A_Sketch, enter your height:")
        #     self.height = stdscr.getch()
        #     stdscr.addstr(1,0,"Welcome to Etch_A_Sketch, enter your width:")
        #     if c == curses.ENTER:
        #     self.width = stdscr.getch()
        #     break
        
        
        # move the cursor and draw "x" and the path
        while True:
            c = stdscr.getch(self.height - 1, self.width - 1)
            if c == curses.KEY_UP:
                if self.pos[1] > 0:
                    self.pos[1] -= 1
            elif c == curses.KEY_DOWN:
                if self.pos[1] < self.height - 1:
                    self.pos[1] += 1
            elif c == curses.KEY_LEFT:
                if self.pos[0] > 0:
                    self.pos[0] -= 1
            elif c == curses.KEY_RIGHT:
                if self.pos[0] < self.width - 1:
                    self.pos[0] += 1
                    
            elif c == ord('r'):
                self.resetall()
                self.path = list(self.pos)
                continue
            elif c == ord('q'):
                break
            else:
                continue
            stdscr.addstr(self.path[1], self.path[0], self.unpassed)
            self.path = list(self.pos)

sketch = SketchHelper()
curses.wrapper(sketch.main)

