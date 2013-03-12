// 07 Dec 2011 - PGJ - This version generalised to work for a physical x-axis. Removed Parameter limits on peak counts. Limits on centroid position set to range of zoomed histogram.

TH1I *histogram, *zoom, *diff;
TCanvas* c1;
TVirtualPad* p1;
TF1 *fit, *fitg[4];
TString *hname;
char line[80];
int min, max, ftype, nbin;
int first, last;
int const MaxPeak=4;
float par[3][MaxPeak];
float parerr[3][MaxPeak];
int const MaxPoly=4;
float poly[MaxPoly];
float polyerr[MaxPoly];
int nback;
int AutoFlag = 0;
int BuffFlag = 1;
int DiffFWHM = 1;
int FixWD = 0;
int FixCN = 0;
int ClickX;

void PeakFit() {
  char filename[80];
  printf("Enter filename:\n");
  scanf("%s",filename);
  PeakFit(filename);
}

void PeakFit(char *filename) {
  TStyle *myStyle = new TStyle("myStyle","PGJs Style");
  myStyle->SetOptStat(0);
  myStyle->SetOptFit(0110);
  myStyle->SetTitleBorderSize(0);
  myStyle->SetTitleAlign(13);
  myStyle->SetCanvasColor(0);
  myStyle->SetTitleFillColor(0);
  myStyle->SetLineWidth(2);
  gROOT->SetStyle("myStyle");
  //  gSystem->Load("buffit/libbuffit.so");  // Local version
  gSystem->Load("/usr/local/share/physics/labs/y3/np/Buffit/buffit/libbuffit.so"); // WebCT version

  if(ReadFile(filename)!=0)
    PeakFit();
}

int ReadFile(const char* filename) {

  FILE *f = fopen(filename,"r");
  if (f == NULL) {
    sprintf(line,"File %s not found.\n",filename);
    printf(line);
    return -1;
  }

  hname = new TString(filename);

  // Look for SPECTRUM header
  ftype = 0;
  while (fscanf(f,"%s",line) == 1 ) {
    if( !strcmp(line,"SPECTRUM") ) {
      // Data follows
      printf("ASCII Export filetype.\n");
      min = 1;
      max = 2049;
      ftype = 1;
      break;
    }
    else if( !strcmp(line,"$DATA:") ) {
      printf("ASCII Save As filetype.\n");
      fscanf(f,"%d",&min);
      fscanf(f,"%d",&max);
      // channel numbers start from zero
      min++;
      // require upper bin edge
      max += 1;    
      ftype = 2;
      break;
    }
  } 

  //  printf("%s\n",line);

  if( ftype == 0 ) {
    printf("Header not found.\n");
    return -1;
  }

  c1 = (TCanvas*) gROOT->Get("c1");
  if( c1 == NULL ) c1 = new TCanvas("c1","c1",1000,600);

  nbin = max - min;
  histogram = (TH1I*) gROOT->Get("spectrum");
  if( histogram == NULL ) histogram = new TH1I("spectrum",filename,nbin,min,max);

  // Now read the data
  int icounts;
  switch(ftype) {
  case 1:
    int ichannel=0;
    // Read first item (channel number) and discard
    while (fscanf(f,"%s",line) != EOF ) {
      for(int i=0; i<5; i++) {
        if( fscanf(f,"%d",&icounts) != EOF ) {
	  histogram->SetBinContent(ichannel++,icounts);
        }  
      }
    }
    break;
  case 2:
    // Read first item (channel number) and discard
    for(int i=min; i<max; i++) {
      if( fscanf(f,"%d",&icounts) != EOF ) {
        histogram->SetBinContent(i,icounts);
      }  
    }
    break;
  }

  fclose(f);

  first = min;
  last = max;

  zoom = (TH1I*) histogram->Clone();
  zoom->SetName("zoom");
  zoom->SetLineColor(kRed);
  zoom->SetLineWidth(2);
  zoom->GetXaxis()->SetNdivisions(505);
  zoom->SetXTitle("Channels");
  zoom->Draw();     // NB. Only histo zoom is ever drawn
  c1->Update();
  p1 = c1->cd(1);   // Set pointer to pad
  return 0;
}

void Zoom() {
  printf("Enter range for fit (min and max channel):\n");
  scanf("%d",&first);
  //  printf("Enter maximum channel:\n",i);
  scanf("%d",&last);
  ZoomDraw();
}

