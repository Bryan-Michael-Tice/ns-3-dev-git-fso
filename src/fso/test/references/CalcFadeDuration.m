function [ fadeDuration ] = CalcFadeDuration( meanIrradiance, irradianceThreshold, scintillationIndex )
%CALCFADEDURATION Return the mean fade duration in seconds
% "Laser Beam Propagation through Random Media"

%return probability of fade over the expected number of fades
Ft = CalcFadeThreshold(meanIrradiance, irradianceThreshold);
numFades = CalcNumFades(scintillationIndex, Ft);
probFade = ProbabilityOfFade(scintillationIndex, Ft);

fadeDuration = (probFade/numFades); % (s)

end

