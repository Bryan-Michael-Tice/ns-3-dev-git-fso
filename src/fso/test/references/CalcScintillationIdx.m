function [ sigma_i ] = CalcScintillationIdx( zenith, lambda, Hsat, Hgs)
%SCINTILLATION_IDX Summary of this function goes here
%   Detailed explanation goes here

A = 1.7*10^(-14);
v = 21.0;
k = 2*pi/lambda;

%fun = @(x) exp(-x.^2).*log(x).^2;
fun = @(h) (A*exp(-Hgs/700)*exp(-(h-Hgs)/100)+ (1/(27*27))*(v^2)*(h.^10)*(5.94*10^(-53)).*(exp(-h/1000))+(2.7*10^(-16))*exp(-h/1500)).*((h-Hgs).^(5.0/6.0));
sigma_i = 2.25*(k^(7/6))*((1/cos(zenith))^(11/6))*integral(fun, Hgs, Hsat);

end

