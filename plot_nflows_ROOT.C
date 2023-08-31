//IMPORTANT: The first time you execute this program, run in advance inside the folder containing the .csv files the command:
// for f in *.csv; do sed -i '1d' "$f"; done
//only once, in order to delete the first (header) line of each .csv file.

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph.h"

using namespace std;

void plot_nflows_ROOT()
{
   TString dir = gSystem->UnixPathName(__FILE__);
   dir.ReplaceAll("plot_nflows_ROOT.C", "");
   dir.ReplaceAll("/./", "/");
   
   gROOT->Reset();
   
   Int_t N = 128;
   
   TFile *f[6];
   TTree *t[6];
   TBranch *b_x0[6];
   TBranch *b_x1[6];
   
   Float_t X0_val[6];
   Float_t X1_val[6];
   Float_t X0[6][N];
   Float_t X1[6][N];
   
   TString filename[6];
   
   filename[0] = "dataPyTorch_after_training_zgrid0.csv";
   filename[1] = "dataPyTorch_after_training_zgrid1.csv";
   filename[2] = "dataPyTorch_after_loading_zgrid0.csv";
   filename[3] = "dataPyTorch_after_loading_zgrid1.csv";
   filename[4] = "dataCpp_after_Cpp_reading_zgrid0.csv";
   filename[5] = "dataCpp_after_Cpp_reading_zgrid1.csv";
      
   
   for (Int_t cou = 0; cou < 6; cou++) {
    TString rootfile = Form("RF%d.root", cou + 1);
    
    f[cou] = new TFile(rootfile, "RECREATE");
    t[cou] = new TTree("ntuple", "data from csv file");
    t[cou]->ReadFile(filename[cou], "x0/F:x1/F", ',');
    t[cou]->Write();
    
    b_x0[cou] = t[cou]->GetBranch("x0");
    b_x1[cou] = t[cou]->GetBranch("x1");
    
    b_x0[cou]->SetAddress(&X0_val[cou]);
    b_x1[cou]->SetAddress(&X1_val[cou]);
    
    for (Int_t k = 0; k < N; k++) {
        b_x0[cou]->GetEntry(k);
        X0[cou][k] = X0_val[cou];
        b_x1[cou]->GetEntry(k);
        X1[cou][k] = X1_val[cou];
    }
   }

   //x0
   TCanvas *c0 = new TCanvas("c0", "c0", 200, 400, 1200, 600);
   c0->SetFillColor(0);
   c0->SetGrid();
   
   //create a 2D histogram to define the range
   TH2F *h0 = new TH2F("h0", "Conditional moons", N, 0.0, 10.0, N, 0.0, 3.5);
   h0->SetXTitle("#font[42]{x0}");
   h0->SetYTitle("#font[42]{x1}");
   h0->Draw();
   c0->GetFrame()->SetFillColor(21);
   c0->GetFrame()->SetBorderSize(12);
   
   Float_t a1[N], b1[N];
   for(int i=0; i<N; i++) {
	   a1[i] = X0[0][i]; 
	   b1[i] = X1[0][i];
   }

   TGraph *gr1 = new TGraph(N, a1, b1);
   gr1->SetLineColor(kRed);
   gr1->SetMarkerColor(kRed);
   gr1->SetMarkerStyle(21);
   gr1->Draw("P");

   Float_t a2[N], b2[N];
   for(int i=0; i<N; i++) {
	   a2[i] = X0[1][i]; 
	   b2[i] = X1[1][i];
   }

   TGraph *gr2 = new TGraph(N, a2, b2);
   gr2->SetLineColor(kMagenta);
   gr2->SetMarkerColor(kMagenta);
   gr2->SetMarkerStyle(21);
   gr2->Draw("P");

   Float_t a3[N], b3[N];
   for(int i=0; i<N; i++) {
           a3[i] = X0[2][i]; 
           b3[i] = X1[2][i];
   }

   TGraph *gr3 = new TGraph(N, a3, b3);
   gr3->SetLineColor(kBlue);
   gr3->SetMarkerColor(kBlue);
   gr3->SetMarkerStyle(21);
   gr3->Draw("P");
   
   Float_t a4[N], b4[N];
   for(int i=0; i<N; i++) {
           a4[i] = X0[3][i]; 
           b4[i] = X1[3][i];
   }

   TGraph *gr4 = new TGraph(N, a4, b4);
   gr4->SetLineColor(kCyan);
   gr4->SetMarkerColor(kCyan);
   gr4->SetMarkerStyle(21);
   gr4->Draw("P");
   
   Float_t a5[N], b5[N];
   for(int i=0; i<N; i++) {
           a5[i] = X0[4][i];
           b5[i] = X1[4][i];
   }

   TGraph *gr5 = new TGraph(N, a5, b5);
   gr5->SetLineColor(kGreen);
   gr5->SetMarkerColor(kGreen);
   gr5->SetMarkerStyle(21);
   gr5->Draw("P");
   
   Float_t a6[N], b6[N];
   for(int i=0; i<N; i++) {
           a6[i] = X0[5][i];
           b6[i] = X1[5][i];
   }

   TGraph *gr6 = new TGraph(N, a6, b6);
   gr6->SetLineColor(kYellow);
   gr6->SetMarkerColor(kYellow);
   gr6->SetMarkerStyle(21);
   gr6->Draw("P");
     
   gStyle->SetOptStat(0);
   
   TLegend *leg0 = new TLegend(0.55, 0.6, 0.8, 0.85);
   leg0->SetTextFont(42);
   leg0->SetTextAngle(0);
   leg0->SetTextColor(kBlack);
   leg0->SetTextSize(0.035);
   leg0->SetTextAlign(12); 
   leg0->AddEntry(gr1, "Halfmoon 1: after training", "l");
   leg0->AddEntry(gr2, "Halfmoon 2: after training", "l");
   leg0->AddEntry(gr3, "Halfmoon 1: after loading", "l");  
   leg0->AddEntry(gr4, "Halfmoon 2: after loading", "l");  
   leg0->AddEntry(gr5, "Halfmoon 1: after reading in C++", "l");  
   leg0->AddEntry(gr6, "Halfmoon 2: after reading in C++", "l");  
   leg0->SetBorderSize(0);
   leg0->Draw();
   
   c0->Update();
   c0->Write();
   c0->Print(Form("Moons.pdf"),"pdf");
}


