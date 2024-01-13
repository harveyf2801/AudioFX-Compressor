function plotStaticCharacteristic(staticCharacteristicFunc)
    % PLOTSTATICCHARACTERISTIC generates a plot representing the static 
    % characteristic of a compressor.
    %
    % Parameters
    % staticCharacteristicFunc: a reference to the function which applies
    %   the compressors static characteristic
    %   (@myCompressor.applyStaticCharacteristic)
    
    % Generate a linear ramp of input levels
    linearRamp = -60:0.5:0;
    
    % Calculate the compressor output for the linear ramp
    output = staticCharacteristicFunc(linearRamp);
    
    % Create a new figure
    figure;
    
    % Plot the linear system and compressor characteristic
    hold on;
    plot(linearRamp, linearRamp, 'LineWidth', 2, 'DisplayName', 'Linear System');
    plot(linearRamp, output, 'LineWidth', 2, 'DisplayName', 'Compressor Characteristic');
    
    % Adding a title and labels, and enabling the grid
    title('Compression Characteristics');
    xlabel('Input Level (dB)');
    ylabel('Output Level (dB)');
    grid on;
    
    % Add legend
    legend('Location', 'best');
    
    % Hold off to ensure the next plot does not overlap
    hold off;
end