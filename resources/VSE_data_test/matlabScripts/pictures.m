

clear all
close all
A = load('output_ekf_cov.csv');

figure(1)
axes('Position', [0.1, 0.1, 0.85, 0.85],'fontname','times','FontSize',24)
hold on

plot (A(:,4),A(:,5),'+b')
hold on
plot (A(:,10),A(:,11),'.r')
plot (A(:,1),A(:,2),'g', 'LineWidth', 2)
xlabel('x [m]','fontname','times','FontSize',24,'FontWeight','b')
ylabel('y [m]','fontname','times','FontSize',24,'FontWeight','b')
legend('Noisy measures Z_m','Estimation X_o','Ground Truth','fontname','times','FontSize',24,'FontWeight','b')
set(gcf, 'units','normalized','outerposition',[0 0 1 1])