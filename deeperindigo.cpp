#include <random>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <map>
#include <set>

//Nicholas Parsly - 2023
using namespace std;

//Compare function for the priority queue
bool compFunc(pair <double, string> p1, pair <double, string> p2) {


	return (p1.first < p2.first);
}


struct YahtzeeGame {

	//Could I have put a bunch of these in a vector to save on having to declare all these?
	//Yes, but there is a non-zero chance I could have forgotten what score-type indexes where,
	//so to play it safe everything gets an int
	int acescore;
	int twoscore;
	int threescore;
	int fourscore;
	int fivescore;
	int sixscore;
	int tkind;
	int fkind;
	int fhouse;
	int sstraight;
	int lstraight;
	int yahtzee;
	int chance;

	int upcount;
	int updist;
	int yahtcount;
	int yahtbonus;

	int check_aces;
	int check_twos;
	int check_threes;
	int check_fours;
	int check_fives;
	int check_sixes;
	int check_tkind;
	int check_fkind;
	int check_fhouse;
	int check_sstraight;
	int check_lstraight;
	int check_yahtzee;
	int check_chance;

	int turn;

	//These store the probabilities of specific rolls for each amount of dice you can roll
	map <string, double> onerolls;
	map <string, double> tworolls;
	map <string, double> threerolls;
	map <string, double> fourrolls;
	map <string, double> fiverolls;

	YahtzeeGame();
	void printCheck();
	double scoreCalc(int ones, int twos, int threes, int fours, int fives, int sixes, int maxmatch);
	double sumScore(string roll);
	double extendScore(string roll);
	void score(string roll);
	void finalScore();
};