void SetFixedFWHM() {
  DiffFWHM = 0;
  printf("Warning: using SAME width (FWHM) for all Gaussian peaks.\n");
}

void UnSetFixedFWHM() {
  DiffFWHM = 1;
  printf("Warning: using VARIABLE width (FWHM) for all Gaussian peaks.\n");
}

void SetFixedCentroid() {
  FixCN = 1;
  printf("Warning: using FIXED centroid positions for all Gaussian peaks.\n");
}

void UnSetFixedCentroid() {
  FixCN = 0;
  printf("Warning: using VARIABLE centroid positions for all Gaussian peaks.\n");
}

void Zoom(int minX, int maxX) {
  first = minX;
  last = maxX;
  ZoomDraw();
}

void ZoomIn() {
  c1->AddExec("MyExec","GetX()");
  printf("Click on the histogram canvas to bring it into focus.\n");
  printf("Then click to select the range you wish to expand.\n");
  int i=0;
  while( i<2 ) {
    p1->WaitPrimitive();
    switch (i) {
    case 0:
      first = ClickX;
    case 1: 
      last = ClickX;
    }    
    i++;
  }
  if( first>last ) {
    int temp = first;
    first = last;
    last = first;
  }
//  printf("first=%d, last=%d\n",first,last);
  gSystem->Sleep(1000);
  c1->DeleteExec("MyExec");
  ZoomDraw();
}

void GetX() {
  int event = p1->GetEvent();
  int px = p1->GetEventX();
//  selected = p1->GetSelected();
//  if( !selected ) return;
  switch(event) {
  case 1:  // Left click
//    if( selected->InheritsFrom("TH1") ) { 
      Float_t xx = p1->AbsPixeltoX(px);
      Float_t x  = c1->PadtoX(xx);
      Float_t ymax = zoom->GetMaximum();
      TArrow* a1 = new TArrow(x,ymax,x,0,0.02,"|->");
      a1->Draw();
      c1->Update();
//      printf("Left click at x=%f\n",x);
      ClickX = x;
//    }
  default:
  }
}

void ZoomOut() {
  first = min;
  last = max;
  ZoomDraw();
}

void ZoomDraw() {
  int nchannels = last-first+1;
  histogram = (TH1I*) gROOT->Get("spectrum");
  zoom = (TH1I*) gROOT->Get("zoom");
  if( zoom != NULL ) zoom->Delete();
  zoom = new TH1I("zoom",hname->Data(),nchannels,first,last);
  for(int i=0; i<nchannels; i++) {
    zoom->SetBinContent(i+1,histogram->GetBinContent(i+first));
  }
  zoom->SetLineColor(kRed);
  zoom->SetLineWidth(2);
  zoom->GetXaxis()->SetNdivisions(505);
  zoom->SetXTitle("Channels");
  zoom->Draw();
  c1->Update();
}

