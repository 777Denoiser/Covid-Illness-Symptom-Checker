/*
Name:Muhammad Seyal
Final Project
Date:11/19/2020
Class: CSE 130-50
Program description: this program prints a table that shows a list of 7 symptoms 
for patients printing if that patient has a certain illness or not. this is a 
symptom checker for covid, cold, flu or other illnesses (note: not entirely accurate 
as there is no data given for patients thus the patients symptoms are generated randomly. ).
*/

#include<iostream>
#include<iomanip>
#include<string>
#include <cstdlib>

using namespace std;

//Base Class
class Symptoms{
protected:
	
		string symptoms[7][4] = { { "Fever","*", "+", "*" },
		{ "Cough","""*", "+", "*" },
		{ "Shortness of Breath","*", "-", "-" },
		{ "Runny Nose","+", "*", "+" },
		{ "Headache","+", "+", "*" },
		{ "Sneezing","-", "*", "-" },
		{ "Fatigue","+", "+", "*" } };


};

//Derived Class
class Report : protected Symptoms{
public:
	
	//method to print table
	void PrintTable()
	{
		cout << "Sample Run:\nWelcome to Symptoms Checker\n\n";
		cout << setw(19)<<"Guide: * Common + Sometimes/Rarely - NO\n";
		
		cout << "==============================================================================\n";
		cout << setw(19) << "Symptoms |" << setw(19) << "Covid-19 |" << setw(19) << "Cold |" << setw(19) << "Flu |" << endl;
		cout << "==============================================================================\n";
		for (int i = 0; i < 7; i++){
			for (int j = 0; j < 4; j++){
				cout << setw(19) << symptoms[i][j];

			}
			cout<<endl;
		}
		cout << "==============================================================================\n\n";

		
	}
	
	//method to generate random symptoms
	void GenerateReport()
	{
		int nop;
		cout<<"Enter the Number of Patients: ";
		cin>>nop;
		cout<<"Thank You...\n";
		cout << "==========================================\n\n";
		cout<<"Symptoms Checker...\n\n";
			
		string testResult[nop][7]; // array to store patient's disease
		int randomNumber;
		
	
	//if number of patients are greater than 0	
	if(nop>0)
	{
	 
	 //generate random prsent or absent for each symptom
	  for(int k=0;k<nop;k++)
	  {	
	  for(int l=0;l<7;l++){
	  
	  	//generate random symptom from given symptoms table ( 7 diseases)
	      int range = 1 - 0 + 1;
		  randomNumber = rand() % range + 0;
		  if(randomNumber==0)
		  testResult[k][l]=symptoms[l][0]+" Absent";
		  else
		  testResult[k][l]=symptoms[l][0]+" Present";
		  
	  }
	  
	}
	
	//check results
	int CovidCount=0, ColdCount=0, FluCount=0, other=0;
	 for(int k=0;k<nop;k++)
	  {	
	  
	  if(testResult[k][0]=="Fever Present" && testResult[k][1]=="Cough Present" && testResult[k][2]=="Shortness of Breath Present")
	  CovidCount++;
	  else if(testResult[k][3]=="Runny Nose Present"&&testResult[k][5]=="Sneezing Present")
	  ColdCount++;
	  else if(testResult[k][0]=="Fever Present"&&testResult[k][1]=="Cough Present"&&testResult[k][4]=="Headache Present" && testResult[k][6]=="Fatigue Present")
	  FluCount++;
	  else
	  other++;
	}
	
	//print results
	
	cout<<CovidCount<<" patients have symptoms of COVID-19\n";
	cout<<ColdCount<<" patients have symptoms of COLD\n";
	cout<<FluCount<<" patients have symptoms of Flu\n";
	cout<<other<<" patients may have some other illness\n";
	cout << "==========================================\n\n";
	
	float covidPer=(CovidCount/(float)nop)*100;
	float coldPer=(ColdCount/(float)nop)*100;
	float fluPer=(FluCount/(float)nop)*100;
	float otherPer=(other/(float)nop)*100;
	
	cout<<"Percentage of each illness:\n\n";
	
	cout<<left<<setw(20)<<"Covid-19:"<<"["<<(int)covidPer<<"%]";
	for(int i=0;i<(int)covidPer;i++)
		cout<<"+";
		cout<<endl;
		
		cout<<left<<setw(20)<<"Cold:"<<"["<<(int)coldPer<<"%]";
	for(int i=0;i<(int)coldPer;i++)
		cout<<"+";
		cout<<endl;
		
		cout<<left<<setw(20)<<"Flu:"<<"["<<(int)fluPer<<"%]";
	for(int i=0;i<(int)fluPer;i++)
		cout<<"+";
		cout<<endl;
		
		
		cout<<left<<setw(20)<<"Other illness:"<<"["<<(int)otherPer<<"%]";
	for(int i=0;i<(int)otherPer;i++)
		cout<<"+";
		cout<<endl;
	
	}	


}
};

//Main Function
int main(){
	Report r1;
	
	r1.PrintTable();
	r1.GenerateReport();

	
}