int main() {


	stringstream ss;
	string lines;
	ifstream fin("yzprobs.txt");
	string s;
	double d;
	int stringsize;
	default_random_engine rgen( (unsigned) time(NULL));
	uniform_int_distribution<int> dist1(1, 6);
	YahtzeeGame yg;

	//Get the probabilties, add them to the appropriate map
	while (getline(fin, lines)) {

		ss.str(lines);
		ss >> s;
		ss >> d;

		stringsize = s.size();

		//Do I look like I use switch statements?
		if (stringsize == 1) {

			yg.onerolls.insert(make_pair(s, d));
		}

		else if (stringsize == 2) {

			yg.tworolls.insert(make_pair(s, d));
		}

		else if (stringsize == 3) {

			yg.threerolls.insert(make_pair(s, d));
		}

		else if (stringsize == 4) {

			yg.fourrolls.insert(make_pair(s, d));
		}

		else if (stringsize == 5) {

			yg.fiverolls.insert(make_pair(s, d));
		}

		ss.clear();
	}

	fin.close();
	//yg.printCheck();	

	string roll;
	string teststring;
	int ones;
	int twos;
	int threes;
	int fours;
	int fives;
	int sixes;
	int maxmatch;
	int temp;
	int dice;
	int rsize;
	double probscore;
	priority_queue <pair <double, string>, vector <pair <double, string> >, decltype(&compFunc)> pq(&compFunc);

	for (int i = 0; i < 13; i++) {

		cout << "Turn " << i + 1 << "\n";
		yg.turn = i + 1;
		roll = "";
		ones = 0;
		twos = 0;
		threes = 0;
		fours = 0;
		fives = 0;
		sixes = 0;
		maxmatch = 0;
		pq = priority_queue <pair <double, string>, vector <pair <double, string> >, decltype(&compFunc)> (&compFunc);


		//First roll
		for (int j = 0; j < 5; j++) {

			dice = dist1(rgen);
			if (dice == 1) {

				ones = ones + 1;
				temp = ones;
			}

			else if (dice == 2) {

				twos = twos + 1;
				temp = twos;
			}

			else if (dice == 3) {

				threes = threes + 1;
				temp = threes;
			}

			else if (dice == 4) {

				fours = fours + 1;
				temp = fours;
			}

			else if (dice == 5) {

				fives = fives + 1;
				temp = fives;
			}

			else if (dice == 6) {

				sixes = sixes + 1;
				temp = sixes;
			}

			if (temp > maxmatch) {

				maxmatch = temp;
			}

			roll = roll + to_string(dice);
		}

		sort(roll.begin(), roll.end());
		cout << "Roll 1: " << roll << "\n";

		//If it's a yahtzee, just take it, assuming it already hasn't marked the yahtzee as zero
		if (maxmatch == 5 && (yg.check_yahtzee || yg.yahtbonus)) {

			yg.score(roll);
			cout << "\n";
			continue;
		}

		//Take a large straight on the first roll if it hasn't been marked yet
		if (((ones && twos && threes && fours && fives) || (twos && threes && fours && fives && sixes)) 
				&& yg.check_lstraight) {

			yg.score(roll);
			cout << "\n";
			continue;
		}

		//First Hold
		for (int j = 0; j < 32; j++) {

			teststring = "";
			if (j & 1) {

				teststring.push_back(roll.at(0));
			}

			if (j & 2) {

				teststring.push_back(roll.at(1));
			}

			if (j & 4) {

				teststring.push_back(roll.at(2));
			}

			if (j & 8) {

				teststring.push_back(roll.at(3));
			}

			if (j & 16) {

				teststring.push_back(roll.at(4));
			}

			probscore = yg.extendScore(teststring);
			pq.push(make_pair(probscore, teststring));
		}
		  
		//Second Roll
		roll = pq.top().second;
		cout << "Hold 1: " << roll << "\n";
		rsize = roll.size();

		for (int j = 0; j < 5 - rsize; j++) {

			dice = dist1(rgen);
			roll = roll + to_string(dice);
		}

		sort(roll.begin(), roll.end());	
		cout << "Roll 2: " << roll << "\n";		

		ones = 0;
		twos = 0;
		threes = 0;
		fours = 0;
		fives = 0;
		sixes = 0;
		maxmatch = 0;

		for (int j = 0; j < 5; j++) {

			if (roll.at(j) == '1') {

				ones = ones + 1;
				temp = ones;
			}

			else if (roll.at(j) == '2') {

				twos = twos + 1;
				temp = twos;
			}

			else if (roll.at(j) == '3') {

				threes = threes + 1;
				temp = threes;
			}

			else if (roll.at(j) == '4') {

				fours = fours + 1;
				temp = fours;
			}

			else if (roll.at(j) == '5') {

				fives = fives + 1;
				temp = fives;
			}

			else if (roll.at(j) == '6') {

				sixes = sixes + 1;
				temp = sixes;
			}

			if (temp > maxmatch) {

				maxmatch = temp;
			}
		}

		//Once again, just take a yahtzee if it hasn't been marked as zero
		if (maxmatch == 5 && (yg.check_yahtzee || yg.yahtbonus)) {

			yg.score(roll);
			cout << "\n";
			continue;
		}

		//Second Hold
		pq = priority_queue <pair <double, string>, vector <pair <double, string> >, decltype(&compFunc)> (&compFunc);
		for (int j = 0; j < 32; j++) {

			teststring = "";
			if (j & 1) {

				teststring.push_back(roll.at(0));
			}

			if (j & 2) {

				teststring.push_back(roll.at(1));
			}

			if (j & 4) {

				teststring.push_back(roll.at(2));
			}

			if (j & 8) {

				teststring.push_back(roll.at(3));
			}

			if (j & 16) {

				teststring.push_back(roll.at(4));
			}

			probscore = yg.sumScore(teststring);
			pq.push(make_pair(probscore, teststring));
		}
		//Third Roll		

		roll = pq.top().second;
		cout << "Hold 2: " << roll << "\n";
		rsize = roll.size();

		for (int j = 0; j < 5 - rsize; j++) {

			dice = dist1(rgen);
			roll = roll + to_string(dice);
		}

		sort(roll.begin(), roll.end());

		cout << "Roll 3: " << roll << "\n";
		
		//Score the final roll
		yg.score(roll);
		cout << "\n";
	}

	yg.finalScore();
	return 0;
}

YahtzeeGame::YahtzeeGame() {

	acescore = 0;
	twoscore = 0;
	threescore = 0;
	fourscore = 0;
	fivescore = 0;
	sixscore = 0;
	tkind = 0;
	fkind = 0;
	fhouse = 0;
	sstraight = 0;
	lstraight = 0;
	yahtzee = 0;
	chance = 0;

	upcount = 0;
	updist = 63;
	yahtcount = 0;
	yahtbonus = 0;

	//Why 1's? Because we'll use these in the heuristic function
	check_aces = 1;
	check_twos = 1;
	check_threes = 1;
	check_fours = 1;
	check_fives = 1;
	check_sixes = 1;
	check_tkind = 1;
	check_fkind = 1;
	check_fhouse = 1;
	check_sstraight = 1;
	check_lstraight = 1;
	check_yahtzee = 1;
	check_chance = 1;
}

//Debugging function
void YahtzeeGame::printCheck() {

	map <string, double>::iterator mit;

	cout << "Ones: " << onerolls.size() << "\n";

	for (mit = onerolls.begin(); mit != onerolls.end(); mit++) {

		cout << mit->first << ": " << mit->second << "\n";
	}

	cout << "Twos: " << tworolls.size() << "\n";

	for (mit = tworolls.begin(); mit != tworolls.end(); mit++) {

		cout << mit->first << ": " << mit->second << "\n";
	}

	cout << "Threes: " << threerolls.size() << "\n";

	for (mit = threerolls.begin(); mit != threerolls.end(); mit++) {

		cout << mit->first << ": " << mit->second << "\n";
	}

	cout << "Fours: " << fourrolls.size() << "\n";

	for (mit = fourrolls.begin(); mit != fourrolls.end(); mit++) {

		cout << mit->first << ": " << mit->second << "\n";
	}


	cout << "Fives: " << fiverolls.size() << "\n";
	for (mit = fiverolls.begin(); mit != fiverolls.end(); mit++) {

		cout << mit->first << ": " << mit->second << "\n";
	}
}