/* using namespace std;

void plotDataPoints(const char* filename, const char* treeName)
{
    TFile* file = new TFile(filename);
    TTree* tree = (TTree*)file->Get(treeName);

    // Variables to store the data
    Double_t x0, x1;

    // Set up the branches to read the data
    tree->SetBranchAddress("x0", &x0);
    tree->SetBranchAddress("x1", &x1);

    // Create a TGraph to hold the points
    TGraph* graph = new TGraph();

    // Loop over the entries in the tree
    Long64_t numEntries = tree->GetEntries();
    for (Long64_t i = 0; i < numEntries; ++i)
    {
        tree->GetEntry(i);
        graph->SetPoint(i, x0, x1);
    }

    // Create a canvas and draw the graph
    TCanvas* canvas = new TCanvas("canvas", "Data Points", 800, 600);
    graph->SetTitle("Data Points");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.8);
    graph->Draw("AP");

    // Clean up
    delete graph;
    delete canvas;
    file->Close();
}

void plotAllDataPoints()
{
    // Call the function to plot each file
    plotDataPoints("dataPyTorch_after_training_zgrid0.csv", "treeName");
    plotDataPoints("dataPyTorch_after_loading_zgrid0.csv", "treeName");
    plotDataPoints("dataCpp_after_Cpp_reading_zgrid0.csv", "treeName");
}

void plot_nflows_ROOT()
{
    // Call the function to plot all the data points
    plotAllDataPoints();
}*/
