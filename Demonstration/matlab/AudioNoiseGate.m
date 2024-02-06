classdef AudioNoiseGate
    %AUDIONOISEGATE Creates an audio noise gate
    % class which can be used to perform a noise gate effect on audio
    
    properties
        threshold           % Threshold parameter (in decibels)
        thresholdLin        % Threshold parameter (in magnitude)
        attack              % Attack time parameter
        release             % Release time parameter
        hold                % Hold time parameter
    end
    
    methods
        function obj = AudioNoiseGate(threshold, hold, attack, release)
            % Constructor to initialize the object with specified parameters
            %
            % Parameters
            % threshold: the dB threshold value that the compressor will
            %   start to take effect after passing
            % hold: the amount of time to hold the compression on after the
            %   attack phase and before the release phase
            % attack: the ammount of time it takes for the compression to
            %   start taking effect after passing the threshold
            % release: the ammount of time it takes for the compression to
            %   stop being applied after going above the threshold

            obj.threshold = threshold;
            obj.thresholdLin = decibelsToGain(threshold, -120);
            obj.attack = attack;
            obj.release = release;
            obj.hold = hold;
        end
        
        function [y, gs] = process(obj, x, fs, sidechain)
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

            % Calculate the absolute of the N-point signal (in magnitude)
            xa = abs(sidechain);
            
            % Compute the static characteristic with soft knee
            gclin = obj.applyLinearStaticCharacteristic(xa);
            
            % Smooth the gain using specified attack and release time parameters
            alphaA = obj.calculateTimeCoeff(obj.attack, fs);
            alphaR = obj.calculateTimeCoeff(obj.release, fs);
            
            glin = obj.smoothGain(gclin, alphaA, alphaR);
            
            % Returning the dB values of the gain reduction
            gs = gainToDecibels(glin, -120);
            
            % Apply the dynamic range limiter
            y = x .* glin;
        end
    end
    
    methods
        function gclin = applyLinearStaticCharacteristic(obj, xa)
            % Public method to compute gain using static characteristic
            % Used for linear calculation
            
            % Either 0 or 1 if over the threshold or not
            gclin = zeros(size(xa));
            gclin(xa >= obj.thresholdLin) = 1;
        end

        function gc = applydBStaticCharacteristic(obj, xa)
            % Public method to compute gain using static characteristic
            % Used for decibel calculation
            
            % Either 0 or 1 if over the threshold or not
            gc = zeros(size(xa));
            gc(xa < obj.threshold) = -60;
        end
        
        function glin = smoothGain(obj, gc, alphaA, alphaR)
            % Public method to apply smoothing through interpolation 
            % onto the gain computer using the attack and release
            % coefficients

            glin = zeros(size(gc));
            
            % Initialize the first element of gs
            glin(1) = gc(1);
            
            % Hold counter for attack
            CA = 0;

            % Perform smoothing using specified attack and release time parameters
            for n = 2:length(gc)
                if (CA > obj.hold) && (gc(n) <= glin(n-1))
                    % Attack phase
                    glin(n) = alphaA * glin(n-1) + (1 - alphaA) * gc(n);

                elseif (CA <= obj.hold)
                    % Hold phase
                    glin(n) = glin(n-1);
                    CA = CA + 1;

                elseif gc(n) > glin(n-1)
                    % Release phase
                    glin(n) = alphaR * glin(n-1) + (1 - alphaR) * gc(n);
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