void Buffit() {
  double spec[8192];
  double centin[MaxPeak], fwhmin[MaxPeak];
  double centr[MaxPeak], area[MaxPeak], fwhm[MaxPeak], bakfit[MaxPoly];
  double cenerr[MaxPeak], arerr[MaxPeak], fwerr[MaxPeak], bakerr[MaxPoly];
  double centi;
  int fwhmi, npeak;
  int nchan = last-first+1;
  histogram = (TH1I*) gROOT->Get("spectrum");
  nchan = histogram->GetNbinsX();
  for(int i=0; i<nchan; i++) {
    spec[i] = histogram->GetBinContent(i+1);
  }

  if( !DiffFWHM ) 
    printf("Warning: using SAME width (FWHM) for all Gaussian peaks.\n");

  printf("Enter a rough value of the FWHM in channels:\n");
  scanf("%d",&fwhmi);
//  printf("Enter order of polynomial background (2=line):\n");
//  scanf("%d",&nback);
  nback = 2; // Drawing functions expect 2nd order background
  if (AutoFlag) {
    ForwardDiff(fwhmi);
    npeak = PeakSearch();
    printf("Attempting to fit %d peaks\n",npeak);
    for(int i=0; i<npeak; i++) {
      centin[i] = par[0][i];
      fwhmin[i] = par[1][i];
    }
  } else {
    printf("Enter the number of peaks (Max=%d):\n",MaxPeak);
    scanf("%d",&npeak);
	if (FixCN) {
		int i=0;
		while( i<npeak ) {
			printf("Enter centroid for peak %d:\n",i+1);
			scanf("%lf",&centi);
			centin[i] = centi;
			fwhmin[i] = fwhmi;
			i++;
		}
	} else {
		c1->AddExec("MyExec","GetX()");
		printf("Click on the histogram canvas to bring it into focus.\n");
		printf("Then click to select the peaks you wish to fit.\n");
		int i=0;
		while( i<npeak ) {
			p1->WaitPrimitive();
			centin[i] = ClickX;
			fwhmin[i] = fwhmi;
			i++;
		}
	}
    c1->DeleteExec("MyExec");
  }
  if (BuffFlag) {
    buffit *b = new buffit();
    b->fit(nchan,spec,npeak,nback,first,last,centin,fwhmin,centr,area,fwhm,bakfit,cenerr,arerr,fwerr,bakerr,DiffFWHM,FixWD,FixCN);
    for(int i=0; i<npeak; i++) {
      par[0][i] = centr[i];
      par[1][i] = fwhm[i];
      par[2][i] = area[i];
      parerr[0][i] = cenerr[i];
      parerr[1][i] = fwerr[i];
      parerr[2][i] = arerr[i];
    }
    for(int i=0; i<nback; i++) {
      poly[i] = bakfit[i];
      polyerr[i] = bakerr[i];
    } 
    // Correct for background reported at x=first rather than x=0
    poly[0] = poly[0] - first*poly[1];
  } else {
    for(int i=0; i<npeak; i++) {
      par[0][i] = centin[i];
      par[1][i] = fwhmin[i];
      par[2][i] = histogram->GetBinContent(centin[i])*fwhmin[i]/2.;
      printf("Peak %d, centroid %d, fwhm %d, counts %d\n",i+1,par[0][i],par[1][i],par[2][i]);
     }
  }
  Fit(npeak);
}

void AutoFit() {
  int fwhm;
  printf("Enter a rough value of the FWHM:\n");
  scanf("%d",&fwhm);
  AutoFit(fwhm);
}

void AutoFit(int fwhm) {
  int npeak = 0;
  ForwardDiff(fwhm);
  npeak = PeakSearch();
  printf("Attempting to fit %d peaks\n",npeak);
  Fit(npeak);
}

void ManualFit() {
  int numpeak;
  printf("Enter the number of peaks you want to fit:\n");
  scanf("%d",&numpeak);
  // Get parameters for the peaks
  for (int i=0; i<numpeak; i++) {
    sprintf(line,"Peak %d, Enter centroid, fwhm, counts:\n",i);
    printf(line);
    scanf("%f",&(par[0][i]));
//    sprintf(line,"Enter the FWHM of peak %d in channels:\n",i);
//    printf(line);
    scanf("%f",&(par[1][i]));
//    sprintf(line,"Enter the approx number of counts in peak %d:\n",i);
//    printf(line);
    scanf("%f",&(par[2][i]));
  }
  Fit(numpeak);
}

void ForwardDiff(int fwhm) {
  int nchannels = last-first+1;
  int offset = fwhm/2;
  histogram = (TH1I*) gROOT->Get("spectrum");
  diff = (TH1I*) gROOT->Get("diff");
  if( diff != NULL ) diff->Delete();
  diff = new TH1I("diff",filename,nchannels,first,last);
  for(int i=0; i<nchannels-fwhm; i++) {
    diff->SetBinContent(i+offset+1,histogram->GetBinContent(i+first) - 
                                   histogram->GetBinContent(i+fwhm+first));
  }  
  diff->Draw();
  c1->Update();
}

