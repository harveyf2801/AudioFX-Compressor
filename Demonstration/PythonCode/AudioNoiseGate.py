import numpy as np
from PythonCode.decibelsToGain import decibels_to_gain
from PythonCode.gainToDecibels import gain_to_decibels

class AudioNoiseGate:
    ''' Creates an audio noise gate class which can be used to perform a noise gate effect on audio. '''
    
    def __init__(self, threshold, hold, attack, release):
        '''
          Constructor to initialize the object with specified parameters.
          
          Parameters -
          threshold: the dB threshold value that the compressor will
            start to take effect after passing
          hold: the amount of time to hold the compression on after the
            attack phase and before the release phase
          attack: the amount of time it takes for the compression to
            start taking effect after passing the threshold
          release: the amount of time it takes for the compression to
            stop being applied after going above the threshold
        '''
        
        self.threshold = threshold
        self.thresholdLin = decibels_to_gain(threshold, -120)
        self.attack = attack
        self.release = release
        self.hold = hold

    def process(self, x, fs, sidechain=None):
        '''
          Public method to process the input signal. The entire processing chain is applied here.
          
          Parameters -
          x: input signal
          fs: sample rate of the input signal
          sidechain (OPTIONAL): the sidechain input to use to calculate
            gain reduction
        '''
        if sidechain is None:
            sidechain = x.copy()

        # Calculate the absolute of the N-point signal (in magnitude)
        xa = np.abs(sidechain)

        # Compute the static characteristic with a soft knee
        gclin = self.applyLinearStaticCharacteristic(xa)

        # Smooth the gain using specified attack and release time parameters
        alphaA = self.calculateTimeCoeff(self.attack, fs)
        alphaR = self.calculateTimeCoeff(self.release, fs)
        glin = self.smoothGain(gclin, alphaA, alphaR)

        # Returning the dB values of the gain reduction
        gs = gain_to_decibels(glin, -120)

        # Apply the dynamic range limiter
        y = x * glin

        return y, gs

    def applyLinearStaticCharacteristic(self, xa):
        ''' Public method to compute gain using static characteristic. Used for linear calculation. '''
        # Either 0 or 1 if over the threshold or not
        gclin = np.zeros_like(xa)
        gclin[xa >= self.thresholdLin] = 1
        return gclin

    def applydBStaticCharacteristic(self, xa):
        ''' Public method to compute gain using static characteristic. Used for decibel calculation. '''
        # Either 0 or 1 if over the threshold or not
        gc = np.zeros_like(xa)
        gc[xa < self.threshold] = -60
        return gc

    def smoothGain(self, gc, alphaA, alphaR):
        '''
          Public method to apply smoothing through interpolation onto the gain computer
          using the attack and release coefficients.
        '''
        glin = np.zeros_like(gc)

        # Initialize the first element of gs
        glin[0] = gc[0]

        # Hold counter for attack
        CA = 0

        # Perform smoothing using specified attack and release time parameters
        for n in range(1, len(gc)):
            if (CA > self.hold) and (gc[n] <= glin[n-1]):
                # Attack phase
                glin[n] = alphaA * glin[n-1] + (1 - alphaA) * gc[n]
            elif (CA <= self.hold):
                # Hold phase
                glin[n] = glin[n-1]
                CA += 1
            elif gc[n] > glin[n-1]:
                # Release phase
                glin[n] = alphaR * glin[n-1] + (1 - alphaR) * gc[n]

        return glin

    def calculateTimeCoeff(self, time, fs):
        '''
          Public method to calculate the time coefficients for both attack and release in an
          exponential form.
        '''
        return np.exp(-np.log(9) / (fs * time))
