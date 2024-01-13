classdef AudioDynamicRangeExpander < AudioDynamicRangeCompressor
    %AUDIODYNAMICRANGEEXPANDER Creates an audio dynamic range expander
    % class which can be used to perform an expander effect on audio
    
    properties
        hold
    end

    methods
        function obj = AudioDynamicRangeExpander(threshold, ratio, knee, hold, attack, release, makeupGainAuto, makeupGain)
            % Constructor to initialize the object with specified parameters
            %
            % Parameters
            % threshold: the dB threshold value that the compressor will
            %   start to take effect after passing
            % ratio: the ratio / ammount of compression to apply i.e. 4 =
            %   4:1
            % hold: the amount of time to hold the compression on after the
            %   attack phase and before the release phase
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
            obj = obj@AudioDynamicRangeCompressor(threshold, ratio, knee, attack, release, makeupGainAuto, makeupGain);
            obj.hold = hold;
        end

        function xsc = applyStaticCharacteristic(obj, xdB)
            % Public method to compute gain using static characteristic
            
            xsc = xdB;
            
            % To vectorise this operation, bitmasks are used instead of if
            % statements
            softKneeMask = (xdB >= (obj.threshold - obj.knee/2)) & (xdB <= (obj.threshold + obj.knee/2));
            
            xsc(softKneeMask) = xdB(softKneeMask) + ((1-obj.ratio)*((xdB(softKneeMask) - obj.threshold - obj.knee/2).^2) / (2 * obj.knee));
            
            elseMask = (xdB < (obj.threshold - obj.knee/2));
            xsc(elseMask) = obj.threshold + ((xdB(elseMask) - obj.threshold)*obj.ratio);
        end

        function gs = smoothGain(obj, gc, alphaA, alphaR)
            % Public method to apply smoothing through interpolation 
            % onto the gain computer using the attack and release
            % coefficients

            gs = zeros(size(gc));
            
            % Initialize the first element of gs
            gs(1) = gc(1);
            
            % Hold counter for attack
            CA = 0;

            % Perform smoothing using specified attack and release time parameters
            for n = 2:length(gc)
                if (CA > obj.hold) && (gc(n) <= gs(n-1))
                    % Attack phase
                    gs(n) = alphaA * gs(n-1) + (1 - alphaA) * gc(n);

                elseif (CA <= obj.hold)
                    % Hold phase
                    gs(n) = gs(n-1);
                    CA = CA + 1;

                elseif gc(n) > gs(n-1)
                    % Release phase
                    gs(n) = alphaR * gs(n-1) + (1 - alphaR) * gc(n);
                end
            end
        end
    end
end
