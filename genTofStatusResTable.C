TH2D* H2D_BTofResVsTray;
TH2D* H2D_BTofResVsModule;
TH2D* H2D_BTofResVsCell;

TProfile* prfx_BTofResVsTray;
TProfile* prfx_BTofResVsModule;
TProfile* prfx_BTofResVsCell;
//-----------------------------------------------------------------------------------------
void readFiles();
void drawAndGetTables();
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void genTofStatusResTable()
{
	readFiles();
	drawAndGetTables();
}
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void readFiles()
{
	TFile *infile = new TFile("./inputfiles/allhists_run18isobar_forT0_pass7.root", "read");
	H2D_BTofResVsTray     = (TH2D*) infile->Get("BTofTimeRes_vs_Tray");
	H2D_BTofResVsModule   = (TH2D*) infile->Get("timeModule");
	H2D_BTofResVsCell     = (TH2D*) infile->Get("BTofTimeRes_vs_Cell");
	prfx_BTofResVsTray    = (TProfile*) H2D_BTofResVsTray  ->ProfileX("prfx_BTofResVsTray"  );
	prfx_BTofResVsModule  = (TProfile*) H2D_BTofResVsModule->ProfileX("prfx_BTofResVsModule");
	prfx_BTofResVsCell    = (TProfile*) H2D_BTofResVsCell  ->ProfileX("prfx_BTofResVsCell"  );
}

