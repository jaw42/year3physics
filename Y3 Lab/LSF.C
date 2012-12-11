void LSF() {
  float x[100], y[100], ex[100], ey[100];
  char filename[80];
  printf("Enter filename.\n");
  scanf("%s",filename);

  FILE *f = fopen(filename,"r");
  if (f == NULL) {
    sprintf(line,"File %s not found.\n",filename);
    printf(line);
    return;
  }

  // Read data
  int i = 0;
  while (fscanf(f,"%f %f %f",&(x[i]),&(y[i]),&(ey[i])) == 3 ) {
    i++;
  }

  printf("Read %d data points.\n",i);

  TGraphErrors *tge = new TGraphErrors(i,x,y,ex,ey); 
  tge->SetMarkerStyle(20);
  tge->SetMarkerColor(kBlue);
  tge->SetLineColor(kBlue);

  c1 = new TCanvas("c1","c1",600,600);

  tge->Draw("AP");
  tge->Fit("pol1");
}
