function [ W ] = CalcDiffractiveBeamRadius( lambda, distance, txBeamRadius, txPhaseFrontRadius )
%DIFFRACTIVE_BEAM_RADIUS Summary of this function goes here
% "Laser Beam Propagation through Random Media"


    theta0 = 1 - (distance/txPhaseFrontRadius);
    lambda0 = 2*distance/((2*pi/lambda)*txBeamRadius^2);
    W = txBeamRadius*sqrt(theta0^2 + lambda0^2);

end

