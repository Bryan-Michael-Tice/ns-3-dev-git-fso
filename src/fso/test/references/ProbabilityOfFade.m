function [ probFade ] = ProbabilityOfFade( scintillationIndex, Ft )
%PROBABILITYOFFADE Return the probability of a fade occuring
% "Laser Beam Propagation through Random Media"

errorFuncParam = (0.5*scintillationIndex - 0.23*Ft)/(sqrt(2*scintillationIndex));

errorFuncIntegrand = @(t) exp(-t^2);
probFade = 0.5*(1 + (2/sqrt(pi))*(integral(errorFuncIntegrand, 0, errorFuncParam)));

end

