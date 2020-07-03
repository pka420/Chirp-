#include "chirper.h"

int inputCheck(int min, int max)				//function to stop program from running into infinite loop if char input is given instead of int 
{
	int temp;
	bool flag=false;
	while(flag==false)							//essentially means while the inputted datatype is not int, keep going
	{
		std::cin>>temp;
		if(temp<min || temp>max) {
			std::cout << "Incorrect input. Please try again."<<std::endl;
			std::cin.clear();						//clears error flag to allow other operations to work corectly
			//cin.ignore('\n');					//ignores any input in buffer
		}
		else
			flag = true;
	}
	return temp;
}

std::string check_string() {
	//POSTCONDITION : this will help setup_account functon of our class.
	//if user inputs more than 180 characters in the display name or user name this function asks user if they wanna ignore 
	//extra characters or they, want to input again.
	std::string input="";
	std::cin.clear();					//clears input buffer
	std::getline(std::cin, input);
	while(input.length() > MSG)	{
		int temp;																//temporary variable
		std::cout<<"Size of username cannot be greator than "<<MSG<<" characters.	\n";
		std::cout<<"1. Trim. (it will ignore extra characters) "<<std::endl;
		std::cout<<"2. Try again. "<<std::endl;											// can add a smiley sad face if get time	
		temp = inputCheck(1,2);
		if(temp==1)
			input.resize(MSG);
		else
			std::cin>>input;
	}
	return input;
}

void Chirp::setup_account() {
	//	POSTCONDITION:
	//set up user account by taking user name and user display name.
	std::string str;	
	std::cout<<"Lets setup your Chirper account.\n";										 					//functionality for only 180 characters is left
	std::cout<<"Enter username: ";
	str = check_string();
	setUser(str);
	std::cout<<"Enter display name: ";
	str = check_string();
	setDN(str);
	std::cout<<"		Adding your friends......."<<std::endl;
	// at this post print the user has been setup successfully now we need to  set up their friend from file:
	add_friends();			// for adding friend (only one in this version)
	std::cout<<">>>>OK ALL SET  Now, you can start Chirping.\n";
}

Chirp::Chirp() {		//default constructor
	head = nullptr;
}
Chirp::Chirp(const Chirp &oreo) {
	//post codition:
	//deep copies our Chirp data from oreo to calling object.
	setUser(oreo.getUser());
	setDN(oreo.getDN());
	postPtr temp = oreo.head;
	while(temp!=nullptr){									//copying head
		add_chirp_post(temp->message);
		head->like = temp->like;
		head->num_likes = temp->num_likes;
		head->reChirp = temp->reChirp;
		temp = temp->next;
	}

	temp = oreo.friends->head;
	while(temp!=nullptr) {									// copying friends			
		friends->add_chirp_post(temp->message);
		friends->head->like = temp->like;
		friends->head->num_likes = temp->num_likes;
		friends->head->reChirp = temp->reChirp;
		temp = temp->next;
	}



}
Chirp::~Chirp() {				//destructor
	//post condition: destructs the calling object
	postPtr temp = head;
	int size=0;
	while(temp!= nullptr) {
		size++;
		temp = temp->next;
	}

	//std::cout<<"Deleting posts..";					//for checking destructor
	for(int i=0;i<size;i++) {
		delete_chirp(1);
	}

	temp = friends->head;
	size=0;
	//std::cout<<"Deleting friends post \n";			//for checking destructor
	while(temp!= nullptr) {
		size++;
		temp = temp->next;
	}
	for(int i=0;i<size;i++) {
		friends->delete_chirp(1);
	}

}

