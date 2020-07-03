#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#define MSG 180
const char heart[] = "\xe2\x99\xa5";
const char arrow[] = "\xe2\x86\x94";


 struct post{
	 std::string message; //must not exceed 180 characters
	 bool like;
	 int num_likes;
	 bool reChirp;
	 post *previous;
	 post *next;
 };
 typedef post* postPtr;
class Chirp; //define chirp, as chirp contains an instance of itself

class Chirp{
private:
	 std::string user;				// name of the user
	 std::string display_name;	// display name of user (i love this functionality)
	 postPtr head; 				//head
	 postPtr previous_chirp();	
	 postPtr next_chirp();
	 void scroll_my_posts();
	 bool isEmpty() const;
	 void display_chirp(postPtr, int);			
	 bool add_like(postPtr it);
	 bool add_chirp_post(std::string);
	 bool delete_chirp(int);
	 char* scroll_thru_posts(); // don't know what to return
	 void scroll_thru_friend_posts();
	 void add_friends();					// I called it in setup_user()
	 void reChirp(postPtr );
	 void change_dn();

public:
	 Chirp(); //constructor
	 Chirp(const Chirp &);//copy constructor
	 Chirp *friends; //one friend
	 ~Chirp(); //destructor]
	 void setup_account();
	 void menu();
	 void setUser(const std::string );
	 std::string getUser() const;
	 void setDN(const std::string );
	 std::string getDN() const;
};