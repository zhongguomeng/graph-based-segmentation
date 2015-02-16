function G = GaussianKernel(sigma,length)

x = -1*length:1:length;
y = -1*length:1:length;

x = exp(-x.^2/(2*sigma));
y = exp(-y.^2/(2*sigma));

G = 1/(2*pi*sigma)*(x'*y);
G = G/sum(sum(G));

