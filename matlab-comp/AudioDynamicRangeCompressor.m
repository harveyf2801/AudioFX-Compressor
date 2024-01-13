classdef AudioDynamicRangeCompressor
    %AUDIODYNAMICRANGECOMPRESSOR Creates an audio dynamic range compressor
    % class which can be used to perform a compressor effect on audio

    properties
        threshold           % Threshold parameter
        ratio               % Ratio Parameter (ISN'T NEEDED FOR LIMITER)
        knee                % Knee parameter
        attack              % Attack time parameter
        release             % Release time parameter
        %hold                % Hold time parameter (FOR EXPANSION ONLY)
        makeupGainAuto      % Boolean flag for automatic makeup gain
        makeupGain          % Makeup gain parameter (if auto mode is false)
    end
    
    methods
        function obj = AudioDynamicRangeCompressor(threshold, ratio, knee, attack, release, makeupGainAuto, makeupGain)
            % Constructor to initialize the object with specified parameters
            %
            % Parameters
            % threshold: the dB threshold value that the compressor will
            %   start to take effect after passing
            % ratio: the ratio / ammount of compression to apply i.e. 4 =
            %   4:1
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

            obj.threshold = threshold;
            obj.ratio = ratio;
            obj.knee = knee;
            obj.attack = attack;
            obj.release = release;
            obj.makeupGainAuto = makeupGainAuto;
            obj.makeupGain = makeupGain;
        end
        
        function [y, gm] = process(obj, x, fs, sidechain)
            % Public method to process the input signal
            % The entire processing chain is applied here
            %
            % Parameters
            % x: input signal
            % fs: sample rate of the input signal
            % sidechain (OPTIONAL): the sidechain input to use to calculate
            % gain reduction
            
            if ~exist('sidechain','var')
                 % If sidechain doesn't exist, use input as sidechain
                  sidechain = x;
            end
            
            % Calculate the N-point signal in decibels (providing a minimum dB value for -inf)
            xdB = gainToDecibels(abs(sidechain), -120);
            
            % Compute the static characteristic with soft knee
            xsc = obj.applyStaticCharacteristic(xdB);

            % Compute the gain
            gc = xsc - xdB;
            
            % Smooth the gain using specified attack and release time parameters
            alphaA = obj.calculateTimeCoeff(obj.attack, fs);
            alphaR = obj.calculateTimeCoeff(obj.release, fs);
            
            gs = obj.smoothGain(gc, alphaA, alphaR);

            % Calculate make-up gain
            if obj.makeupGainAuto
                % Calculate the auto makeup gain
                % Assuming target level is 0 dB, adjust as needed
                targetLevel = 0;
                
                % Peak level of the compressed signal
                compressedPeakLevel = max(gs);
                
                % Gain reduction applied by the compressor
                gainReduction = targetLevel - compressedPeakLevel;
                
                % Auto makeup gain calculation
                M = gainReduction;
            else
                M = obj.makeupGain;
            end
            
            % Add make-up gain to the smoothed gain
            gm = gs + M;
            
            % Translate gain from dB to linear domain
            glin = decibelsToGain(gm, -120);
            
            % Apply the dynamic range limiter
            y = x .* glin;
        end
    end
    
    methods
        function xsc = applyStaticCharacteristic(obj, xdB)
            % Public method to compute gain using static characteristic
            
            xsc = xdB;
            
            % To vectorise this operation, bitmasks are used instead of if
            % statements
            softKneeMask = (xdB >= (obj.threshold - obj.knee/2)) & (xdB <= (obj.threshold + obj.knee/2));
            
            xsc(softKneeMask) = xdB(softKneeMask) + (((1/obj.ratio)-1)*((xdB(softKneeMask) - obj.threshold + obj.knee/2).^2) / (2 * obj.knee));
            
            elseMask = (xdB > (obj.threshold + obj.knee/2));
            xsc(elseMask) = obj.threshold + ((xdB(elseMask) - obj.threshold)/obj.ratio);
        end
        
        function gs = smoothGain(~, gc, alphaA, alphaR)
            % Public method to apply smoothing through interpolation 
            % onto the gain computer using the attack and release
            % coefficients

            gs = zeros(size(gc));
            
            % Initialize the first element of gs
            gs(1) = gc(1);
            
            % Perform smoothing using specified attack and release time parameters
            for n = 2:length(gc)
                if gc(n) <= gs(n-1)
                    gs(n) = alphaA * gs(n-1) + (1 - alphaA) * gc(n);
                elseif gc(n) > gs(n-1)
                    gs(n) = alphaR * gs(n-1) + (1 - alphaR) * gc(n);
                end
            end
        end
        
        function alpha = calculateTimeCoeff(~, time, fs)
            % Public method to calculate the time coefficients for both
            % attack and realease in an exponential form
            alpha = exp(-log(9) / (fs * time));
        end
    end
end
