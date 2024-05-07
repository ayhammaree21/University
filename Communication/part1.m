% Ac=16;
% u=0.22;
% fm=800;
% fc=10000;
%  t=0:0.01:10;
% S=Ac*(1+u*cos(2*pi*fm*t)).*cos(2*pi*fc*t);
% plot(t,S,'r')
% %axis([0 20 -20 20])
% ylabel('S(t)');
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
% hold on;
%plot(t,s2,'b');
%plot(t,s2,'color','b','LineWidth',1,'LineStyle',"-");
% xlabel('Time');
% ylabel('amplitude');
% hold off;
% grid on;


