import numpy as np
from PythonCode.decibelsToGain import decibels_to_gain
from PythonCode.gainToDecibels import gain_to_decibels

class AudioDynamicRangeCompressor:
    '''
        Creates an audio dynamic range compressor
        class which can be used to perform a compressor effect on audio
    '''
    def __init__(self, threshold, ratio, knee, attack, release, makeupGainAuto, makeupGain):
        '''
            Constructor to initialize the object with specified parameters
            
            Parameters -
            threshold: the dB threshold value that the compressor will
              start to take effect after passing
            ratio: the ratio / amount of compression to apply i.e. 4 =
              4:1
            knee: the smoothing width applied to the static characteristic
              after the input passes the threshold
            attack: the amount of time it takes for the compression to
              start taking effect after passing the threshold
            release: the amount of time it takes for the compression to
              stop being applied after going above the threshold
            makeupGainAuto: either true or false to turn auto makeup gain
              calculation on / off
            makeupGain: if manual makeup gain mode is on, the makeup gain to
              apply to the signal
        '''
        self.threshold = threshold
        self.ratio = ratio
        self.knee = knee
        self.attack = attack
        self.release = release
        self.makeupGainAuto = makeupGainAuto
        self.makeupGain = makeupGain

    def process(self, x, fs, sidechain=None):
        '''
            Public method to process the input signal
            The entire processing chain is applied here
            
            Parameters
            x: input signal
            fs: sample rate of the input signal
            sidechain (OPTIONAL): the sidechain input to use to calculate
             gain reduction
        '''

        if sidechain is None:
            # If sidechain doesn't exist, use input as sidechain
            sidechain = x.copy()

        # Calculate the N-point signal in decibels (providing a minimum dB value for -inf)
        xdB = gain_to_decibels(np.abs(sidechain), -120)

        # Compute the static characteristic with soft knee
        xsc = self.applyStaticCharacteristic(xdB)

        # Compute the gain
        gc = xsc - xdB

        # Smooth the gain using specified attack and release time parameters
        alphaA = self.calculateTimeCoeff(self.attack, fs)
        alphaR = self.calculateTimeCoeff(self.release, fs)

        gs = self.smoothGain(gc, alphaA, alphaR)

        # Calculate make-up gain
        if self.makeupGainAuto:
            # Calculate the auto makeup gain
            # Assuming target level is 0 dB, adjust as needed
            targetLevel = 0

            # Peak level of the compressed signal
            compressedPeakLevel = np.max(gs)

            # Gain reduction applied by the compressor
            gainReduction = targetLevel - compressedPeakLevel

            # Auto makeup gain calculation
            M = gainReduction
        else:
            M = self.makeupGain

        # Add make-up gain to the smoothed gain
        gm = gs + M

        # Translate gain from dB to linear domain
        glin = decibels_to_gain(gm, -120)

        # Apply the dynamic range limiter
        y = x * glin

        return y, gm

    def applyStaticCharacteristic(self, xdB):
        ''' Public method to compute gain using static characteristic '''

        xsc = xdB.copy()

        # To vectorize this operation, bitmasks are used instead of if statements
        softKneeMask = np.logical_and(xdB >= (self.threshold - self.knee/2),
                                      xdB <= (self.threshold + self.knee/2))

        xsc[softKneeMask] = xdB[softKneeMask] + (((1/self.ratio)-1)*((xdB[softKneeMask] - self.threshold + self.knee/2)**2) / (2 * self.knee))

        elseMask = xdB > (self.threshold + self.knee/2)
        xsc[elseMask] = self.threshold + ((xdB[elseMask] - self.threshold)/self.ratio)

        return xsc

    def smoothGain(self, gc, alphaA, alphaR):
        '''
            Public method to apply smoothing through interpolation
            onto the gain computer using the attack and release coefficients
        '''

        gs = np.zeros_like(gc)

        # Initialize the first element of gs
        gs[0] = gc[0]

        # Perform smoothing using specified attack and release time parameters
        for n in range(1, len(gc)):
            if gc[n] <= gs[n-1]:
                gs[n] = alphaA * gs[n-1] + (1 - alphaA) * gc[n]
            elif gc[n] > gs[n-1]:
                gs[n] = alphaR * gs[n-1] + (1 - alphaR) * gc[n]

        return gs

    def calculateTimeCoeff(self, time, fs):
        '''
            Public method to calculate the time coefficients for both
            attack and release in an exponential form
        '''
        alpha = np.exp(-np.log(9) / (fs * time))
        return alpha