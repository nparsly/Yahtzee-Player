#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <time.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

//Nicholas Parsly
using namespace std;

map <string, double> probs;

//Now, I admit this code isn't pretty, but I wrote it in an hour between classes
void calcProb(int size) {

	//This utilizes the fact that adding an additional roll onto any combination or prior rolls multiplies the odds
	//by 1/6. (e.g. going from ordered 1124 to ordered 11245 just involves multiplying the odds of 1124 by 1/6)
	//
	//It adds the odds for different possible combinations for the same roll together to get the total odds of a roll
	//(e.g. 112 + 4 and 114 + 2 both result in the same roll, so we have to add their odds together. We do not
	//have to add both 121 + 4 and 112 + 4 because we already counted them together when finding the total odds for 112)
	if (size > 5) {

		return;
	}

	string s;
	string cps1;
	string cps2;
	double curprob;
	map <string, double>::iterator mit;


	for (int i = 1; i < 7; i++) {

		s.resize(1);
		s.at(0) = i + '0';

		if (size > 1) {

			cps1 = s;
			sort(cps1.begin(), cps1.end());

			for (int j = 1; j < 7; j++) {

				curprob = probs.find(cps1)->second;
				s.resize(2);
				s.at(1) = j + '0';

				if (size > 2) {

					cps1 = s;
					sort(cps1.begin(), cps1.end());

					//This is the code that for ignoring repeat orderings like in the 112 + 4 and 121 + 4 example
					//Essentially, if the number we're extending is not in canonical ordering, we ignore it so we don't
					//double count
					if (cps1 != s) {

						continue;
					}
					for (int k = 1; k < 7; k++) {

						curprob = probs.find(cps1)->second;
						s.resize(3);
						s.at(2) = k + '0';

						if (size > 3) {

							cps1 = s;
							sort(cps1.begin(), cps1.end());

							if (cps1 != s) {

								continue;
							}
							for (int m = 1; m < 7; m++) {

								curprob = probs.find(cps1)->second;
								s.resize(4);
								s.at(3) = m + '0';

								if (size > 4) {

									cps1 = s;
									sort(cps1.begin(), cps1.end());
									if (cps1 != s) {

										continue;
									}
									for (int n = 1; n < 7; n++) {

										curprob = probs.find(cps1)->second;
										s.resize(5);
										s.at(4) = n + '0';
										curprob = curprob * (1.0 / 6.0);
										
										cps2 = s;
										sort(cps2.begin(), cps2.end());

										mit = probs.find(cps2);

										if (mit != probs.end()) {
											
											mit->second = mit->second + curprob;
										}

										else {

											probs.insert(make_pair(cps2, curprob));

										}
									}
								}

								else {

									curprob = curprob * (1.0 / 6.0);

									cps2 = s;
									sort(cps2.begin(), cps2.end());

									mit = probs.find(cps2);

									if (mit != probs.end()) {


										mit->second = mit->second + curprob;
									}

									else {

										probs.insert(make_pair(cps2, curprob));
									}
								}
							}
						}

						else {

							curprob = curprob * (1.0 / 6.0);

							cps2 = s;
							sort(cps2.begin(), cps2.end());

							mit = probs.find(cps2);

							if (mit != probs.end()) {

								mit->second = mit->second + curprob;
							}

							else {

								probs.insert(make_pair(cps2, curprob));
							}
						}
					}
				}

				else {

					curprob = curprob * (1.0 / 6.0);

					cps2 = s;
					sort(cps2.begin(), cps2.end());

					mit = probs.find(cps2);

					if (mit != probs.end()) {

						mit->second = mit->second + curprob;
					}

					else {

						probs.insert(make_pair(cps2, curprob));
					}
				}
			}
		}

		else {

			probs.insert(make_pair(s, (1.0 / 6.0)));			
		}
	}

	calcProb(size + 1);
}

int main() {


	calcProb(1);
	map <string, double>::iterator mit;

	double s1 = 0.0;
	double s2 = 0.0;
	double s3 = 0.0;
	double s4 = 0.0;
	double s5 = 0.0;
	int count1;
	int count2;
	int count3;
	int count4;
	int count5;
	int count6;
	ofstream fout;
	fout.open("yzprobs.txt");

	for (mit = probs.begin(); mit != probs.end(); mit++) {

		count1 = 0;
		count2 = 0;
		count3 = 0;
		count4 = 0;
		count5 = 0;
		count6 = 0;
		//Sanity check code
		if (mit->first.size() == 1) {

			s1 = s1 + mit->second;
		}

		else if (mit->first.size() == 2) {

			s2 = s2 + mit->second;
		}

		else if (mit->first.size() == 3) {

			s3 = s3 + mit->second;
		}

		else if (mit->first.size() == 4) {

			s4 = s4 + mit->second;
		}

		else if (mit->first.size() == 5) {

			s5 = s5 + mit->second;
		}

		cout << mit->first << ": " << mit->second << "\n";

		for (int i = 0; i < mit->first.size(); i++) {

			if (mit->first.at(i) == '1') {

				count1 = count1 + 1;
			}

			else if (mit->first.at(i) == '2') {

				count2 = count2 + 1;
			}

			else if (mit->first.at(i) == '3') {

				count3 = count3 + 1;
			}

			else if (mit->first.at(i) == '4') {

				count4 = count4 + 1;
			}

			else if (mit->first.at(i) == '5') {

				count5 = count5 + 1;
			}

			else if (mit->first.at(i) == '6') {

				count6 = count6 + 1;
			}
		}

		//Output file information
		fout << mit->first << " " << mit->second << " ";
		fout << count1 << " " << count2 << " " << count3 << " " << count4 << " " << count5 << " " << count6 << "\n";
	}

	cout << "S1: " << s1 << "\n";
	cout << "S2: " << s2 << "\n";
	cout << "S3: " << s3 << "\n";
	cout << "S4: " << s4 << "\n";
	cout << "S5: " << s5 << "\n";
	return 0;
}

