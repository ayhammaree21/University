% Ac=1;
% u=0.25;
% fm=1;
% syms t; 
% A=Ac.*(1+u.*cos(2*pi.*fm.*t));
% fplot(A,'r')
% axis([0 2 0 1.4])
% ylabel('A(t)');
% xlabel('t(sec)');

ac=16;
fm=800;
fc=10000;
m=0.22;
t=0:0.01:10;
p=3.14;
s=abs(ac+ac*m*cos(2*p*fm));
s2=(ac+ac*m*cos(2*p*fm*t)).*cos(2*p*fc*t);
plot(t,s,'r');
hold on;
plot(t,s2,'b');
plot(t,s2,'color','b','LineWidth',1,'LineStyle',"-");
xlabel('Time');
ylabel('amplitude');
hold off;
grid on;