//Get the possible score for each box for a roll
double YahtzeeGame::scoreCalc(int ones, int twos, int threes, int fours, int fives, int sixes, int maxmatch) {

	double sum = 0.0;
	double totaldice;

	totaldice = (ones * 1.0) + (twos * 2.0) + (threes * 3.0) + (fours * 4.0) + (fives * 5.0) + (sixes * 6.0);

	//Upper Section
	sum = sum + (ones * 1.0 * check_aces);
	sum = sum + (twos * 2.0 * check_twos);
	sum = sum + (threes * 3.0 * check_threes);
	sum = sum + (fours * 4.0 * check_fours);
	sum = sum + (fives * 5.0 * check_fives);
	sum = sum + (sixes * 6.0 * check_sixes);

	//3-of-a-kind
	if (maxmatch >= 3) {

		sum = sum + (totaldice * check_tkind);
	}

	//4-of-a-kind
	if (maxmatch >= 4) {

		sum = sum + (totaldice * check_fkind);
	}

	//Full house
	if (maxmatch == 3) {

		if ((ones == 2) || (twos == 2) || (threes == 2) || (fours == 2) || (fives == 2) || (sixes == 2)) {

			sum = sum + (25.0 * check_fhouse);
		}
	}

	else if (maxmatch == 5) {

		sum = sum + (25.0 * check_fhouse);
	}

	//Small straight
	if ((ones && twos && threes && fours) || (twos && threes && fours && fives) || (threes && fours && fives && sixes)) {

		sum = sum + (30.0 * check_sstraight);
	}

	else if (maxmatch == 5) {

		sum = sum + (30.0 * check_sstraight);
	}

	//Large straight
	if ((ones && twos && threes && fours && fives) || (twos && threes && fours && fives && sixes)) {

		sum = sum + (40.0 * check_lstraight);
	}

	else if (maxmatch == 5) {

		sum = sum + (40.0 * check_lstraight);
	}

	//Yahtzee
	if (maxmatch == 5) {

		sum = sum + (50.0 * check_yahtzee);
	}

	sum = sum + (totaldice * check_chance);

	return sum;
}

//Second layer of scoring (used for 2nd roll and extendScore)
double YahtzeeGame::sumScore(string roll) {

	map <string, double> tempmap;
	map <string, double>::iterator mit;
	int ones;
	int twos;
	int threes;
	int fours;
	int fives;
	int sixes;
	int maxmatch;
	int temp;
	double sum;
	string tempstring;

	//Get the right map to iterate through (5 - size of string)
	if (roll.size() == 0) {

		tempmap = fiverolls;
	}

	else if (roll.size() == 1) {

		tempmap = fourrolls;
	}

	else if (roll.size() == 2) {

		tempmap = threerolls;
	}

	else if (roll.size() == 3) {

		tempmap = tworolls;
	}

	else if (roll.size() == 4) {

		tempmap = onerolls;
	}

	else {
		//Case where its an all hold
		ones = 0;
		twos = 0;
		threes = 0;
		fours = 0;
		fives = 0;
		sixes = 0;
		maxmatch = 0;

		for (int i = 0 ; i < 5; i++) {

			if (roll.at(i) == '1') {

				ones = ones + 1;
				temp = ones;
			}

			else if (roll.at(i) == '2') {

				twos = twos + 1;
				temp = twos;
			}

			else if (roll.at(i) == '3') {

				threes = threes + 1;
				temp = threes;
			}

			else if (roll.at(i) == '4') {

				fours = fours + 1;
				temp = fours;
			}

			else if (roll.at(i) == '5') {

				fives = fives + 1;
				temp = fives;
			}

			else if (roll.at(i) == '6') {

				sixes = sixes + 1;
				temp = sixes;
			}

			if (temp > maxmatch) {
				maxmatch = temp;
			}
		}

		//Just get the info for the scoring and return that
		return scoreCalc(ones, twos, threes, fours, fives, sixes, maxmatch);
	}

	sum = 0.0;

	//Go through each possible roll for a given hold
	for (mit = tempmap.begin(); mit != tempmap.end(); mit++) {

		tempstring = roll + mit->first;

		ones = 0;
		twos = 0;
		threes = 0;
		fours = 0;
		fives = 0;
		sixes = 0;
		maxmatch = 0;

		for (int i = 0 ; i < 5; i++) {

			if (tempstring.at(i) == '1') {

				ones = ones + 1;
				temp = ones;
			}

			else if (tempstring.at(i) == '2') {

				twos = twos + 1;
				temp = twos;
			}

			else if (tempstring.at(i) == '3') {

				threes = threes + 1;
				temp = threes;
			}

			else if (tempstring.at(i) == '4') {

				fours = fours + 1;
				temp = fours;
			}

			else if (tempstring.at(i) == '5') {

				fives = fives + 1;
				temp = fives;
			}

			else if (tempstring.at(i) == '6') {

				sixes = sixes + 1;
				temp = sixes;
			}

			if (temp > maxmatch) {

				maxmatch = temp;
			}
		}

		//For each roll, find the max score, multiply it by the probability of rolling it
		//and then add it to the total sum
		sum = sum + scoreCalc(ones, twos, threes, fours, fives, sixes, maxmatch) * mit->second;
	}

	return sum;
}

