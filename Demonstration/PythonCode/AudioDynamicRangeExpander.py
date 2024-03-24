from PythonCode.AudioDynamicRangeCompressor import AudioDynamicRangeCompressor

class AudioDynamicRangeExpander(AudioDynamicRangeCompressor):
    '''
        Creates an audio dynamic range expander
        class which can be used to perform an expander effect on audio
    '''
    def __init__(self, threshold, ratio, knee, hold, attack, release, makeupGainAuto, makeupGain):
        '''
            Constructor to initialize the object with specified parameters
            
            Parameters -
            threshold: the dB threshold value that the compressor will
              start to take effect after passing
            ratio: the ratio / ammount of compression to apply i.e. 4 =
              4:1
            hold: the amount of time to hold the compression on after the
              attack phase and before the release phase
            knee: the smoothing width applied to the static characteristic
              after the input passes the threshold
            attack: the ammount of time it takes for the compression to
              start taking effect after passing the threshold
            release: the ammount of time it takes for the compression to
              stop being applied after going above the threshold
            makeupGainAuto: either true or false to turn auto makeup gain
              calculation on / off
            makeupGain: if manual makeup gain mode is on, the makeup gain to
              apply to the signal
        '''
        
        # Call superclass constructor
        super().__init__(threshold, ratio, knee, attack, release, makeupGainAuto, makeupGain)
        
        self.hold = hold

    def applyStaticCharacteristic(self, xdB):
        '''
            Public method to compute gain using static characteristic
        '''
        
        xsc = xdB.copy()

        # To vectorise this operation, bitmasks are used instead of if statements
        softKneeMask = (xdB >= (self.threshold - self.knee/2)) & (xdB <= (self.threshold + self.knee/2))
        xsc[softKneeMask] = xdB[softKneeMask] + ((1 - self.ratio) * ((xdB[softKneeMask] - self.threshold - self.knee/2)**2) / (2 * self.knee))

        elseMask = (xdB < (self.threshold - self.knee/2))
        xsc[elseMask] = self.threshold + ((xdB[elseMask] - self.threshold) * self.ratio)

        return xsc

    def smoothGain(self, gc, alphaA, alphaR):
        '''
            Public method to apply smoothing through interpolation
            onto the gain computer using the attack and release
            coefficients
        '''
        
        gs = gc.copy()
        
        # Initialize the first element of gs
        gs[0] = gc[0]

        # Hold counter for attack
        CA = 0

        # Perform smoothing using specified attack and release time parameters
        for n in range(1, len(gc)):
            if (CA > self.hold) and (gc[n] <= gs[n-1]):
                gs[n] = alphaA * gs[n-1] + (1 - alphaA) * gc[n]
            elif CA <= self.hold:
                gs[n] = gs[n-1]
                CA += 1
            elif gc[n] > gs[n-1]:
                gs[n] = alphaR * gs[n-1] + (1 - alphaR) * gc[n]

        return gs