int PeakSearch() {
  int ndiff, ncounts, npeak=0; 
  int peaktotal=0, peakwidth=0;
  int nchannels = last-first+1;
  int PeakStart=0, centroid;
  diff = (TH1I*) gROOT->Get("diff");
  histogram = (TH1I*) gROOT->Get("spectrum");
  for(int i=0; i<nchannels; i++) {
    ndiff = diff->GetBinContent(i);
    if( PeakStart==1 ) {
      if( centroid==0  && ndiff>75 ) {
	printf("Found centroid at channel %d\n",first+i);
        centroid = first+i;   
      } elseif( centroid!=0 && ndiff<75 ) { 
	printf("Ending peak at channel %d\n",first+i);
        printf("Centroid %d\n",centroid);
        printf("Peak width %d\n",peakwidth);
        printf("Peak total %d\n",peaktotal);
	if( npeak<MaxPeak ) {
          par[0][npeak] = centroid;
          par[1][npeak] = peakwidth/2;
          par[2][npeak] = peaktotal;
    	  npeak++;
        } else {
          printf("WARNING: MaxPeak exceeded!\n");
        }
	PeakStart=0;
      } else {
	peakwidth++;
	peaktotal += histogram->GetBinContent(first+i);
      }
    } else {
      if( ndiff < -75 ) {
	printf("Starting peak at channel %d\n",first+i);
	peaktotal = 0;
	peakwidth = 0;
        centroid=0;
	PeakStart=1;
      }
    }
  }
  return npeak;
}

void Fit() {
  int numpeak;
  // Get parameters for the peaks
  printf("Enter the number of peaks you want to fit:\n");
  scanf("%d",&numpeak);
  Fit(numpeak);
}

