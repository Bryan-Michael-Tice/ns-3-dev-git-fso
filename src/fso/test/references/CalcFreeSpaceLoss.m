function [ loss ] = CalcFreeSpaceLoss( d, lambda )
%Calculate the free space path loss for an electromagnetic wave (far-field)
loss = 20*log10(4*pi*d/lambda);
end

