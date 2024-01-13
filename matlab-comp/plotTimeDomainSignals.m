function plotTimeDomainSignals(originalSignal, wetSignal, Fs)
    % PLOTTIMEDOMAINSIGNALS plots one signal over the other in the time
    % domain, demonstrating any time feature that may have changed from
    % one signal to another.
    %
    % Parameters
    % originalSignal: the first signal to plot
    % wetSignal: the second signal to plot on top of the originalSignal
    % Fs: the sampling frequency of the audio
    
    % Create a time vector for plotting
    t = (0:length(originalSignal) - 1) / Fs;

    % Create a figure
    figure;

    % Plot the original signal in the background with reduced opacity
    plot(t, originalSignal, 'Color', [0.6, 0.6, 0.6], 'LineWidth', 1.5, 'DisplayName', 'Original Signal');
    hold on;

    % Plot the compressed signal in the foreground
    plot(t, wetSignal, 'b', 'LineWidth', 1.5, 'DisplayName', 'Compressed Signal');

    % Adding a title and labels, and enabling the grid
    title('Comparison of Original and Compressed Signals');
    xlabel('Time (s)');
    ylabel('Amplitude (magnitude)');
    grid on;

    % Add legend
    legend('Location', 'best');

    % Hold off to ensure the next plot does not overlap
    hold off;
end