void Fit(int numpeak) {
  int offset;
  float centroid, fwhm, ncounts;

  switch(numpeak) {
  case 1:
    fit = new TF1("fit1g",fit1g,first,last,5);
    fit->SetParNames("Centroid","FWHM","Counts","Intercept","Slope");
    fit->SetParLimits(0,first,last);
    fit->SetParLimits(1,0,500.);
    //    fit->SetParLimits(2,50.,999999999.);
    break;
  case 2:
    if( !DiffFWHM ) {
      fit = new TF1("fit2gw",fit2gw,first,last,7);
      fit->SetParNames("1-Centroid","1-FWHM","1-Counts",
	   	       "2-Centroid","2-Counts",
		       "Intercept","Slope");
      fit->SetParLimits(0,first,last);
      fit->SetParLimits(1,0,500.);
      //      fit->SetParLimits(2,50.,999999999.);
      fit->SetParLimits(3,first,last);
      //      fit->SetParLimits(4,50.,999999999.);
    } else {
      fit = new TF1("fit2g",fit2g,first,last,8);
      fit->SetParNames("1-Centroid","1-FWHM","1-Counts",
	   		"2-Centroid","2-FWHM","2-Counts",
		     	"Intercept","Slope");
      fit->SetParLimits(0,first,last);
      fit->SetParLimits(1,0,500.);
      //      fit->SetParLimits(2,50.,999999999.);
      fit->SetParLimits(3,first,last);
      fit->SetParLimits(4,0,500.);
      //      fit->SetParLimits(5,50.,999999999.);
    }
    break;
  case 3:
    if( !DiffFWHM ) {
      fit = new TF1("fit3gw",fit3gw,first,last,9);
      fit->SetParNames("1-Centroid","1-FWHM","1-Counts",
	   	       "2-Centroid","2-Counts",
		       "3-Centroid","3-Counts",
		       "Intercept","Slope");
      fit->SetParLimits(0,first,last);
      fit->SetParLimits(1,0,500.);
      //      fit->SetParLimits(2,50.,999999999.);
      fit->SetParLimits(3,first,last);
      //      fit->SetParLimits(4,50.,999999999.);
      fit->SetParLimits(5,first,last);
      //      fit->SetParLimits(6,50.,999999999.);
    } else {
      fit = new TF1("fit3g",fit3g,first,last,11);
      fit->SetParNames("1-Centroid","1-FWHM","1-Counts",
	   	       "2-Centroid","2-FWHM","2-Counts",
		       "3-Centroid","3-FWHM","3-Counts",
		       "Intercept","Slope");
      fit->SetParLimits(0,first,last);
      fit->SetParLimits(1,0,500.);
      //      fit->SetParLimits(2,50.,999999999.);
      fit->SetParLimits(3,first,last);
      fit->SetParLimits(4,0,500.);
      //      fit->SetParLimits(5,50.,999999999.);
      fit->SetParLimits(6,first,last);
      fit->SetParLimits(7,0,500.);
      //      fit->SetParLimits(8,50.,999999999.);
    }
    break;
  case 4:
    if( !DiffFWHM ) {
      fit = new TF1("fit4gw",fit4gw,first,last,11);
      fit->SetParName(0,"1-Centroid");
      fit->SetParName(1,"1-FWHM");
      fit->SetParName(2,"1-Counts");
      fit->SetParName(3,"2-Centroid");
      fit->SetParName(4,"2-Counts");
      fit->SetParName(5,"3-Centroid");
      fit->SetParName(6,"3-Counts");
      fit->SetParName(7,"4-Centroid");
      fit->SetParName(8,"4-Counts");
      fit->SetParName(9,"Intercept");
      fit->SetParName(10,"Slope");
      fit->SetParLimits(0,first,last);
      fit->SetParLimits(1,0,500.);
      //      fit->SetParLimits(2,50.,999999999.);
      fit->SetParLimits(3,first,last);
      //      fit->SetParLimits(4,50.,999999999.);
      fit->SetParLimits(5,first,last);
      //      fit->SetParLimits(6,50.,999999999.);
      fit->SetParLimits(7,first,last);
      //      fit->SetParLimits(8,50.,999999999.);
    } else {
      fit = new TF1("fit4g",fit4g,first,last,14);
      fit->SetParName(0,"1-Centroid");
      fit->SetParName(1,"1-FWHM");
      fit->SetParName(2,"1-Counts");
      fit->SetParName(3,"2-Centroid");
      fit->SetParName(4,"2-FWHM");
      fit->SetParName(5,"2-Counts");
      fit->SetParName(6,"3-Centroid");
      fit->SetParName(7,"3-FWHM");
      fit->SetParName(8,"3-Counts");
      fit->SetParName(9,"4-Centroid");
      fit->SetParName(10,"4-FWHM");
      fit->SetParName(11,"4-Counts");
      fit->SetParName(12,"Intercept");
      fit->SetParName(13,"Slope");
      fit->SetParLimits(0,first,last);
      fit->SetParLimits(1,0,500.);
      //      fit->SetParLimits(2,50.,999999999.);
      fit->SetParLimits(3,first,last);
      fit->SetParLimits(4,0,500.);
      //      fit->SetParLimits(5,50.,999999999.);
      fit->SetParLimits(6,first,last);
      fit->SetParLimits(7,0,500.);
      //      fit->SetParLimits(8,50.,999999999.);
      fit->SetParLimits(9,first,last);
      fit->SetParLimits(10,0,500.);
      //      fit->SetParLimits(11,50.,999999999.);
    }
    break;
  default:
    break;
  }
  fit->SetLineColor(kBlue);

  offset = 0;
  fit->SetParameter(0+offset,par[0][0]);
  fit->SetParameter(1+offset,par[1][0]);
  fit->SetParameter(2+offset,par[2][0]);
  offset += 3;  // tricky, adding offset for b/g below
  for (int i=1; i<numpeak; i++) {
    if( !DiffFWHM ) {
      offset = 3+(i-1)*2;
      fit->SetParameter(0+offset,par[0][i]);
//      fit->SetParameter(1+offset,par[1][i]);
      fit->SetParameter(1+offset,par[2][i]);
      offset += 2;  // tricky, adding offset for b/g below
    } else {
      offset = i*3;
      fit->SetParameter(0+offset,par[0][i]);
      fit->SetParameter(1+offset,par[1][i]);
      fit->SetParameter(2+offset,par[2][i]);
      offset += 3;  // tricky, adding offset for b/g below
// Setting the parameter errors is probably redundant if using ROOT (re)fitter
//  if (BuffFlag) {
//    fit->SetParError(0+offset,parerr[0][i]);
//    fit->SetParError(1+offset,parerr[1][i]);
//    fit->SetParError(2+offset,parerr[2][i]);
//  }
    }
  }
  
  // straight line paremeters follow
  if (BuffFlag) {
    for(int i=0; i<nback; i++) {
      fit->SetParameter(i+offset,poly[i]);
      fit->SetParError(i+offset,polyerr[i]);
    }
  } else {
    fit->SetParameter(0+offset,1);
    fit->SetParameter(1+offset,1);
  }

  printf("Starting ROOT fit.\n");
  zoom->Fit(fit,"RB");

//  if( !BuffFlag ) 
//    zoom->Fit(fit,"RB");
//  else {
//    zoom->Draw();
//    fit->Draw("SAME");
//    c1->Update();
    //    TVirtualHistPainter *p = zoom->GetPainter();
    //    p->PaintStat(1,fit);
//  }

  int idx;
  char fitnam[6];
  for (int i=0; i<numpeak; i++) {
    idx = i + 1;
    sprintf(fitnam,"gauss%d",idx);
    fitg[i] = new TF1(fitnam,gauss,first,last,3);
    if( !DiffFWHM ) {
      if( i==0 ) {
        fitg[i]->SetParameter(0,fit->GetParameter(0));
        fitg[i]->SetParameter(1,fit->GetParameter(1));
        fitg[i]->SetParameter(2,fit->GetParameter(2));
        offset = 3;
      } else {
        fitg[i]->SetParameter(0,fit->GetParameter(0+offset));
// tricky, using scaled sigma (1/sqrt(E) dependence)
        fitg[i]->SetParameter(1,fit->GetParameter(1)*sqrt(fit->GetParameter(0+offset)/fit->GetParameter(0)));
        fitg[i]->SetParameter(2,fit->GetParameter(1+offset));
        offset +=2;
      }
    } else {
      offset = i*3;
      fitg[i]->SetParameter(0,fit->GetParameter(0+offset));
      fitg[i]->SetParameter(1,fit->GetParameter(1+offset));
      fitg[i]->SetParameter(2,fit->GetParameter(2+offset));
    }
    fitg[i]->SetLineColor(kGreen);
    fitg[i]->Draw("SAME");
  }
}


