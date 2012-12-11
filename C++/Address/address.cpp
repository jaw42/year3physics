#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>

#include "Base64Encoder.h"

using namespace std;

// Function for calculating e^(-x)sin(x), used throughout
double y(double x){
	return exp(-x) * sin(x);
}

class Contact {
	private:
		string name;
		string name_form;	//formatted name
		string name_nick;	//nickname
		string photo;	//url or base64 encoded picture
		string birthday;	//date of birth 
		string organisation;
		string title;
		string telephone;
		string telephone_home;
		string telephone_work;
		string telephone_mob;
		string adr;
		string adr_home;
		string adr_work;
		string email;
		string email_home;
		string email_work;
	public:
		string get(string get);

		void add_name(string name_add);
		void add_name_form(string name_form_add);
		void add_name_nick(string name_nick_add);
		void add_photo(string photo_add);
		void add_birthday(string birthday_add);	
		void add_organisation(string organisation_add);
		void add_title(string title_add);
		void add_telephone(string telephone_add);
		void add_telephone_home(string telephone_home_add);
		void add_telephone_work(string telephone_work_add);
		void add_telephone_mob(string telephone_mob_add);
		void add_adr(string adr_add);
		void add_adr_home(string adr_home_add);
		void add_adr_work(string adr_work_add);
		void add_email(string email_add);
		void add_email_home(string email_home_add);
		void add_email_work(string email_work_add);
};

string Contact::get(string get){
	if(get == "name"){return name;}
	else if(get == "name_form"){return name_form;}
	else if(get == "name_nick"){return name_nick;}
	else if(get == "photo"){return photo;}
	else if(get == "birthday"){return birthday;}
	else if(get == "organisation"){return organisation;}
	else if(get == "title"){return title;}
	else if(get == "telephone"){return telephone;}
	else if(get == "telephone_home"){return telephone_home;}
	else if(get == "telephone_work"){return telephone_work;}
	else if(get == "telephone_mob"){return telephone_mob;}
	else if(get == "adr"){return adr;}
	else if(get == "adr_home"){return adr_home;}
	else if(get == "adr_work"){return adr_work;}
	else if(get == "email"){return email;}
	else if(get == "email_home"){return email_home;}
	else if(get == "email_work"){return email_work;}
	else{return 0;}
}

void Contact::add_name(string name_add){name = name_add;}
void Contact::add_name_form(string name_form_add){name_form = name_form_add;}
void Contact::add_name_nick(string name_nick_add){name_nick = name_nick_add;}
void Contact::add_photo(string photo_add){photo = photo_add;}
void Contact::add_birthday(string birthday_add){birthday = birthday_add;}	
void Contact::add_organisation(string organisation_add){organisation = organisation_add;}
void Contact::add_title(string title_add){title = title_add;}
void Contact::add_telephone(string telephone_add){telephone = telephone_add;}
void Contact::add_telephone_home(string telephone_home_add){telephone_home = telephone_home_add;}
void Contact::add_telephone_work(string telephone_work_add){telephone_work = telephone_work_add;}
void Contact::add_telephone_mob(string telephone_mob_add){telephone_mob = telephone_mob_add;}
void Contact::add_adr(string adr_add){adr = adr_add;}
void Contact::add_adr_home(string adr_home_add){adr_home = adr_home_add;}
void Contact::add_adr_work(string adr_work_add){adr_work = adr_work_add;}
void Contact::add_email(string email_add){email = email_add;}
void Contact::add_email_home(string email_home_add){email_home = email_home_add;}
void Contact::add_email_work(string email_work_add){email_work = email_work_add;}

int main(){
	string mainmenu;
	while(true){
		cout << "Welcome to Address.\nPlease select an option: \n" 
				<< "\t(1) Add contact,\n"
				<< "\t(2) View Contact,\n"
				<< "\t(3) Search\n"
				<< "\t(0) Exit\n";
		
		getline(cin,mainmenu);

		if(mainmenu == "1"){
			Contact contact;
			string temp;
			cout << "You chose to Add a contact\n";
			
			cout << "Name: ";
			getline(cin, temp);
			contact.add_name(temp);
		//	contact.add(name).data(temp); // ideal syntax, need to investigate

			cout << "Formatted Name: ";
			getline(cin, temp);
			contact.add_name_form(temp);
			
			cout << "\nThe contact is completed. Summary is shown below.\n"
					<< "Would you like to change any of the entries? Press the number to edit, or 0 to save.\n";
			
			cout <<    "\t(1) Name:           " << contact.get("name") << endl
					<< "\t(2) Formatted Name: " << contact.get("name_form") << endl
					<< "\t(0) SAVE" << endl << endl;

			string editentry;
			getline(cin, editentry);
			cout << "Thanks " << editentry << endl << endl;

		} else if(mainmenu == "2"){
			cout << "You chose to View a Contact\n";

			  const string s = "ADP GmbH\nAnalyse Design & Programmierung\nGesellschaft mit beschränkter Haftung" ;

			  string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
			  string decoded = base64_decode(encoded);

			  cout << "encoded: " << encoded << endl;
			  cout << "decoded: " << decoded << endl;

		} else if(mainmenu == "3"){
			cout << "You chose to Search\n";
		} else if(mainmenu == "0"){
			return 0;
		} else{
			cout << "Option not recognised, please choose again.\n";
		}
	}
}



