#include <string>
#include <vector>
#include<map>
#include <vcl.h>
#include <tchar.h>
#include "Unit1.h"
class IDokument
	{
		public:
			virtual float SumaPozycji(TFDQuery* Query)=0;
		public:
        	IDokument()=default;
			String Data_Ksiegowania;
			int ID_Dokumentu;
	};
//
class IPozycja
	{
	protected:
		int ID_Pozycji;
		int ID_Dokumentu;
	public:
		float Kwota;
	};
//
class CDokument : public IDokument
	{
	public:
		~CDokument();
		CDokument(String Data,int ID)
			{
				this->Data_Ksiegowania=Data;
				this->ID_Dokumentu=ID;
			};

		virtual float SumaPozycji(TFDQuery* Query) override
			{
				float Suma=0;
					Query->SQL->Clear();
					Query->SQL->Add("SELECT ID_Dokumentu, SUM(Kwota) AS suma_pozycji FROM POZYCJE");
					Query->SQL->Add("WHERE ID_Dokumentu=");
					Query->SQL->Add(ID_Dokumentu);
					Query->SQL->Add("GROUP BY ID_Dokumentu");
					Query->Open();
					if (!Query->IsEmpty())
						Suma = Query->FieldByName("suma_pozycji")->AsFloat;
					Query->Close();
				return Suma;
			};

	private:
		std::vector<IPozycja> Vector_IPozycja;

	};
	//
class CPozycja : public IPozycja
	{
	public:
		~CPozycja();
		CPozycja(int id_p,int id_d,float Kw)
			{
				this->ID_Pozycji=id_p;
				this->ID_Dokumentu=id_d;
				this->Kwota=Kw;
			}
	};
	//
class CDokumenty
	{
	public:
		IDokument* Znajdz(int id)
			{
				if (Mapa_IDokument.find(id) != Mapa_IDokument.end())
					{          IDokument* test= Mapa_IDokument[id];
                    test=test;
						return Mapa_IDokument[id];
					}
				 else
					 {
						return nullptr;
					 }
			}

		float Sumuj(TFDQuery* Query)
			{
				float Suma=0;
				 Query->SQL->Clear();
				 Query->SQL->Add("SELECT SUM(Kwota) AS suma_pozycji FROM POZYCJE");
				 Query->Open();

					 if (!Query->IsEmpty())
						Suma = Query->Fields->FieldByName("suma_pozycji")->AsFloat;

				Query->Close();
				return Suma;
			}
			  ////
		float Sumuj(TFDQuery* Query,String WybranaData)
			  {
				float Suma=0;
				 Query->SQL->Clear();
				 Query->SQL->Add("SELECT SUM(p.Kwota) AS suma_pozycji FROM POZYCJE p INNER JOIN DOKUMENTY d ON p.ID_Dokumentu = d.ID_Dokumentu WHERE d.DATA_KSIEGOWANIA <CAST(:WybranaData AS DATE)");
				 Query->Params->ParamByName("WybranaData")->AsString = WybranaData.c_str();
				 Query->Open();

				if (!Query->IsEmpty())
					Suma = Query->FieldByName("suma_pozycji")->AsFloat;

				Query->Close();
				return Suma;
			  }
		void DodajNowy(TFDQuery* Query,IDokument* NowyDokument)
		{
			Query->SQL->Clear();
			Query->SQL->Text = "INSERT INTO DOKUMENTY (DATA_KSIEGOWANIA) VALUES (:parametr)";
			Query->Params->ParamByName("parametr")->AsString = NowyDokument->Data_Ksiegowania.c_str();
			Query->ExecSQL();
			 Mapa_IDokument[Mapa_IDokument.size()+1]= NowyDokument;
			Query->Close();
		}
	public:
		std::map<int, IDokument*> Mapa_IDokument;

	};
//
	class CPozycje
	{
	public:
		std::vector<IPozycja*> Vector_IPozycja;
	};