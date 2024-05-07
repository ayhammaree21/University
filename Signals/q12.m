t2 = -15:0.01:15; % time
nval = floor((max(abs(t2))+1)/3)+1; % required limit of n
Y = 0; % initialize the pulse train signal
for n=-nval:nval % values of n for the range of t
Yn=rectangularPulse((t2-3*n)/2); % rectangular pulse
Y=Y+Yn; % performs summation
end
% plot Y(t) pulse train
figure(2)
axis([-20 20 -2 2])
plot(t2,Y)
xlabel('t')
ylabel('Y(t)')
grid on