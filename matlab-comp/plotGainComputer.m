function plotGainComputer(g)
    % PLOTGAINCOMPUTER plots the output of the gain computer of a compressor.
    %
    % Parameters
    % g: representing the gain values over time
    
    % Create a new figure
    figure;
    
    % Plot the gain values
    plot(g, 'LineWidth', 2);
    
    % Adding a title and labels, and enabling a grid
    title('Compressor Gain Computer Output');
    xlabel('Time (s)');
    ylabel('Gain Reduction (dB)');
    ylim([-35, 0]);
    grid on;
    
    % Add legend
    legend('Gain Computer Output', 'Location', 'southeast');
    
    % Hold off to ensure the next plot does not overlap
    hold off;
end
