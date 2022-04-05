void newlightYield(){
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.15);


  double xmin = -6.75;
  double xmax = 6.75;
  int nbinsx = 40;
  //double ymin = -3.25;
  //double ymax = 3.25;
  double ymin = -6.5;
  double ymax = 6.5;
  int nbinsy = 40;
  //double shift_y = 3.25;

  double pde = 3.e-2; // photon detection efficiency [1]
  double ly_prod = 24.e3; // produced photons per MeV [1/MeV]
  double ngen = 1.e6; // number of generated photons per point [1]
  
  int nvols = 710;
  TH1D* htXe = new TH1D("htXe","",nvols+1,-0.5,nvols+0.5); //Xe histogram for collecting data from files
  TH1D* htAr = new TH1D("htAr","",nvols+1,-0.5,nvols+0.5); //Ar histogram for collecting data from files
  TFile* fXe; //Xe
  TFile* fAr; //Ar
  
  double dx = (xmax-xmin)/double(nbinsx); //0.675/2;
  double dy = (ymax-ymin)/double(nbinsy); //0.65/2;
  double x0, y0, sumXe=0, sumtotXe=0, sumAr=0, sumtotAr=0;
  double err_sumtotXe, err_sumtotAr;
  TString fnameXe, fnameAr;
  TH1D* haXe;//auxiliar histogram for Xe
  TH1D* haAr;//auxiliar histogram for Ar
  TH2F *Xeaux = new TH2F("Xeaux","PE per MeV ",    nbinsx,xmin,xmax,nbinsy,ymin,ymax);//Auxiliar LY map (Xe only)
  TH2F *Phtot = new TH2F("PhtotXeAr","PE per MeV ",nbinsx,xmin,xmax,nbinsy,ymin,ymax); //LY map for Ar+Xe
  TH2F *Araux = new TH2F("Araux","PE per MeV ",    nbinsx,xmin,xmax,nbinsy,ymin,ymax);  //Auxiliar LY map (Ar only)

  int bin_global;
  
  for(int i=0;i<nbinsx/2;i++){ //loop over x position
    for(int j=0;j<nbinsy/2;j++){//loop over y position
      x0 = i*dx;
      //y0 = ymin + (0.5+j)*dy;
      y0 = (0.5+j)*dy;

      fnameXe.Form("standalone_sim_Xe_jobs/new_Xetest/arapuca_%d_%d.root",i,j);
      fnameAr.Form("standalone_sim_Ar_jobs/new_Artest/arapuca_%d_%d.root",i,j);
      fXe = TFile::Open(fnameXe);
      fAr = TFile::Open(fnameAr);
      haXe = (TH1D*)fXe->Get("hv");
      haAr = (TH1D*)fAr->Get("hv");
      htXe->Add(haXe); //summing to get average at center, but can be made per plane (k value)
      htAr->Add(haAr);
      fXe->Close();
      fAr->Close();
      sumXe=htXe->Integral(5,164) + 0.9*(htXe->Integral(165,484));//summing all landing photons over the two laterals (index from 5 to 164) and cathode (from 165 to 484) with 90% light transmission
      sumAr=htAr->Integral(5,164) + 0.9*(htAr->Integral(165,484));

      sumtotXe+=sumXe;
      sumtotAr+=sumAr;

      htXe->Reset();
      htAr->Reset();
      
      err_sumtotXe = (sqrt(sumtotXe)/ngen)*pde*ly_prod; //3% detection efficiency and 25,000 photons per MeV
      sumtotXe=(sumtotXe/ngen)*pde*ly_prod; //3% detection efficiency and 25,000 photons per MeV
      err_sumtotAr = (sqrt(sumtotAr)/ngen)*pde*ly_prod;
      sumtotAr=(sumtotAr/ngen)*pde*ly_prod;

      bin_global = Xeaux->FindBin(x0,y0);
      Xeaux->SetBinContent(bin_global,sumtotXe);
      Xeaux->SetBinError(bin_global,err_sumtotXe);
      Araux->SetBinContent(bin_global,sumtotAr);
      Araux->SetBinError(bin_global,err_sumtotAr);
      sumtotXe=0.0;
      sumtotAr=0.0;
    } //for in j
  }//for in i

  // mirror over x
  int aux  = nbinsx;
  for(int i=1;i<=nbinsx/2;i++){
    for(int j=nbinsy/2+1;j<=nbinsy;j++){
      Xeaux->SetBinContent(i,j,Xeaux->GetBinContent(aux,j));
      Xeaux->SetBinError(i,j,Xeaux->GetBinError(aux,j));
      Araux->SetBinContent(i,j,Araux->GetBinContent(aux,j));
      Araux->SetBinError(i,j,Araux->GetBinError(aux,j));
    }
    aux--;
  }

  // mirror over y
  aux  = nbinsy;
  for(int j=1;j<=nbinsy/2;j++){
    for(int i=1;i<=nbinsx;i++){
      Xeaux->SetBinContent(i,j,Xeaux->GetBinContent(i,aux));
      Xeaux->SetBinError(i,j,Xeaux->GetBinError(i,aux));
      Araux->SetBinContent(i,j,Araux->GetBinContent(i,aux));
      Araux->SetBinError(i,j,Araux->GetBinError(i,aux));
    }
    aux--;
  }

  //TFile* f = TFile::Open("lyR0.root");
  //PhtotAr->Add((TH2F*)f->Get("Phztot"));

  // loop to fill 2D histogram
  double avg; //auxiliar variable for getting the weighted histo
  double err;
  for(int i=1;i<=nbinsx;i++){
    for(int j=1;j<=nbinsy;j++){
      avg=(Araux->GetBinContent(i,j))*0.47; // Ar light assumed to be 25%
      err = TMath::Power(Araux->GetBinError(i,j)*0.47,2); // for error, sum in quadrature Ar and Xe contributions. 
      avg+=(Xeaux->GetBinContent(i,j))*0.53;
      err += TMath::Power(Xeaux->GetBinError(i,j)*0.53,2);
      err = sqrt(err);  
      Phtot->SetBinContent(i,j,avg);
      Phtot->SetBinError(i,j,err);
    }
  }

  // loop to compute LY_avg and LY_min
  double ly_avg = 0.;
  double err_ly_avg = 0.;
  double ly_min = 1.e6;
  double err_ly_min = 0.;
  int counter = 0;
  for(int i=nbinsx/2+1;i<=nbinsx;i++){
    for(int j=nbinsy/2+1;j<=nbinsy;j++){
      avg = Phtot->GetBinContent(i,j);
      err = Phtot->GetBinError(i,j);
      ly_avg += avg;
      err_ly_avg += TMath::Power(err,2);
      if (avg < ly_min) {
        ly_min = avg;
        err_ly_min = err;
        //std::cout << avg << ", " << err << std::endl;
      }
      counter++;
    }
  }

  ly_avg = ly_avg / (counter);
  err_ly_avg = sqrt(err_ly_avg) / (counter);
  
  std::cout << "LY_avg (PEs/MeV):" << Form("%.3f",ly_avg) << " +/- " << Form("%.3f",err_ly_avg) << std::endl;
  std::cout << "LY_min (PEs/MeV):" << Form("%.3f",ly_min) << " +/- " << Form("%.3f",err_ly_min) << std::endl;
    
  gStyle->SetPalette(1);
  TFile* total = new TFile("LYmapXe53Ar47_ref_4_20.root", "RECREATE");
  Phtot->GetXaxis()->SetTitle("x (m)");
  Phtot->GetYaxis()->SetTitle("y (m)");
  Phtot->GetZaxis()->SetTitle("LY (PEs/MeV)");
  
  Phtot->GetZaxis()->SetRangeUser(0,125);
  Phtot->Draw("COLZ");
  Phtot->Write();

  total->Close();
}
