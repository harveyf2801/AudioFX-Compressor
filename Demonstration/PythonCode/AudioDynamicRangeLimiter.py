from PythonCode.AudioDynamicRangeCompressor import AudioDynamicRangeCompressor

class AudioDynamicRangeLimiter(AudioDynamicRangeCompressor):
    '''
        Creates an audio dynamic range limiter
        class which can be used to perform a limiter effect on audio
    '''

    def __init__(self, threshold, knee, attack, release, makeupGainAuto, makeupGain):
        '''
            Constructor to initialize the object with specified parameters

            Parameters:
            - threshold: the dB threshold value that the compressor will
              start to take effect after passing
            - knee: the smoothing width applied to the static characteristic
              after the input passes the threshold
            - attack: the amount of time it takes for the compression to
              start taking effect after passing the threshold
            - release: the amount of time it takes for the compression to
              stop being applied after going above the threshold
            - makeupGainAuto: either True or False to turn auto makeup gain
              calculation on / off
            - makeupGain: if manual makeup gain mode is on, the makeup gain to
              apply to the signal
        '''

        # Call superclass constructor
        super().__init__(threshold, float('inf'), knee, attack, release, makeupGainAuto, makeupGain)

    def applyStaticCharacteristic(self, xdB):
        ''' Public method to compute gain using static characteristic '''

        xsc = xdB.copy()

        # To vectorize this operation, bitmasks are used instead of if statements
        softKneeMask = (xdB >= (self.threshold - self.knee/2)) & (xdB <= (self.threshold + self.knee/2))
        xsc[softKneeMask] = xdB[softKneeMask] - (((xdB[softKneeMask] - self.threshold + self.knee/2)**2) / (2 * self.knee))

        xsc[xdB > (self.threshold + self.knee/2)] = self.threshold

        return xsc
