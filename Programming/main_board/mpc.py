'''
MPC Algorithm Implementation
'''
from machine import Timer
class SyncedMove:
    '''
    Object to handle the Synced Move where all 
    '''
    MAX_NUDGE_NUM = 8  # max number of .forward or .backward to reach either end of axis
    NUDGE_TIME = 1000 # 1sec

    FORWARD_STATE = 1
    BACKWARD_STATE = -1
    INVERT_STATE = {1: -1, -1: 1}  # dict to invert the states
    def __init__(self, axis):
        
        self.axis = axis  # for controlling the axis

        self.state = self.FORWARD_STATE
        self.nudges_moved = 0

        # dummy temp for calling pause before run
        self.timer = Timer()

    def run(self):
        '''
        run routine, made to be compatible with pause/start cycle

        starts the timer that calls the one_step movement until a pause
        '''
        self.timer = Timer(period=self.NUDGE_TIME, mode=Timer.PERIODIC, callback=self.one_step)

    def pause(self):
        '''
        Pauses the run routine to be continued later when .run is called again
        '''
        self.timer.deinit()
        
    def one_step(self):
        '''
        THIS METHOD IS MADE TO RUN UNDER A WHILE TRUE
        IT'S BASICALLY A ONE STEP THAT NEEDS TO BE REPEATED
        '''
        if self.nudges_moved <= self.MAX_NUDGE_NUM:
            self.axis.move(0, self.axis.DEFAULT_NUDGE_VALUE * self.state)
            self.axis.move(1, self.axis.DEFAULT_NUDGE_VALUE * self.state)
            self.axis.move(2, self.axis.DEFAULT_NUDGE_VALUE * self.state)

            self.nudges_moved += 1

        else:
            self.state = self.INVERT_STATE[self.state]
            self.nudges_moved = 0

class MPC:
    '''
    Abstraction for MPC control Algorithm
    '''
    def __init__(self, sensors, actuators):

        self.synced_move = SyncedMove(actuators.axis)

    def run(self):
        '''
        runs all MPC routine
        '''
        self.synced_move.run()

    def pause(self):
        '''
        pauses all MPC routines
        '''
        self.synced_move.pause()