Double_t gauss(Double_t *x, Double_t *p) { 
  Double_t C_INV_SQRT_2PI = 0.39894228; 
  Double_t dM = 1; 
  Double_t arg1 = 0; 
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35); 
  Double_t fitval =  
    p[2]*C_INV_SQRT_2PI*dM*exp(-0.5*arg1*arg1)/(p[1]/2.35);
  return fitval; 
} 

Double_t fit1g(Double_t *x, Double_t *p) { 
  Double_t C_INV_SQRT_2PI = 0.39894228; 
  Double_t dM = 1; 
  Double_t arg1 = 0; 
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35); 
  Double_t fitval =  
    p[2]*C_INV_SQRT_2PI*dM*exp(-0.5*arg1*arg1)/(p[1]/2.35) +
    p[3]+p[4]*x[0]; 
  return fitval; 
} 

Double_t fit2g(Double_t *x, Double_t *p) { 
  Double_t C_INV_SQRT_2PI = 0.39894228; 
  Double_t dM = 1; 
  Double_t arg1 = 0, arg2 = 0;
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35); 
  if( p[4] ) arg2 = (x[0]-p[3])/(p[4]/2.35); 
  Double_t fitval =  
    p[2]*C_INV_SQRT_2PI*dM*exp(-0.5*arg1*arg1)/(p[1]/2.35) +
    p[5]*C_INV_SQRT_2PI*dM*exp(-0.5*arg2*arg2)/(p[4]/2.35) +
    p[6]+p[7]*x[0]; 
  return fitval; 
} 

Double_t fit3g(Double_t *x, Double_t *p) { 
  Double_t C_INV_SQRT_2PI = 0.39894228; 
  Double_t dM = 1; 
  Double_t arg1 = 0, arg2 = 0, arg3 = 0;
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35); 
  if( p[4] ) arg2 = (x[0]-p[3])/(p[4]/2.35); 
  if( p[7] ) arg3 = (x[0]-p[6])/(p[7]/2.35); 
  Double_t fitval =  
    p[2]*C_INV_SQRT_2PI*dM*exp(-0.5*arg1*arg1)/(p[1]/2.35) +
    p[5]*C_INV_SQRT_2PI*dM*exp(-0.5*arg2*arg2)/(p[4]/2.35) +
    p[8]*C_INV_SQRT_2PI*dM*exp(-0.5*arg3*arg3)/(p[7]/2.35) +
    p[9]+p[10]*x[0]; 
  return fitval; 
} 

