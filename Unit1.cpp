//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
      String projectFolder = ExtractFilePath(Application->ExeName);
	  projectFolder=TPath::Combine(projectFolder, "TESTOWE.FDB");
		FDConnection1->DriverName="FB";
		FDConnection1->Params->Values["User_Name"] = "SYSDBA";
		FDConnection1->Params->Values["Password"] = "masterkey";
		FDConnection1->Params->Values["Database"] = projectFolder;
		FDConnection1->ConnectionName="LocalConnection";
		FDConnection1->Connected = true;
		//Nawi¹zywanie po³¹czenia z baz¹ danych w kodzie, nie za pomoc¹ data explorera
        //¿eby program móg³ dzia³aæ na ka¿dym urz¹dzeniu

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
 int TempIDPozycji;
 float TempKwota;
 int TempIDDokumentu;
 String dataKsiegowania;
	Pozycje.Vector_IPozycja.clear();
	Dokumenty.Mapa_IDokument.clear();
  //Zrobione w ten sposób ¿eby po drugim naciœniêciu nie dodawa³o duplikowanych wartoœci

	FDQuery2->Open();
	while (!FDQuery2->Eof)
	{
		TempIDDokumentu = FDQuery2->FieldByName("ID_Dokumentu")->AsInteger;
		dataKsiegowania = FDQuery2->FieldByName("DATA_KSIEGOWANIA")->AsString;
			IDokument* dokument = new CDokument(dataKsiegowania,TempIDDokumentu);
			Dokumenty.Mapa_IDokument[TempIDDokumentu]=dokument;
		FDQuery2->Next();
	}
	FDQuery2->Close();

	Edit4->Text= TempIDDokumentu;

	FDQuery3->Open();
	while (!FDQuery3->Eof)
	{
		TempIDPozycji=  FDQuery3->FieldByName("ID_Pozycji")->AsInteger;
		TempIDDokumentu = FDQuery3->FieldByName("ID_Dokumentu")->AsInteger;
		TempKwota = FDQuery3->FieldByName("Kwota")->AsFloat;
			IPozycja* pozycja = new CPozycja(TempIDPozycji,TempIDDokumentu,TempKwota);
			Pozycje.Vector_IPozycja.push_back(pozycja);
		FDQuery3->Next();
	}
	FDQuery3->Close();
		Button2->Enabled=true;
		Button3->Enabled=true;
		Button4->Enabled=true;
		Button5->Enabled=true;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
  {
	TDateTime wybranaData = DateTimePicker1->Date;
	String Data= DateToStr(wybranaData);
		IDokument* dokument = new CDokument(Data,Form1->Dokumenty.Mapa_IDokument.size()+1);
	//normalnie po new callowany by³by delete w destruktorze, jednak z tego powodu ze
	//destruktor który by³by w tym celu callowany to destruktor samej aplikacji to zwolnieniem
	//tej pamiêci zajmie siê system windows

	Dokumenty.DodajNowy(FDQuery1,dokument);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	Edit2->Text=Dokumenty.Sumuj(FDQuery1);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button4Click(TObject *Sender)
   {
	int wartosc = StrToIntDef(Edit1->Text, 0);
		if(wartosc == 0)
	   {
		   Edit3->Text="WprowadŸ ID";
		   return;
	   }
       if(Dokumenty.Znajdz(wartosc))
	   Edit3->Text=Dokumenty.Znajdz(wartosc)->SumaPozycji(FDQuery1);
}
//---------------------------------------------------------------------------






void __fastcall TForm1::Button5Click(TObject *Sender)
{
	TDateTime wybranaData = DateTimePicker1->Date;
	String Data= DateToStr(wybranaData);

   Edit5->Text=Dokumenty.Sumuj(FDQuery1,Data);
}
//---------------------------------------------------------------------------

