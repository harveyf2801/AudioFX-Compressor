function plotAttackReleaseCharacteristics(attackTime, releaseTime, threshold)

    % Define the parameters
    frequency = 4; % in Hertz
    duration = attackTime+releaseTime+3; % in seconds
    sampling_rate = 1000; % number of samples per second

    thresholdG = decibelsToGain(threshold, -120);
    amp1 = thresholdG-(thresholdG*0.8);
    amp2 = thresholdG+(thresholdG*0.8);
    
    % Generate time vector
    t = linspace(0, duration, duration * sampling_rate);

    % Generate sine wave with variable amplitude
    amplitude = amp1*ones(size(t)); % Initialize amplitude to 1 for the entire duration
    
    % Change amplitude back to 1 for the second half-second
    amplitude(t > 1 & t <= 1+attackTime+1) = amp2;

    % Calculate the sine wave values
    y = amplitude .* sin(2 * pi * frequency * t);

    % Calculate the timings for the attack and release stages
    attackStart = 1;
    attackEnd = attackStart + attackTime;
    releaseStart = attackEnd + 1;
    releaseEnd = releaseStart + releaseTime;

    figure;

    subplot(3,1,1);

    hold on;
    
    % Plot the background for the attack and release stages
    fill([attackStart attackStart attackEnd attackEnd], [amp2 -amp2 -amp2 amp2], "m", 'EdgeColor', 'none', 'FaceAlpha', 0.3);
    fill([attackEnd attackEnd releaseStart releaseStart], [amp2 -amp2 -amp2 amp2], "r", 'EdgeColor', 'none', 'FaceAlpha', 0.3);
    fill([releaseStart releaseStart releaseEnd releaseEnd], [amp2 -amp2 -amp2 amp2], "y", 'EdgeColor', 'none', 'FaceAlpha', 0.3);

    % Plot the sine wave
    plot(t, y, 'b');

    % Plot the threshold line
    threshold_line = ones(size(t)) * thresholdG;
    plot(t, threshold_line, '--r', 'LineWidth', 1); % dashed red line
    plot(t, -threshold_line, '--r', 'LineWidth', 1); % dashed red line

    % Label the axes
    xlabel('Time (seconds)');
    ylabel('Amplitude');
    title('Input Signal');

    legend('Attack', 'Compression', 'Release', 'Sine Wave', 'Threshold', 'Location','eastoutside');

    % Display the grid
    grid on;

    hold off;

    subplot(3,1,2);
    hold on;

    % Calculate the gain reduction needed
    peak = gainToDecibels(amp2, -120);
    if (peak<threshold)
        xsc = peak;
    else
        xsc = threshold;
    end
    
    gainReduction = decibelsToGain(xsc-peak, -120);

    % Plot the background for the attack and release stages
    fill([attackStart attackStart attackEnd attackEnd], [1 gainReduction gainReduction 1], "m", 'EdgeColor', 'none', 'FaceAlpha', 0.3);
    fill([attackEnd attackEnd releaseStart releaseStart], [1 gainReduction gainReduction 1], "r", 'EdgeColor', 'none', 'FaceAlpha', 0.3);
    fill([releaseStart releaseStart releaseEnd releaseEnd], [1 gainReduction gainReduction 1], "y", 'EdgeColor', 'none', 'FaceAlpha', 0.3);

    % Plotting the basic linear envelope
    envelope = ones(size(t));
    envelope(t > attackStart & t <= attackEnd) = linspace(1, gainReduction, length(t(t > attackStart & t <= attackEnd)));
    envelope(t > attackEnd & t <= releaseStart) = gainReduction;
    envelope(t > releaseStart & t <= releaseEnd) = linspace(gainReduction, 1, length(t(t > releaseStart & t <= releaseEnd)));

    plot(t, envelope, 'b');

    % Adding labels and titles
    xlabel('Time (seconds)');
    ylabel('Amplitude');
    title('Envelope');

    % Adding a legend
    legend('Attack', 'Compression', 'Release', 'Envelope', 'Location','eastoutside');

    hold off;

    subplot(3,1,3);
    hold on;
    
    % Plot the background for the attack and release stages
    fill([attackStart attackStart attackEnd attackEnd], [amp2 -amp2 -amp2 amp2], "m", 'EdgeColor', 'none', 'FaceAlpha', 0.3);
    fill([attackEnd attackEnd releaseStart releaseStart], [amp2 -amp2 -amp2 amp2], "r", 'EdgeColor', 'none', 'FaceAlpha', 0.3);
    fill([releaseStart releaseStart releaseEnd releaseEnd], [amp2 -amp2 -amp2 amp2], "y", 'EdgeColor', 'none', 'FaceAlpha', 0.3);

    % Plot the sine wave
    plot(t, y .* envelope, 'b');

    % Plot the threshold line
    plot(t, threshold_line, '--r', 'LineWidth', 1); % dashed red line
    plot(t, -threshold_line, '--r', 'LineWidth', 1); % dashed red line

    % Label the axes
    xlabel('Time (seconds)');
    ylabel('Amplitude');
    title('Output Signal');

    legend('Attack', 'Compression', 'Release', 'Sine Wave', 'Threshold', 'Location', 'eastoutside');

    % Display the grid
    grid on;

    hold off;
end