//The two layer sumScore. Used for the 1st hold
double YahtzeeGame::extendScore(string roll) {

	map <string, double> tempmap;
	map <string, double>::iterator mit;
	double sum;
	string tempstring;
	string teststring;

	sum = 0.0;

	//One again, find the right map 
	if (roll.size() == 0) {

		tempmap = fiverolls;
	}

	else if (roll.size() == 1) {

		tempmap = fourrolls;
	}

	else if (roll.size() == 2) {

		tempmap = threerolls;
	}

	else if (roll.size() == 3) {

		tempmap = tworolls;
	}

	else if (roll.size() == 4) {

		tempmap = onerolls;
	}

	else {
	
		//Case where its an all hold
		tempstring = roll;
		for (int j = 0; j < 32; j++) {

			teststring = "";
			if (j & 1) {

				teststring.push_back(tempstring.at(0));
			}

			if (j & 2) {

				teststring.push_back(tempstring.at(1));
			}

			if (j & 4) {

				teststring.push_back(tempstring.at(2));
			}

			if (j & 8) {

				teststring.push_back(tempstring.at(3));
			}

			if (j & 16) {

				teststring.push_back(tempstring.at(4));
			}

			sum = sum + (sumScore(teststring));
		}
			
		return sum;
	}


	//Go through each possible roll for the hold
	for (mit = tempmap.begin(); mit != tempmap.end(); mit++) {

		tempstring = roll + mit->first;

		//Then consider every possible hold you can do for that set of dice
		for (int j = 0; j < 32; j++) {

			teststring = "";
			if (j & 1) {

				teststring.push_back(tempstring.at(0));
			}

			if (j & 2) {

				teststring.push_back(tempstring.at(1));
			}

			if (j & 4) {

				teststring.push_back(tempstring.at(2));
			}

			if (j & 8) {

				teststring.push_back(tempstring.at(3));
			}

			if (j & 16) {

				teststring.push_back(tempstring.at(4));
			}

			//Multiply the odds of rolling that set of dice by the sumScore for the secondary hold, then add it to the
			//running sum
			sum = sum + mit->second * (sumScore(teststring));
		}
	}

	return sum;
}

