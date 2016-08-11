function [ I_mean ] = CalcMeanIrradiance( txBeamRadius, WLT, r )
%MEAN_IRRADIANCE Compute the mean irradiance
% Chapter 4 of "Laser Beam Propagation through Random Media" 
I_mean = ((txBeamRadius.^2)/(WLT.^2)).*exp(-2*(r.^2)/(WLT.^2));

end