void Chirp::menu() {
	//POST codition: it is the main menu user will see after setting up their account.
	int temp;
	std::string str;					//temporary variables for the work
	bool flag = true;			// it helps to decide whether to stay here or to go back to main()
	while(flag == true) {
		std::cout<<"-------------------Select an option. \n";
		std::cout<<"			1. Chirp. (Add a new post to your wall) \n";
		std::cout<<"			2. Scroll through your Chirps(posts) \n";
		std::cout<<"			3. Scroll through friend Chirps(posts) \n";
		std::cout<<"			4. Change Display name \n";
		std::cout<<"			5. Exit App"<<std::endl;
		temp = inputCheck(1,5);
		switch(temp) {
			case 1:
				std::cout<<"Enter a new message for post: "<<std::endl;
				std::cin.ignore();
				str = check_string();	
				if(add_chirp_post(str) == true)
					std::cout<<"Post added succesfully";
				break;
			case 2:
				scroll_my_posts();
				break;
			case 3:
				//std::cout<<friends->getUser();
				scroll_thru_friend_posts();
				break;
			case 4:
				change_dn();
				std::cout<<"\n Display name changed successfully \n press any button to go back to main menu.";
				break;
			case 5:
				// have to exit the function so 
				flag = false;
				break;
			default:
			//I don't think user can reach here but still:
				std::cout<<"Invalid option \n Going back to main \n";
				break;		
		}
	}

}

postPtr Chirp::previous_chirp() {
	//POSTCONDITION: returns pointer to the previous chirp in the stack
	return head->previous;
}
postPtr Chirp::next_chirp() {
	//POSTCONDITION: returns pointer to the next chirp in the stack
	return head->next;
}
bool Chirp::isEmpty() const{
	//POSTCONDITION: returns true if the wall is empty.
	return head == nullptr;
}
void Chirp::display_chirp(postPtr it, int i){

	int count = 0;
	std::cout <<" _______________________________________________________" << std::endl; //55x
	std::cout << "|"<< std::setw(25) << std::right << ">>>>> Post #" << i << " <<<<"<< std::setw(24) << "|" << std::endl;
	std::cout << "| ";
	for(int j = 0; j < it->message.length(); j++){
	    std::cout << it->message[j];
	    count++;
	    if((count > 45 && it->message[j] == ' ') || it->message[j+1] == '\0'){
	 	   for(int k = 0; k < 53-count; k++){
	       		std::cout << " ";
	    	}
	    	std::cout << "|" << std::endl;
	        count = 0;
	        if(it->message[j+1] == '\0') break;
	    	std::cout << "| ";
	  	}
	}

	std::cout << "\tLikes: ";
	if(it->like) std::cout << heart << it->num_likes << "\t";
	else std::cout << "\t";
	std::cout << "\tReChirps: ";
	if(it->reChirp) std::cout << arrow << std::endl;
	else std::cout << std::endl;
	std::cout <<" _______________________________________________________\n\n"; //55x
}

bool Chirp::add_like(postPtr it) {
	//POSTCONDITION: adds 1 more like to the post
	if(it->like == false) {
		it->num_likes = 1;
		it->like = true;
	}
	else
		it->num_likes++;
	return true;
}

char* Chirp::scroll_thru_posts() {		//function to help the scrollers out there
	char* ch = new char;
	//this is an additional functionality 
	// if the user accidentaly types previous instead of just a single letter then it will automatically detect 'p' the first alphabet.
	std::cout<<"  (p)revious		(n)ext"<<std::endl;
	std::cout<<"  (l)ike			(r)eChirp		(d)elete		(e)xit"<<std::endl;
	std::cin>>ch;
	std::cin.clear();
	//char* temp = new char;
	//temp = ch[0];
	//delete ch;
	return ch;
}

