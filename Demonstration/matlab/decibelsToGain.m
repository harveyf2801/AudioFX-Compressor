function gain = decibelsToGain(decibels, minusInfinityDb)
    if (decibels > minusInfinityDb)
        gain = 10.0.^(decibels * 0.05);
    else
        gain = 0;
    end
end

