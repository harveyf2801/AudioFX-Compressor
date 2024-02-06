classdef AudioDynamicRangeLimiter < AudioDynamicRangeCompressor
    %AUDIODYNAMICRANGELIMITER Creates an audio dynamic range limiter
    % class which can be used to perform a limiter effect on audio

    methods
        function obj = AudioDynamicRangeLimiter(threshold, knee, attack, release, makeupGainAuto, makeupGain)
            % Constructor to initialize the object with specified parameters
            %
            % Parameters
            % threshold: the dB threshold value that the compressor will
            %   start to take effect after passing
            % knee: the smoothing width applied to the static characteristic
            %   after the input passes the threshold
            % attack: the ammount of time it takes for the compression to
            %   start taking effect after passing the threshold
            % release: the ammount of time it takes for the compression to
            %   stop being applied after going above the threshold
            % makeupGainAuto: either true or false to turn auto makeup gain
            %   calculation on / off
            % makeupGain: if manual makeup gain mode is on, the makeup gain to
            %   apply to the signal

            % Call superclass constructor
            obj = obj@AudioDynamicRangeCompressor(threshold, inf, knee, attack, release, makeupGainAuto, makeupGain);
        end

        function xsc = applyStaticCharacteristic(obj, xdB)
            % Public method to compute gain using static characteristic
            
            xsc = xdB;
            
            % To vectorise this operation, bitmasks are used instead of if
            % statements
            softKneeMask = (xdB >= (obj.threshold - obj.knee/2)) & (xdB <= (obj.threshold + obj.knee/2));
            
            xsc(softKneeMask) = xdB(softKneeMask) - (((xdB(softKneeMask) - obj.threshold + obj.knee/2).^2) / (2 * obj.knee));
            
            xsc(xdB > (obj.threshold + obj.knee/2)) = obj.threshold;
        end
    end
end