void Chirp::scroll_my_posts() {
	if(isEmpty() == true) {
		char ch;
		std::cout<<"There is nothing to scroll here. \n";
		std::cout<<"Try to add new posts through menu"<<std::endl;
	}
	else {
		postPtr it = head;
		int count = 1; 
		std::cout<<"Displaying "<<getDN()<<"'s posts:"<<std::endl;
		display_chirp(it, 1);
		bool flag = true;
		char* choice = new char;
		while(flag == true) {
			choice = scroll_thru_posts();			//calling function to ask for next or previous chirps scrolling etc
			char ch = choice[0];
			switch(ch) {
				case 'p':case 'P':
					if(it->previous == nullptr) {
						std::cout<<"No previous Chirps to see. \n";
						flag = false;
					}
					else {
						count--;
						it = it->previous;
						display_chirp(it, count);
					}
					//keep_rolling();
				break; 
				case 'n':case 'N':
					if(it->next == nullptr) {
						std::cout<<"You have reached the end. \n";
						flag = false;
					}
					else {
						count++;
						it = it->next;
						display_chirp(it, count);
					}
					//keep_rolling
				break;
				case 'l':case 'L':
					if(add_like(it)==true)
						std::cout<<"Liked successfully!"<<std::endl;
					display_chirp(it,count);
					//keep_rolling
				break;
				case 'r':case 'R':
					reChirp(it);
					std::cout<<"reChirped successfully!"<<std::endl;
					it = head;										//	Displaying again from starting
					display_chirp(it, count);						//	diplaying rechirped ;post which became our new head
					//keep_rolling
				break;

				case 'd':case 'D':
					it = it->next;
					std::cout<<"deleting post number: "<<count<<std::endl;
					if(delete_chirp(count) == true) {
						std::cout<<"Chirp deleted successfully "<<std::endl;
						flag = false;
						//no more rolling
					}
					else {
						std::cout<<"Chirp deleting failed \n";
						display_chirp(it, count);
						//keep rolling
					}
				break;
				case 'e':case 'E':
					flag = false;
					//No more rolling back to main
				break;
				default: 
					std::cout<<"Invalid option \n Try again (Press 'E' or 'e' to Exit)!"<<std::endl;
					flag = true;
					//keep_rolling
				break;
			}		//switch case ends here
		}			// while ends here
		delete choice;
	}				//else ends here
}

//lets scroll through friends posts now
void Chirp::scroll_thru_friend_posts() {
	if(friends->head == nullptr) {
		std::cout<<"There is nothing to scroll here. \n";
	}
	else {
		postPtr it = friends->head;
		int count = 1; 
		std::cout<<"Displaying "<<friends->getDN()<<"'s posts:"<<std::endl;
		display_chirp(it, 1);
		bool flag = true;
		char* choice = new char;
		while(flag == true) {
			choice = scroll_thru_posts();			//calling function to ask for next or previous chirps scrolling etc
			char ch = choice[0];
			switch(ch) {
				case 'p':case 'P':
					if(it->previous == nullptr) {
						std::cout<<"No previous Chirps to see. \n";
						flag = false;
					}
					else {
						count--;
						it = it->previous;
						display_chirp(it, count);
					}
					//keep_rolling();
				break; 
				case 'n':case 'N':
					if(it->next == nullptr) {
						std::cout<<"You have reached the end. \n";
						flag = false;
					}
					else {
						count++;
						it = it->next;
						friends->display_chirp(it, count);
					}
					//keep_rolling
				break;
				case 'l':case 'L':
					if(friends->add_like(it)==true)
						std::cout<<"Liked successfully!"<<std::endl;
					friends->display_chirp(it,count);
					//keep_rolling
				break;
				case 'r':case 'R':
					reChirp(it);
					std::cout<<"reChirped successfully to your Wall!"<<std::endl;
					it = head;										//	Displaying again from starting
					friends->display_chirp(it, count);						//	diplaying rechirped ;post which became our new head
					//keep_rolling
				break;

				case 'd':case 'D':
					it = it->next;
					std::cout<<"deleting post number: "<<count<<std::endl;
					if(friends->delete_chirp(count) == true) {
						std::cout<<"Chirp deleted successfully "<<std::endl;
						flag = false;
						//no more rolling
					}
					else
						std::cout<<"Chirp deleting failed \n";					
					//keep_rolling
				break;
				case 'e':case 'E':
					flag = false;
					//No more rolling back to main
				break;
				default: 
					std::cout<<"Invalid option \n Try again (Press 'E' or 'e' to Exit)!"<<std::endl;
					flag = true;
					//keep_rolling
				break;
			}		//switch case ends here
		}			// while ends here
		delete choice;
	}				//else ends here
}


void Chirp::reChirp(postPtr it) {
	//POSTCONDITITION: adds a new post to top but it would be rechirped.
	add_chirp_post(it->message);
	head->reChirp = true;
}

