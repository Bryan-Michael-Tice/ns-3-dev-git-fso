function [ Ft ] = CalcFadeThreshold( meanIrradiance, irradianceThreshold )
%CALCFADETHRESHOLD Returns the Fade Threshold Parameter
%   The fade parameter (dB) is the level below the on-axis mean irradiance
%   that the irradiance threshold is set

Ft = 10*log10(meanIrradiance/irradianceThreshold);% (dB)

end

