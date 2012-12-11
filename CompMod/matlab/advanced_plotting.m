%----------------------------------------------------------------------------------
% Matlab example file advanced_plotting.m
%
% This script plots a simple 2D graph to screen and to file as eps and png.
% Very many plotting options are set explicitly in order to demonstrate
% the required get/set commands for customisation.
%
% Andreas Freise 20.09.2011
%----------------------------------------------------------------------------------

% reset matlab, clear all data and all variables
clear all;

%Some default settings which should really be in your startup file
% set page properties for printing
set(0,'DefaultFigurePaperUnits','centimeters');
set(0,'DefaultAxesFontSize',15);
set(0,'DefaultAxesFontWeight','bold');
set(0,'DefaultAxesFontName','helvetica')
set(0,'DefaultAxesLineWidth', 1.0);
set(0,'DefaultLineLineWidth', 2.0);
set(0,'defaultlinemarkersize',6)
set(0,'DefaultAxesGridLineStyle','-');
set(0,'DefaultAxesMinorGridLineStyle','--');
set(0,'DefaultAxesXColor',[0 0 0]);
set(0,'DefaultAxesYColor',[0 0 0]);
set(0,'defaultfigurenumbertitle','on');
set(0,'DefaultAxesLineWidth',0.5)
set(0,'DefaultFigureColor', 'w');
set(0,'DefaultFigurePosition', [100 100 800 500]);

ThinLineStyles=1;

% Creating some dummy data
x=linspace(1,100);

% Now creating the actual plot
figure
plot(x,x.*x);

% Customising the plot

set(gca, 'XLim', [1 80]);
grid on;

% Titles and subtitles can be set like this, but should not be used when exporting for a report.
% (in the report a full figure caption is required anyway).
%title('Mytitle'); 
%title(char('Mytitle','mysubtital'));

set(get(gcf,'CurrentAxes'),'FontName','Helvetica', 'FontWeight','normal', 'FontAngle', 'normal', 'FontSize',12)

L=legend('Measured phase noise');
set(L, 'FontName','Helvetica','FontWeight','normal', 'FontAngle', 'normal', 'FontSize',10) 

xlabel('Frequency [Hz]');
ylabel('Phase [rad/sqrt(Hz)]');

set(get(gca,'XLabel'), 'FontName','Helvetica','FontWeight','normal','FontAngle', 'normal', 'FontSize',12);
set(get(gca,'YLabel'), 'FontName','Helvetica','FontWeight','normal','FontAngle', 'normal', 'FontSize',12);

set(get(gcf,'CurrentAxes'),'XMinorTick','on','XMinorGrid','off')
set(get(gcf,'CurrentAxes'),'YMinorTick','on','YMinorGrid','off')

set(gcf,'PaperUnits','centimeters')
set(gcf,'PaperPosition',[2.5 14 18 12])

% output filename for eps and png plots
filename='advanced_plot';

% To creat a pdf file, plot as eps and then use `epstopdf yourfile.eps' on phymat to creatae the pdf
eval(['print -depsc2 ',filename,'.eps']); 

% now for the png plot which will probably be used
% in a talk we can set some fonts differently, e.g. bold

set(get(gcf,'CurrentAxes'),'FontName','Helvetica', 'FontWeight','bold', 'FontAngle', 'normal', 'FontSize',12)
set(legend, 'FontName','Helvetica','FontWeight','normal', 'FontAngle', 'normal', 'FontSize',10) 
set(get(gca,'XLabel'), 'FontName','Helvetica','FontWeight','normal','FontAngle', 'normal', 'FontSize',12);
set(get(gca,'YLabel'), 'FontName','Helvetica','FontWeight','normal','FontAngle', 'normal', 'FontSize',12);
set(get(gca,'YLabel'), 'FontName','Helvetica','FontWeight','normal','FontAngle', 'normal', 'FontSize',12);

% Plot png file
eval(['print -dpng ',filename,'.png']);