Double_t fit4g(Double_t *x, Double_t *p) { 
  Double_t C_INV_SQRT_2PI = 0.39894228; 
  Double_t dM = 1; 
  Double_t arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0;
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35); 
  if( p[4] ) arg2 = (x[0]-p[3])/(p[4]/2.35); 
  if( p[7] ) arg3 = (x[0]-p[6])/(p[7]/2.35); 
  if( p[10] ) arg4 = (x[0]-p[9])/(p[10]/2.35); 
  Double_t fitval =  
    p[2]*C_INV_SQRT_2PI*dM*exp(-0.5*arg1*arg1)/(p[1]/2.35) +
    p[5]*C_INV_SQRT_2PI*dM*exp(-0.5*arg2*arg2)/(p[4]/2.35) +
    p[8]*C_INV_SQRT_2PI*dM*exp(-0.5*arg3*arg3)/(p[7]/2.35) +
    p[11]*C_INV_SQRT_2PI*dM*exp(-0.5*arg4*arg4)/(p[10]/2.35) +
    p[12]+p[13]*x[0]; 
  return fitval; 
} 

Double_t fit2gw(Double_t *x, Double_t *p) { 
  Double_t C_INV_SQRT_2PI = 0.39894228; 
  Double_t dM = 1; 
  Double_t arg1 = 0, arg2 = 0;
// Only one FWHM, other scaled by sqrt(E) NOT 1./sqrt(E)
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35); 
  if( p[1] ) arg2 = (x[0]-p[3])/(p[1]*sqrt(p[3]/p[0])/2.35); 
  Double_t fitval =  
    p[2]*C_INV_SQRT_2PI*dM*exp(-0.5*arg1*arg1)/(p[1]/2.35) +
    p[4]*C_INV_SQRT_2PI*dM*exp(-0.5*arg2*arg2)/(p[1]*sqrt(p[3]/p[0])/2.35) +
    p[5]+p[6]*x[0]; 
  return fitval; 
} 

Double_t fit3gw(Double_t *x, Double_t *p) { 
  Double_t C_INV_SQRT_2PI = 0.39894228; 
  Double_t dM = 1; 
  Double_t arg1 = 0, arg2 = 0, arg3 = 0;
// Only one FWHM, other scaled by sqrt(E) NOT 1./sqrt(E)
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35); 
  if( p[1] ) arg2 = (x[0]-p[3])/(p[1]*sqrt(p[3]/p[0])/2.35); 
  if( p[1] ) arg3 = (x[0]-p[5])/(p[1]*sqrt(p[5]/p[0])/2.35); 
  Double_t fitval =  
    p[2]*C_INV_SQRT_2PI*dM*exp(-0.5*arg1*arg1)/(p[1]/2.35) +
    p[4]*C_INV_SQRT_2PI*dM*exp(-0.5*arg2*arg2)/(p[1]*sqrt(p[3]/p[0])/2.35) +
    p[6]*C_INV_SQRT_2PI*dM*exp(-0.5*arg3*arg3)/(p[1]*sqrt(p[5]/p[0])/2.35) +
    p[7]+p[8]*x[0]; 
  return fitval; 
} 

Double_t fit4gw(Double_t *x, Double_t *p) { 
  Double_t C_INV_SQRT_2PI = 0.39894228; 
  Double_t dM = 1; 
  Double_t arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0;
// Only one FWHM, other scaled by sqrt(E) NOT 1./sqrt(E)
  if( p[1] ) arg1 = (x[0]-p[0])/(p[1]/2.35); 
  if( p[1] ) arg2 = (x[0]-p[3])/(p[1]*sqrt(p[3]/p[0])/2.35); 
  if( p[1] ) arg3 = (x[0]-p[5])/(p[1]*sqrt(p[5]/p[0])/2.35); 
  if( p[1] ) arg4 = (x[0]-p[7])/(p[1]*sqrt(p[7]/p[0])/2.35); 
  Double_t fitval =  
    p[2]*C_INV_SQRT_2PI*dM*exp(-0.5*arg1*arg1)/(p[1]/2.35) +
    p[4]*C_INV_SQRT_2PI*dM*exp(-0.5*arg2*arg2)/(p[1]*sqrt(p[3]/p[0])/2.35) +
    p[6]*C_INV_SQRT_2PI*dM*exp(-0.5*arg3*arg3)/(p[1]*sqrt(p[5]/p[0])/2.35) +
    p[8]*C_INV_SQRT_2PI*dM*exp(-0.5*arg4*arg4)/(p[1]*sqrt(p[7]/p[0])/2.35) +
    p[9]+p[10]*x[0]; 
  return fitval; 
} 