void drawAndGetTables()
{
	TCanvas *c1 = new TCanvas("c1");
	gStyle->SetOptStat(0);
	gPad->SetTopMargin(0.08);
	gPad->SetBottomMargin(0.11);
	gPad->SetLeftMargin(0.11);
	gPad->SetRightMargin(0.05);
	//---------------------------------------------------------------------------
	//---------------------------------------------------------------------------

	TLatex tl;
	tl.SetTextSize(0.06);
	tl.SetNDC();
	
	c1->SetLogz();

	H2D_BTofResVsTray->SetXTitle("TrayId");
	H2D_BTofResVsTray->SetYTitle("T_{measure}-T_{expected}");
	H2D_BTofResVsTray->GetYaxis()->SetTitleSize(0.06);
	H2D_BTofResVsTray->GetYaxis()->SetTitleOffset(0.75);
	H2D_BTofResVsTray->GetXaxis()->SetTitleSize(0.05);
	H2D_BTofResVsTray->GetXaxis()->SetTitleOffset(0.85);
	H2D_BTofResVsTray->SetAxisRange(-2,2,"y");
	H2D_BTofResVsTray->Draw("colz");
	
	c1->SaveAs("./plots/H2D_BTofResVsTray.png");

	prfx_BTofResVsTray->SetXTitle("TrayId");
	prfx_BTofResVsTray->SetYTitle("<T_{measure}-T_{expected}>");
	prfx_BTofResVsTray->GetYaxis()->SetTitleSize(0.06);
	prfx_BTofResVsTray->GetYaxis()->SetTitleOffset(0.75);
	prfx_BTofResVsTray->GetXaxis()->SetTitleSize(0.05);
	prfx_BTofResVsTray->GetXaxis()->SetTitleOffset(0.85);
	prfx_BTofResVsTray->SetAxisRange(-0.2,0.2,"y");
	prfx_BTofResVsTray->Draw("pe");
	
	c1->SaveAs("./plots/prfx_BTofResVsTray.png");
	
	//------------------------
	vector<int> trayVect;
	//------------------------

	std::ofstream outdata;
	outdata.open("./badTrayModuleCell_Run18.dat", std::ofstream::out);

	for(int itray=1; itray<=120; itray++)
	{
		if(prfx_BTofResVsTray->GetBinContent(itray)==0)
		{
			trayVect.push_back(itray);
			
			//cout<<"itray: "<<itray<<endl;
			cout<<itray<<"\t"<<0<<"\t"<<0<<endl;
			outdata<<itray<<"\t"<<0<<"\t"<<0<<endl;
		}
	}
	
	cout<<"trayVect.size(): "<<trayVect.size()<<endl;

	H2D_BTofResVsModule->SetXTitle("ModuleId");
	H2D_BTofResVsModule->SetYTitle("T_{measure}-T_{expected}");
	H2D_BTofResVsModule->GetYaxis()->SetTitleSize(0.06);
	H2D_BTofResVsModule->GetYaxis()->SetTitleOffset(0.75);
	H2D_BTofResVsModule->GetXaxis()->SetTitleSize(0.05);
	H2D_BTofResVsModule->GetXaxis()->SetTitleOffset(0.85);
	H2D_BTofResVsModule->SetAxisRange(-2,2,"y");
	H2D_BTofResVsModule->Draw("colz");
	
	c1->SaveAs("./plots/H2D_BTofResVsModule.png");

	prfx_BTofResVsModule->SetXTitle("ModuleId");
	prfx_BTofResVsModule->SetYTitle("<T_{measure}-T_{expected}>");
	prfx_BTofResVsModule->GetYaxis()->SetTitleSize(0.06);
	prfx_BTofResVsModule->GetYaxis()->SetTitleOffset(0.75);
	prfx_BTofResVsModule->GetXaxis()->SetTitleSize(0.05);
	prfx_BTofResVsModule->GetXaxis()->SetTitleOffset(0.85);
	prfx_BTofResVsModule->SetAxisRange(-0.2,0.2,"y");
	prfx_BTofResVsModule->Draw("pe");
	
	c1->SaveAs("./plots/prfx_BTofResVsModule.png");
	
	//------------------------
	vector<int> iModuleVect;
	//------------------------

	for(int imodule=1; imodule<=3840; imodule++)
	{
		if(prfx_BTofResVsModule->GetBinContent(imodule)==0)
		{
			//cout<<"------------------------------"<<endl;
			//cout<<"imodule: "<<imodule<<endl;
			int TrayfromImodule   = (int)(imodule/32.) + 1;
			int ModulefromImodule = (int)(imodule%32)  + 1;
			//cout<<"TrayfromImodule: "<<TrayfromImodule<<endl;
			//cout<<"ModulefromImodule: "<<ModulefromImodule<<endl;
			int im = (TrayfromImodule-1)*32  + (ModulefromImodule-1);
			if(im != imodule) cout<<"please check the formula of imodule, it is wrong !!!!"<<endl;
			//cout<<"im: "<<im<<endl;
			//cout<<"------------------------------"<<endl;
			
			if ( std::find(trayVect.begin(), trayVect.end(), TrayfromImodule ) == trayVect.end() )
			{
				//store the imodule for later cell loop to remove the existed Tray and Module 
				iModuleVect.push_back(im);
				//cout<<"TrayfromImodule: "<<TrayfromImodule<<endl;
				//cout<<"ModulefromImodule: "<<ModulefromImodule<<endl;
				cout<<TrayfromImodule<<"\t"<<ModulefromImodule<<"\t"<<0<<endl;
				outdata<<TrayfromImodule<<"\t"<<ModulefromImodule<<"\t"<<0<<endl;
			}
		}
	}//imodule

	cout<<"iModuleVect.size(): "<<iModuleVect.size()<<endl;

	H2D_BTofResVsCell->SetXTitle("CellId");
	H2D_BTofResVsCell->SetYTitle("T_{measure}-T_{expected}");
	H2D_BTofResVsCell->GetYaxis()->SetTitleSize(0.06);
	H2D_BTofResVsCell->GetYaxis()->SetTitleOffset(0.75);
	H2D_BTofResVsCell->GetXaxis()->SetTitleSize(0.05);
	H2D_BTofResVsCell->GetXaxis()->SetTitleOffset(0.85);
	H2D_BTofResVsCell->SetAxisRange(-2,2,"y");
	H2D_BTofResVsCell->Draw("colz");

	c1->SaveAs("./plots/H2D_BTofResVsCell.png");

	prfx_BTofResVsCell->SetXTitle("CellId");
	prfx_BTofResVsCell->SetYTitle("<T_{measure}-T_{expected}>");
	prfx_BTofResVsCell->GetYaxis()->SetTitleSize(0.06);
	prfx_BTofResVsCell->GetYaxis()->SetTitleOffset(0.75);
	prfx_BTofResVsCell->GetXaxis()->SetTitleSize(0.05);
	prfx_BTofResVsCell->GetXaxis()->SetTitleOffset(0.85);
	prfx_BTofResVsCell->SetAxisRange(-0.2,0.2,"y");
	prfx_BTofResVsCell->Draw("pe");

	c1->SaveAs("./plots/prfx_BTofResVsCell.png");

	
	//loop for single bad cells 
	for(int icell=1; icell<=23040; icell++)
	{
		if(prfx_BTofResVsCell->GetBinContent(icell)==0)
		{
			//cout<<"------------------------------"<<endl;
			//cout<<"icell: "<<icell<<endl;
			
			int TrayfromIcell      = (int)(icell/192.)     + 1;
			int ModulefromIcell    = (int)((icell%192)/6.) + 1;
			int CellfromIcell      = (int)((icell%192)%6)  + 1;
			
			int iModuleTem  = (TrayfromIcell-1)*32  + (ModulefromIcell-1);
			int iCellTem    = (TrayfromIcell-1)*192 + (ModulefromIcell-1)*6 + (CellfromIcell-1);
			
			if(iCellTem != icell)
			{
				cout<<"please check the icell formula, it is wrong !!!!"<<endl;
				cout<<"CellfromIcell: "<<CellfromIcell<<endl;
				cout<<"icell: "<<icell<<endl;
				return;
			}

			if ( std::find(trayVect.begin(), trayVect.end(), TrayfromIcell ) == trayVect.end() )
			{
				if(std::find(iModuleVect.begin(), iModuleVect.end(), iModuleTem ) == iModuleVect.end() )
				{
					cout<<TrayfromIcell<<"\t"<<ModulefromIcell<<"\t"<<CellfromIcell<<"\t"<<endl;
					outdata<<TrayfromIcell<<"\t"<<ModulefromIcell<<"\t"<<CellfromIcell<<"\t"<<endl;
				}
			}
		}
	}//icell
	
	outdata.close();

}


//-----------------------------------------------------------------------
//Int_t toftray      = (int)tofcellid/192 + 1;
//Int_t tofmodule    = (int)((tofcellid%192)/6.)+1;

//int tray    = mBTofHitIndexed->tray();
//int module  = mBTofHitIndexed->module();
//
//int iModule = (tray-1)*32  + (module-1);
//int cell    = (tray-1)*192 + (module-1)*6 + (mBTofHitIndexed->cell()-1);