//Scoring function. Chooses what to mark
void YahtzeeGame::score(string roll) {

	int ones;
	int twos;
	int threes;
	int fours;
	int fives;
	int sixes;
	int temp;
	int maxmatch;
	int mnumval;
	int aimup;
	bool yaht;
	bool mnumcheck;
	char tempnum;
	char mnum;
	int totaldice;
	int oneval;
	int twoval;
	int threeval;
	int fourval;
	int fiveval;
	int sixval;

	ones = 0;
	twos = 0;
	threes = 0;
	fours = 0;
	fives = 0;
	sixes = 0;
	maxmatch = 0;
	yaht = false;
	mnumcheck = false;

	//aimup is the turn at which it will prefer getting the upper section bonus over going for a lower section score
	aimup = 0;

	//First get the basic info about the last roll
	for (int i = 0 ; i < 5; i++) {

		if (roll.at(i) == '1') {

			ones = ones + 1;
			temp = ones;
			tempnum = '1';
		}

		else if (roll.at(i) == '2') {

			twos = twos + 1;
			temp = twos;
			tempnum = '2';

		}

		else if (roll.at(i) == '3') {

			threes = threes + 1;
			temp = threes;
			tempnum = '3';
		}

		else if (roll.at(i) == '4') {

			fours = fours + 1;
			temp = fours;
			tempnum = '4';
		}

		else if (roll.at(i) == '5') {

			fives = fives + 1;
			temp = fives;
			tempnum = '5';
		}

		else if (roll.at(i) == '6') {

			sixes = sixes + 1;
			temp = sixes;
			tempnum = '6';
		}

		if (temp > maxmatch) {

			//Maxmatch is the highest number of matches
			//mnum is the die value with the highest number of matches
			maxmatch = temp;
			mnum = tempnum;
		}
	}

	//Get sum of all dice and the sums of individual values
	totaldice = (ones * 1) + (twos * 2) + (threes * 3) + (fours * 4) + (fives * 5) + (sixes * 6);
	oneval = ones * 1;
	twoval = twos * 2;
	threeval = threes * 3;
	fourval = fours * 4;
	fiveval = fives * 5;
	sixval = sixes * 6;


	//Determine if there is a yahtzee
	if (maxmatch == 5) {

		yaht = true;
	}


	//See if the highest matched value is still available to fill
	if (mnum == '1') {

		mnumcheck = check_aces;
		mnumval = oneval;
	}

	else if (mnum == '2') {

		mnumcheck = check_twos;
		mnumval = twoval;

	}

	else if (mnum == '3') {

		mnumcheck = check_threes;
		mnumval = threeval;
	}

	else if (mnum == '4') {

		mnumcheck = check_fours;
		mnumval = fourval;
	}

	else if (mnum == '5') {

		mnumcheck = check_fives;
		mnumval = fiveval;
	}

	else if (mnum == '6') {

		mnumcheck = check_sixes;
		mnumval = sixval;
	}

	//If it gets a yahtzee and hasn't marked one yet, score it
	if (yaht && check_yahtzee) {

		check_yahtzee = 0;
		yahtzee = 50;
		yahtcount = 0;
		yahtbonus = 1;
		cout << "Score: Yahtzee - 50\n";
		return;
	}

	//Add to yahtzee bonus count for repeat yahtzees
	if (yaht && yahtbonus) {

		yahtcount = yahtcount + 1;
	}


	//I use a lot of double if statements in this section for readability and because I figure they will be optimized 
	//together by the compiler
	//If this were a really serious program, I would have not be using the double ifs

	//Upper section priority for yahtzee joker rule
	if (yaht) {

		//Can you tell I wrote these before adding mnumcheck
		if (mnum == '1') {

			if (check_aces) {

				check_aces = 0;
				acescore = 5;
				upcount = upcount + 5;
				cout << "Score: Aces - 5\n";
				return;
			}
		}

		else if (mnum == '2') {

			if (check_twos) {

				check_twos = 0;
				twoscore = 10;
				upcount = upcount + 10;
				cout << "Score: Twos - 10\n";
				return;
			}
		}

		else if (mnum == '3') {

			if (check_threes) {

				check_threes = 0;
				threescore = 15;
				upcount = upcount + 15;
				cout << "Score: Threes - 15\n";
				return;
			}
		}

		else if (mnum == '4') {

			if (check_fours) {

				check_fours = 0;
				fourscore = 20;
				upcount = upcount + 20;
				cout << "Score: Fours - 20\n";
				return;
			}
		}

		else if (mnum == '5') {

			if (check_fives) {

				check_fives = 0;
				fivescore = 25;
				upcount = upcount + 25;
				cout << "Score: Fives - 25\n";
				return;
			}
		}

		else if (mnum == '6') {

			if (check_sixes) {

				check_sixes = 0;
				sixscore = 30;
				upcount = upcount + 30;
				cout << "Score: Sixes - 30\n";
				return;
			}
		}
	}


	//Large straight
	if (check_lstraight) {

		if (yaht || (ones && twos && threes && fours && fives) || (twos && threes && fours && fives && sixes)) {

			check_lstraight = 0;
			lstraight = 40;
			cout << "Score: Large Straight - 40\n";

			return;
		}
	}

	//Small straight
	if (check_sstraight) {

		if (yaht || (ones && twos && threes && fours) || (twos && threes && fours && fives) ||
				(threes && fours && fives && sixes)) {

			check_sstraight = 0;
			sstraight = 30;
			cout << "Score: Small Straight - 30\n";
			return;
		}
	}

	//Full house
	if (check_fhouse) {

		if (yaht || 
				((maxmatch == 3) && (ones == 2 || twos == 2 || threes == 2 || fours == 2 || fives == 2 || sixes == 2))) {

			if ((turn > aimup) && mnumcheck) {

				//If using the three-of-a-kind would fulfill the upper section bonus, prioritize it over the full house
				if (upcount + mnumval >= 63) {

					if (mnum == '1') {

						check_aces = 0;
						acescore = mnumval;
						upcount = upcount + acescore;
						cout << "Score: Aces - " << acescore << "\n";
						return;

					}

					else if (mnum == '2') {

						check_twos = 0;
						twoscore = mnumval;
						upcount = upcount + twoscore;
						cout << "Score: Twos - " << twoscore << "\n";
						return;

					}

					else if (mnum == '3') {

						check_threes = 0;
						threescore = mnumval;
						upcount = upcount + threescore;
						cout << "Score: Threes - " << threescore << "\n";
						return;

					}

					else if (mnum == '4') {

						check_fours = 0;
						fourscore = mnumval;
						upcount = upcount + fourscore;
						cout << "Score: Fours - " << fourscore << "\n";
						return;

					}

					else if (mnum == '5') {

						check_fives = 0;
						fivescore = mnumval;
						upcount = upcount + fivescore;
						cout << "Score: Fives - " << fivescore << "\n";
						return;

					}

					else if (mnum == '6') {

						check_sixes = 0;
						sixscore = mnumval;
						upcount = upcount + sixscore;
						cout << "Score: Sixes - " << sixscore << "\n";
						return;

					}
				}
			}

			//If it's a really good sum, and its early enough, go for either four-of-a-kind of three-of-a-kind
			if (totaldice >= 25) {

				if (check_tkind || (yaht && check_fkind)) {

					if (turn < 8) {

						if (yaht && check_fkind) {

							check_fkind = 0;
							fkind = totaldice;
							cout << "Score: Four-of-a-kind - " << totaldice << "\n";
							return;
						}

						if (check_tkind) {

							check_tkind = 0;
							tkind = totaldice;
							cout << "Score: Three-of-a-kind - " << totaldice << "\n";
							return;
						}
					}
				}
			}

			check_fhouse = 0;
			fhouse = 25;
			cout << "Score: Full House - " << 25 << "\n";
			return;
		}
	}

	//Four-of-a-kind
	if (check_fkind) {

		if (maxmatch >= 4) {
			
			//Prioritize filling the upper section for 4, 5, and 6
			if (mnumcheck && (mnum == '4' || mnum == '5' || mnum == '6')) {

					if (mnum == '4') {

						check_fours = 0;
						fourscore = mnumval;
						upcount = upcount + fourscore;
						cout << "Score: Fours - " << fourscore << "\n";
						return;
					}

					else if (mnum == '5') {

						check_fives = 0;
						fivescore = mnumval;
						upcount = upcount + fivescore;
						cout << "Score: Fives - " << fivescore << "\n";
						return;
					}

					else if (mnum == '6') {

						check_sixes = 0;
						sixscore = mnumval;
						upcount = upcount + sixscore;
						cout << "Score: Sixes - " << sixscore << "\n";
						return;
					}
			}

			if ((turn > aimup) && mnumcheck) {

				//If at least some progress has been made on the upper section, go for the four-of-a-kind in the upper
				//section
				if (upcount + mnumval >= 12) {

					if (mnum == '1') {

						check_aces = 0;
						acescore = mnumval;
						upcount = upcount + acescore;
						cout << "Score: Aces - " << acescore << "\n";
						return;

					}

					else if (mnum == '2') {

						check_twos = 0;
						twoscore = mnumval;
						upcount = upcount + twoscore;
						cout << "Score: Twos - " << twoscore << "\n";
						return;

					}

					else if (mnum == '3') {

						check_threes = 0;
						threescore = mnumval;
						upcount = upcount + threescore;
						cout << "Score: Threes - " << threescore << "\n";
						return;

					}

					else if (mnum == '4') {

						check_fours = 0;
						fourscore = mnumval;
						upcount = upcount + fourscore;
						cout << "Score: Fours - " << fourscore << "\n";
						return;

					}

					else if (mnum == '5') {

						check_fives = 0;
						fivescore = mnumval;
						upcount = upcount + fivescore;
						cout << "Score: Fives - " << fivescore << "\n";
						return;

					}

					else if (mnum == '6') {

						check_sixes = 0;
						sixscore = mnumval;
						upcount = upcount + sixscore;
						cout << "Score: Sixes - " << sixscore << "\n";
						return;

					}
				}
			}

			check_fkind = 0;
			fkind = totaldice;
			cout << "Score: Four-of-a-kind - " << totaldice << "\n";
			return;
		}
	}

	if (check_tkind) {

		if (maxmatch >= 3) {

			//Literally always favor putting a three-of-a-kind in the upper section (this caused a massive improvement
			//in scoring)
			if ((turn > aimup) && mnumcheck) {

				if (upcount + mnumval >= 0) {

					if (mnum == '1') {

						check_aces = 0;
						acescore = mnumval;
						upcount = upcount + acescore;
						cout << "Score: Aces - " << acescore << "\n";
						return;

					}

					else if (mnum == '2') {

						check_twos = 0;
						twoscore = mnumval;
						upcount = upcount + twoscore;
						cout << "Score: Twos - " << twoscore << "\n";
						return;

					}

					else if (mnum == '3') {

						check_threes = 0;
						threescore = mnumval;
						upcount = upcount + threescore;
						cout << "Score: Threes - " << threescore << "\n";
						return;

					}

					else if (mnum == '4') {

						check_fours = 0;
						fourscore = mnumval;
						upcount = upcount + fourscore;
						cout << "Score: Fours - " << fourscore << "\n";
						return;

					}

					else if (mnum == '5') {

						check_fives = 0;
						fivescore = mnumval;
						upcount = upcount + fivescore;
						cout << "Score: Fives - " << fivescore << "\n";
						return;

					}

					else if (mnum == '6') {

						check_sixes = 0;
						sixscore = mnumval;
						upcount = upcount + sixscore;
						cout << "Score: Sixes - " << sixscore << "\n";
						return;

					}
				}
			}

			check_tkind = 0;
			tkind = totaldice;
			cout << "Score: Three-of-a-kind - " << totaldice << "\n";
			return;
		}
	}

	//Go for chance if nothing else works and it's a decently high chance
	if ((maxmatch < 3) && totaldice >= 20 && check_chance) {

		check_chance = 0;
		chance = totaldice;
		cout << "Score: Chance - " << totaldice << "\n";
		return;
	}


	//Score a three-of-a-kind or better in the upper section
	if (maxmatch >= 3) {

		if (mnumcheck) {

			if (mnum == '1') {

				check_aces = 0;
				acescore = mnumval;
				upcount = upcount + acescore;
				cout << "Score: Aces - " << acescore << "\n";
				return;

			}

			else if (mnum == '2') {

				check_twos = 0;
				twoscore = mnumval;
				upcount = upcount + twoscore;
				cout << "Score: Twos - " << twoscore << "\n";
				return;

			}

			else if (mnum == '3') {

				check_threes = 0;
				threescore = mnumval;
				upcount = upcount + threescore;
				cout << "Score: Threes - " << threescore << "\n";
				return;

			}

			else if (mnum == '4') {

				check_fours = 0;
				fourscore = mnumval;
				upcount = upcount + fourscore;
				cout << "Score: Fours - " << fourscore << "\n";
				return;

			}

			else if (mnum == '5') {

				check_fives = 0;
				fivescore = mnumval;
				upcount = upcount + fivescore;
				cout << "Score: Fives - " << fivescore << "\n";
				return;

			}

			else if (mnum == '6') {

				check_sixes = 0;
				sixscore = mnumval;
				upcount = upcount + sixscore;
				cout << "Score: Sixes - " << sixscore << "\n";
				return;
			}
		}
	}


	//If it comes down to eliminating ones or twos early, fill in the one that has more 
	if (check_aces && check_twos) {

		if (ones >= twos) {

			check_aces = 0;
			acescore = oneval;
			upcount = upcount + acescore;
			cout << "Score: Aces - " << acescore << "\n";
			return;
		}

		else {

			check_twos = 0;
			twoscore = twoval;
			upcount = upcount + twoscore;
			cout << "Score: Twos - " << twoscore << "\n";
			return;
		}
	}


	//Eliminate ones and twos early if there's really nothing
	if (check_aces) {
		check_aces = 0;
		acescore = oneval;
		upcount = upcount + acescore;
		cout << "Score: Aces - " << acescore << "\n";
		return;
	}

	if (check_twos) {

		check_twos = 0;
		twoscore = twoval;
		upcount = upcount + twoscore;
		cout << "Score: Twos - " << twoscore << "\n";
		return;
	}

	//Then do bad chance
	if (check_chance) {

		check_chance = 0;
		chance = totaldice;
		cout << "Score: Chance - " << totaldice << "\n";
		return;
	}

	//Check for pairs for threes, fours, fives, or sixes
	if (maxmatch == 2) {

		if (threes == 2 && check_threes) {
				check_threes = 0;
				threescore = mnumval;
				upcount = upcount + threescore;
				cout << "Score: Threes - " << threescore << "\n";
				return;

		}

		else if (fours == 2 && check_fours) {
				check_fours = 0;
				fourscore = mnumval;
				upcount = upcount + fourscore;
				cout << "Score: Fours - " << fourscore << "\n";
				return;

		}

		else if (fives == 2 && check_fives) {
				check_fives = 0;
				fivescore = mnumval;
				upcount = upcount + fivescore;
				cout << "Score: Fives - " << fivescore << "\n";
				return;

		}

		else if (sixes == 2 && check_sixes) {
				check_sixes = 0;
				sixscore = mnumval;
				upcount = upcount + sixscore;
				cout << "Score: Sixes - " << sixscore << "\n";
				return;

		}
	}

	//If it's late, don't try for yahtzee
	if ((turn > 8) && check_yahtzee && (upcount < 63)) {

		check_yahtzee = 0;
		yahtzee = 0;
		cout << "Score: Yahtzee - " << 0 << "\n";
		return;
	}

	//Now eliminate threes
	if (check_threes) {

		check_threes = 0;
		threescore = threeval;
		upcount = upcount + threescore;
		cout << "Score: Threes - " << threescore << "\n";
		return;
	}
	
	//Get rid of yahtzees
	if (check_yahtzee) {
		
		check_yahtzee = 0;
		yahtzee = 0;
		cout << "Score: Yahtzee - " << 0 << "\n";
		return;
	}

	//Go for fours, fives, or sixes with only one die
	if (check_fours && fours == 1) {

		check_fours = 0;
		fourscore = fourval;
		upcount = upcount + fourscore;
		cout << "Score: Fours - " << fourscore << "\n";
		return;
	}

	if (check_fives && fives == 1) {

		check_fives = 0;
		fivescore = fiveval;
		upcount = upcount + fivescore;
		cout << "Score: Fives - " << fivescore << "\n";
		return;
	}

	if (check_sixes && sixes == 1) {

		check_sixes = 0;
		sixscore = sixval;
		upcount = upcount + sixscore;
		cout << "Score: Sixes - " << sixscore << "\n";
		return;
	}

	//Get rid of large straight
	if (check_lstraight) {

		check_lstraight = 0;
		lstraight = 0;
		cout << "Score: Large Straight - " << 0 << "\n";
		return;
	}

	//Then four-of-a-kind
	if (check_fkind) {
			
		check_fkind = 0;
		fkind = 0;
		cout << "Score: Four-of-a-kind - " << 0 << "\n";
		return;
	}
	
	//Now go for fours, fives, and sixes
	if (check_fours) {

		check_fours = 0;
		fourscore = fourval;
		upcount = upcount + fourscore;
		cout << "Score: Fours - " << fourscore << "\n";
		return;
	}

	if (check_fives) {

		check_fives = 0;
		fivescore = fiveval;
		upcount = upcount + fivescore;
		cout << "Score: Fives - " << fivescore << "\n";
		return;
	}

	if (check_sixes) {

		check_sixes = 0;
		sixscore = sixval;
		upcount = upcount + sixscore;
		cout << "Score: Sixes - " << sixscore << "\n";
		return;
	}

	//Then full house, small straight, and three-of-a-kind
	if (check_fhouse) {
		
		check_fhouse = 0;
		fhouse = 0;
		cout << "Score: Full house - " << 0 << "\n";
		return;
	}


	if (check_sstraight) {

		check_sstraight = 0;
		sstraight = 0;
		cout << "Score: Small Straight - " << 0 << "\n";
		return;
	}

	if (check_tkind) {

		check_tkind = 0;
		tkind = 0;
		cout << "Score: Three-of-a-kind - " << 0 << "\n";
		return;
	}

	return;
}