void Chirp::add_friends() {
	//POSTCODITION: the function add(s) friend(s) {1 right now} to the calling funtion 
	//By default file for reading is friends_list.dat
    std::ifstream fin("friends_list.dat");
    std::string word;

    if(!fin){
        std::cout << "Can not find the specified file\n";
        exit(1); //terminate with an error
    } //else opened just fine

  //Chirp f; //make a new member "friends" of the class
  fin >> word;

  friends = new Chirp;

  friends->setUser(word);
  fin >> word;
  friends->setDN(word);
  for(std::string line; getline(fin, line); ){
      if(line == "") continue;
      line.resize(MSG); //as a precaution
      friends->add_chirp_post(line);
  }
}

bool Chirp::add_chirp_post(std::string str) {
	//POSTCONDITION: parameter string for the post
	//returns true if the process of adding is successfull false instead
	postPtr malone = new post;
	malone->message = str;
	malone->like = false;
	malone->num_likes = 0;
	malone->reChirp = false;

	if(head == nullptr) {
		head = malone;
		head->next = nullptr;
		head->previous = nullptr;
	}
	else {
		malone->next = head;
		malone->previous = nullptr;
		head->previous = malone;
		head = malone;
	}
	return true;
}

void Chirp::change_dn() {
	//POSTCONDITION: changes Display name of the user.
	std::string temp;
	std::cin.ignore();
	std::cout<<"Enter new display name: "<<std::endl;
	temp = check_string();
	display_name = temp;
}
void Chirp::setDN(const std::string newDN) {
	//POSTCONDITION: changes Display name of the user.
	display_name = newDN;
}

void Chirp::setUser(const std::string str) {
	//POSTCONDITION: set user name to str
	user = str;
}

std::string Chirp::getUser() const{
	//POSTCONDITION: returns a std::string carrying username
	return user;
}

std::string Chirp::getDN() const {
	//POSTCONDITION: returns display_name.
	return display_name;
}
bool Chirp::delete_chirp(int position) {
	if(head == nullptr){				// when there is nothing to delete.
		std::cout<<"Cannot delete post. The wall is already empty"<<std::endl;
		return false;
	}
	else {
		int i = 1;		//starting of the head
		postPtr kanye = head;
		bool flag = true;
		while(kanye != nullptr && flag == true) {			// in order to find the pointer to be deleted.
			if(i == position) {				// found the pointer to be deleted
				flag = false;
				break;
			}
			i++;
			kanye = kanye->next;
		}
		if(flag == true)
			return false;
		//now the deletinon part----------hard part
		// kanye has 2 attributes next and previous, which can have 2 values either equals to nullptr or not
		// this gives us total of 4 cases.
		//lets 0 denotes the value of ptr to be nullptr and 1 denotes otherwise, then:
		if(kanye->next == nullptr && kanye->previous == nullptr)	{			//case 00
			// to visualize this is the case where only head is there i.e kanye is equals to head
			//std::cout<<"Deleting......:"<<kanye->message;			//for chrcking destructor
			delete kanye;			
			head = nullptr;
			return true;
		}
		else if(kanye->next != nullptr && kanye->previous == nullptr)	{		//case 01
			//to visualize this is the case when we are deleting head but there are other things in the list
			head = kanye->next;
			head->previous = nullptr;
			//std::cout<<"Deleting......:"<<kanye->message;			//for checking destructor
			delete kanye;
			return true;
		}
		else if(kanye->next == nullptr && kanye->previous != nullptr)	{		//case 10
			// to visualize we are at the end
			kanye->previous->next = nullptr;
			//std::cout<<"Deleting......:"<<kanye->message;			//for checking destructor
			delete kanye;
			return true;
		}
		else if(kanye->next != nullptr && kanye->previous != nullptr) {	//case 11
			// to visualize we are in the middle
			kanye->previous->next = kanye->next;
			kanye->next->previous = kanye->previous;
			//std::cout<<"Deleting......:"<<kanye->message;			//for checking destructor
			delete kanye;
			return true;
		}

	}
	return false;	// we won't reach here
}

int main(){
	Chirp ch;
	ch.setup_account();
	ch.menu();
	
	std::cout << "See you next time\n";
	return 0;
}