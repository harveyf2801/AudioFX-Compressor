import numpy as np

def gain_to_decibels(gain, minus_infinity_db):
    '''
        Convert gain to decibels.

        Parameters -
        gain (float): The gain value to be converted.
        minus_infinity_db (float): The value representing minus infinity in decibels.
    '''

    # Vectorised
    if type(gain) is np.ndarray:
        decibels = np.ones(gain.shape) * minus_infinity_db
        mask = gain > 0
        decibels[mask] = np.maximum(minus_infinity_db, np.log10(gain[mask]) * 20.0)

    else:
        # Check if gain is greater than zero
        if gain > 0:
            # Calculate decibels using the formula: decibels = 20 * log10(gain)
            decibels = max(minus_infinity_db, np.log10(gain) * 20.0)
        else:
            # If gain is less than or equal to zero, set decibels to minus_infinity_db
            decibels = minus_infinity_db

    return decibels