//Calculate and output the final score card of the game
void YahtzeeGame::finalScore() {

	int finaltotal;
	int lowertotal;

/*	
	cout << "Aces: " << check_aces << "\n";
	cout << "Twos: " << check_twos << "\n";
	cout << "Threes: " << check_threes << "\n";
	cout << "Fours: " << check_fours << "\n";
	cout << "Fives: " << check_fives << "\n";
	cout << "Sixes: " << check_sixes << "\n";
	cout << "Three-of-a-kind: " << check_tkind << "\n";
	cout << "Four-of-a-kind: " << check_fkind << "\n";
	cout << "Full House: " << check_fhouse << "\n";
	cout << "Small Straight: " << check_sstraight << "\n";
	cout << "Large Straight: " << check_lstraight << "\n";
	cout << "Yahtzee: " << check_yahtzee << "\n";
*/


	cout << "Aces: " << acescore << "\n";
	cout << "Twos: " << twoscore << "\n";
	cout << "Threes: " << threescore << "\n";
	cout << "Fours: " << fourscore << "\n";
	cout << "Fives: " << fivescore << "\n";
	cout << "Sixes: " << sixscore << "\n";
	cout << "Upper Section Total: " << upcount << "\n";
	cout << "Upper Section Bonus: ";

	if (upcount >= 63) {

		cout << 35 << "\n";
	}

	else {

		cout << 0 << "\n";
	}

	cout << "\n";

	cout << "Three-of-a-kind: " << tkind << "\n";
	cout << "Four-of-a-kind: " << fkind << "\n";
	cout << "Full House: " << fhouse << "\n";
	cout << "Small Straight: " << sstraight << "\n";
	cout << "Large Straight: " << lstraight << "\n";
	cout << "Yahtzee: " << yahtzee << "\n";
	
	lowertotal = tkind;
	lowertotal = lowertotal + fkind;
	lowertotal = lowertotal + fhouse;
	lowertotal = lowertotal + sstraight;
	lowertotal = lowertotal + lstraight;
	lowertotal = lowertotal + yahtzee;

	cout << "Lower Section Total: " << lowertotal << "\n";
	cout << "Yahtzee Bonus: " << (yahtcount * yahtbonus * 100) << "\n\n";

	finaltotal = upcount + lowertotal;

	if (upcount >= 63) {

		finaltotal = finaltotal + 35;
	}

	finaltotal = finaltotal + (yahtcount * yahtbonus * 100);

	cout << "Final Score: " << finaltotal << "\n";
	return;
}

