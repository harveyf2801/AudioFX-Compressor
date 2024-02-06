import matplotlib.pyplot as plt
import numpy as np

def plot_static_characteristic(static_characteristic_func):
    '''
        Generates a plot representing the static characteristic of a compressor.

        Parameters:
        - static_characteristic_func (function): A reference to the function which applies
          the compressor's static characteristic (@myCompressor.applyStaticCharacteristic)
    '''
    
    # Generate a linear ramp of input levels
    linear_ramp = np.arange(-60, 0.5, 0.5)
    
    # Calculate the compressor output for the linear ramp
    output = static_characteristic_func(linear_ramp)
    
    # Create a new figure
    plt.figure()

    # Plot the linear system and compressor characteristic
    plt.plot(linear_ramp, linear_ramp, linewidth=2, label='Linear System')
    plt.plot(linear_ramp, output, linewidth=2, label='Compressor Characteristic')

    # Adding a title and labels, and enabling the grid
    plt.title('Compression Characteristics')
    plt.xlabel('Input Level (dB)')
    plt.ylabel('Output Level (dB)')
    plt.grid(True)

    # Add legend
    plt.legend(loc='best')

    # Show the plot
    plt.show()
