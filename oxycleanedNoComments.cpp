#include <iostream>			
#include <iomanip>			
#include <string>			
#include <cmath>			
#include <chrono>			
#include <random>			
#include <thread>				
#include <map>				
#include <algorithm>		

using namespace std;	
using std::cout;
using std::cin;

random_device g_rd;												
mt19937_64 g_mt(g_rd());										

int randgen(int min, int max) {
	uniform_int_distribution<int> intdist(min, max);					
	return intdist(g_mt);												
}

double drandgen(double min, double max) {
	uniform_real_distribution<double> doubdist(min, max);					
	return doubdist(g_mt);													
}

class Player {
protected:
	short hp = 0;														
	unsigned short attlvl, strlvl, deflvl = 0;							
	unsigned short attbonus, strbonus, defbonus = 0;					
	unsigned short attstyle, strstyle, defstyle = 0;					
	unsigned int maxattroll, maxstrroll, maxdefroll = 0;				
	string name = "defaultName";										

public:

	
	Player() {
		hp = attlvl = strlvl = deflvl = attbonus = strbonus = defbonus =
		attstyle = strstyle = defstyle = maxattroll = maxstrroll = 
		maxdefroll  = 0; 
		name = "constructDefaultName";
	}

	Player(
		short chp,
		unsigned short cattlvl, unsigned short cstrlvl, unsigned short cdeflvl,
		unsigned short cattbonus, unsigned short cstrbonus, unsigned short cdefbonus,
		unsigned short cattstyle, unsigned short cstrstyle, unsigned short cdefstyle,
		unsigned int cmaxattroll, unsigned int cmaxstrroll, unsigned int cmaxdefroll, string cname) :
		hp(chp),
		attlvl(cattlvl), strlvl(cstrlvl), deflvl(cdeflvl),
		attbonus(cattbonus), strbonus(cstrbonus), defbonus(cdefbonus),
		attstyle(cattstyle), strstyle(cstrstyle), defstyle(cdefstyle),
		maxattroll(cmaxattroll), maxstrroll(cmaxstrroll), maxdefroll(cmaxdefroll), name(cname) { }
	

	~Player(){}
	
	void setstats(unsigned short& att, unsigned short& str, unsigned short& def, short& hps) {
		attlvl = att;
		strlvl = str;
		deflvl = def;
		hp = hps;
		return;
	}

	void setbonuses(string weapon) {
		bool setwep = false;
		if (weapon == "tent" || weapon == "Tent" || weapon == "tentwhip" || weapon == "Tentwhip" || weapon == "1") {
			attbonus = 90;
			strbonus = 86;
			defbonus = 0;
			setwep = true;
		}
		if (weapon == "whip" || weapon == "Whip" || weapon == "2") {
			attbonus = 82;
			strbonus = 82;
			defbonus = 0;
			setwep = true;
		}
		if (setwep == false) {
			cout << "\nERROR: Weapon not set properly. Options are 'tent' or 'whip'. You can also enter 1 or 2 accordingly.\n";
			cout << "ERROR: Please close the app and try setting weapon again.";
		}
		return;
	}

	void setcbstyle(int style) {
		if (style == 1) {				
			attstyle = 3;	
			strstyle = 0; 
			defstyle = 3;
			return;
		}
		else if (style == 2) {			
			attstyle = 1;
			strstyle = 1;
			defstyle = 3;
			return;
		}
		else { 
			cout << "ERROR setting combat stances. Please close the program." << endl;
			cin.get(); cin.get(); cin.get();	
			return;
		}
		return;
	}

	void setname(string playername) {
		name = playername;
		return;
	}

	void setmaxrolls() {
																	
		double attstat = attlvl;											
		double attstance = attstyle;										
		double attefflvl = attstat + attstance;								
		attefflvl = attefflvl + 8;											
		double equipatt = attbonus;											
		double attmax = attefflvl * (equipatt + 64);						
		maxattroll = (unsigned int)attmax;									

																	
		double strstat = strlvl;											
		double strstance = strstyle;										
		double strefflvl = strstat + strstance;								
		strefflvl = strefflvl + 8;											
		double equipstr = strbonus;											
		double strmaxhit = 0.5 + strefflvl * (equipstr + 64) / 640;			
		maxstrroll = (unsigned int)strmaxhit;								

																	
		double defstat = deflvl;											
		double defstance = defstyle;										
		double defefflvl = defstat + defstance;								
		defefflvl = defefflvl + 8;											
		double equipdef = defbonus;											
		double defmax = defefflvl * (equipdef + 64);						
		maxdefroll = (unsigned int)defmax;									

		return;
	}

	
	unsigned int rolldamage() {
		return randgen(0, maxstrroll);
	}

	short getbasehp()  { return hp; }
	unsigned short getbaseatt() { return attlvl; }
	unsigned short getbasedef() { return deflvl; }
	unsigned short getbasestr() { return strlvl; }
	string getname() { return name; }
	unsigned int gmsr() { return maxstrroll; }
	unsigned int gmar() { return maxattroll; }
	unsigned int gmdr() { return maxdefroll; }
};

int main() {

	
	cout << "\n\n\n";
	cout << " $$$$$$\\                       $$$$$$\\  $$\\                                               $$\\ \n$$  __$$\\                     $$  __$$\\ $$ |                                              $$ |\n$$ /  $$ |$$\\   $$\\ $$\\   $$\\ $$ /  \\__|$$ | $$$$$$\\   $$$$$$\\  $$$$$$$\\   $$$$$$\\   $$$$$$$ |\n$$ |  $$ |\\$$\\ $$  |$$ |  $$ |$$ |      $$ |$$  __$$\\  \\____$$\\ $$  __$$\\ $$  __$$\\ $$  __$$ |\n$$ |  $$ | \\$$$$  / $$ |  $$ |$$ |      $$ |$$$$$$$$ | $$$$$$$ |$$ |  $$ |$$$$$$$$ |$$ /  $$ |\n$$ |  $$ | $$  $$<  $$ |  $$ |$$ |  $$\\ $$ |$$   ____|$$  __$$ |$$ |  $$ |$$   ____|$$ |  $$ |\n $$$$$$  |$$  /\\$$\\ \\$$$$$$$ |\\$$$$$$  |$$ |\\$$$$$$$\\ \\$$$$$$$ |$$ |  $$ |\\$$$$$$$\\ \\$$$$$$$ |\n \\______/ \\__/  \\__| \\____$$ | \\______/ \\__| \\_______| \\_______|\\__|  \\__| \\_______| \\_______|\n                    $$\\   $$ |                                                                \n                    \\$$$$$$  |                                                                \n                     \\______/                                                                 \n\n\n";
	cout << "_______________________________________________________________________________________________\n\n";
	cout << "Utilizing Mersenne-Twister 64bit PRNG Algorithm - MT19937-64\n";
	cout << "Testing random_device for seed value 1:   " << g_rd() << '\n';
	this_thread::sleep_for(1s);				
	cout << "Testing random_device for seed value 2:   " << g_rd() << '\n';
	cout << "These values should be different every time you run the program.\n";
	cout << "It is highly unlikely, but by random chance the numbers may be the same!\n\n";
	cout << "WARNING: Some hardware does not support random_device to generate non-deterministic random numbers!\n";
	cout << "If the seeds above are the same, likely your computer does not support random_device OR the entropy pool is exhausted!\n\n";
	cout << "If the seeds are the same try waiting and then re-run the program.\nIf they are the same AGAIN I do not reccomend using the program!\n\n";
	cout << "_______________________________________________________________________________________________\n\n";
	cout << "Displaying histogram of 1,000,000 integers generated.\n\n";
	uniform_int_distribution<int> unidist(0, 10);
	map<int, int> hist;
	for (int n = 0; n < 1000000; ++n) {++hist[round(unidist(g_mt))];}
	for (auto p : hist) {cout << fixed << setprecision(1) << setw(2) << p.first << ' ' << string(p.second / 2000, '$') << '\n';}
	cout << "\nDistribution should be mostly uniform: flat & evenly distributed.\nIf this is not uniformly distributed there may be an error!\n\n";
	cout << "_______________________________________________________________________________________________\n\n";

	

	bool youexist = false;
	unsigned int simulations = 1000000;
	unsigned short youratt = 0;			
	unsigned short yourstr = 0;
	unsigned short yourdef = 0;
	short yourhp = 0;

	cout << "WARNING: Please restart the calculator if your stats change!\n\n";
	cout << "WARNING: This calculator assumes:\n   (1) You are NOT at a strength level breakpoint!\n   (2) You always swap between Accurate & Defensive!\n   (3) You and enemy are both using Abyssal Tentacle!\n\n";
	cout << "NOTE: Simulations may take a bit to calculate, please leave the console open even if it seems frozen!\n";
	cout << "NOTE: Default simulations are 1 million per scenario, you can change this value in the C++ source code.\n\n";
	cout << "_______________________________________________________________________________________________\n\n";

	
	
	while(true) {

		auto start = std::chrono::high_resolution_clock::now();						
		
		unsigned short enemyatt = 0;			
		unsigned short enemystr = 0;
		unsigned short enemydef = 0;
		short enemyhp = 0;

		double yourhc = 0.0;					
		double enemyhc = 0.0;					

		double yourdps = 0.0;					
		double enemydps = 0.0;

		int cbswap = 1;							
		int cbcontdef = 2;						


		cout << "Please enter stats EXACTLY in this format: ATT (space) STR (space) DEF (space) HP (enter)\n\n";

		
		if (youexist == false){
			cout << "Your Stats: ";
			cin >> youratt >> yourstr >> yourdef >> yourhp;
		}
		youexist = true;					

		cout << endl;
		cout << "Enemy Stats: ";
		cin >> enemyatt >> enemystr >> enemydef >> enemyhp;
		cout << endl;

		Player you;
		you.setstats(youratt, yourstr, yourdef, yourhp);
		you.setbonuses("tent");
		you.setcbstyle(cbswap);
		you.setname("You");
		you.setmaxrolls();				
		
		Player enemy;
		enemy.setstats(enemyatt, enemystr, enemydef, enemyhp);
		enemy.setbonuses("tent");
		enemy.setcbstyle(cbswap);
		enemy.setname("Enemy");
		enemy.setmaxrolls();			

		
		int maxhit_enemy_attdef = enemy.gmsr();
		enemy.setcbstyle(cbcontdef);
		enemy.setmaxrolls();
		int maxhit_enemy_contdef = enemy.gmsr();
		if (maxhit_enemy_contdef > maxhit_enemy_attdef) {
			cout << "--------------------------------------------------------------------------------------------------\n";
			cout << "  WARNING: ENEMY HAS A STRENGTH BREAKPOINT. Enemy can gain +1 Max Hit using CONTROLLED on attacks!\n";
			cout << "  We will run extra simulations for the following scenarios:\n";
			cout << "     (1) Both players swap Accurate & Defensive.\n";
			cout << "     (2) Enemy swaps Controlled & Defensive, You swap Accurate & Defensive.\n";
			cout << "--------------------------------------------------------------------------------------------------\n\n";
			enemy.setcbstyle(cbswap);	
			enemy.setmaxrolls();		
		}

		
		if (you.gmar() >= enemy.gmdr()) {
			double formulaA = (double)1.0 - ((double)enemy.gmdr() + (double)2.0) / ((double)2.0 * (double)(you.gmar() + 1.0));
			yourhc = formulaA;
		}
		else {
			double formulaB = (double)you.gmar() / ((double)2.0 * (double)enemy.gmdr() + (double)1.0);
			yourhc = formulaB;
		}
		
		if (enemy.gmar() >= you.gmdr()) {
			double formulaA_enemy = (double)1.0 - ((double)you.gmdr() + (double)2.0) / ((double)2.0 * (double)(enemy.gmar() + 1.0));
			enemyhc = formulaA_enemy;
		}
		else {
			double formulaB_enemy = (double)enemy.gmar() / ((double)2.0 * (double)you.gmdr() + (double)1.0);
			enemyhc = formulaB_enemy;
		}


		
		cout << "     ----- VERIFY THE FOLLOWING IS CORRECT -----\n";
		cout << "      YOUR STATS:  " << "ATT: " << youratt << "   STR: " << yourstr << "   DEF: " << yourdef << "   HP: " << yourhp << "\n";
		cout << "     ENEMY STATS: " << " ATT: " << enemyatt << "   STR: " << enemystr << "   DEF: " << enemydef << "   HP: " << enemyhp << "\n\n";


		
		cout << fixed << setprecision(2) << endl;
		cout << "STANDARD STATS - BOTH PLAYERS SWAP ACCURATE & DEFENSIVE\n";
		cout << "      YOUR HITRATE: " << (yourhc*100.0)<< " %" << "   YOUR MAX HIT: " << you.gmsr() << "   YOUR DPS: " << (double)yourhc * (double)(you.gmsr() / 2.0) / 2.4 << '\n';
		cout << "     ENEMY HITRATE: " << (enemyhc*100.0)<< " %" << "  ENEMY MAX HIT: " << enemy.gmsr() << "  ENEMY DPS: " << (double)enemyhc * (double)(enemy.gmsr() / 2.0) / 2.4 << "\n\n"; 
	

		
		if (maxhit_enemy_contdef > maxhit_enemy_attdef) {
			double yourhc2 = 0.0;					
			double enemyhc2 = 0.0;					
			unsigned int yourwins2 = 0;				
			unsigned int enemywins2 = 0;			
			int contdef = 2;

			double youpidwin = 0.0;
			double enemypidwin = 0.0;

			Player enemydc;
			enemydc.setstats(enemyatt, enemystr, enemydef, enemyhp);
			enemydc.setbonuses("tent");
			enemydc.setcbstyle(contdef);
			enemydc.setname("Enemy");
			enemydc.setmaxrolls();
		
			
			if (you.gmar() >= enemydc.gmdr()) {
				double formulaA = (double)1.0 - ((double)enemydc.gmdr() + (double)2.0) / ((double)2.0 * (double)(you.gmar() + 1.0));
				yourhc2 = formulaA;
			}
			else {
				double formulaB = (double)you.gmar() / ((double)2.0 * (double)enemydc.gmdr() + (double)1.0);
				yourhc2 = formulaB;
			}

			
			if (enemydc.gmar() >= you.gmdr()) {
				double formulaA_enemy = (double)1.0 - ((double)you.gmdr() + (double)2.0) / ((double)2.0 * (double)(enemydc.gmar() + 1.0));
				enemyhc2 = formulaA_enemy;
			}
			else {
				double formulaB_enemy = (double)enemydc.gmar() / ((double)2.0 * (double)you.gmdr() + (double)1.0);
				enemyhc2 = formulaB_enemy;
			}

			
			cout << fixed << setprecision(2) << endl;
			cout << "BREAKPOINT STATS - ENEMY SWAPS CONTROLLED & DEFENSIVE, YOU SWAP ACCURATE & DEFENSIVE\n";
			cout << "      YOUR HITRATE: " << (yourhc2*100.0) << " %" << "   YOUR MAX HIT: " << you.gmsr() << "   YOUR DPS: " << (double)yourhc2 * (double)(you.gmsr() / 2.0) / 2.4 << '\n';
			cout << "     ENEMY HITRATE: " << (enemyhc2*100.0) << " %" << "  ENEMY MAX HIT: " << enemydc.gmsr() << "  ENEMY DPS: " << (double)enemyhc2 * (double)(enemydc.gmsr() / 2.0) / 2.4 << '\n';
		
			cout << "\n\n--- RUNNING BREAKPOINT SIMULATIONS... PLEASE WAIT... ---\n\n";

			
			for(unsigned int i = 0; i < simulations; i++ ) {
				short yourhp = you.getbasehp();								
				short enemyhp = enemydc.getbasehp();
				bool dead = false;											

				while(!dead) {
					double randdouble1 = drandgen(0, 1);					
					double randdouble2 = drandgen(0, 1);

					if (dead == true) { break; }							
					if (yourhc2 > randdouble1) {							
						short damage = you.rolldamage();					
						enemyhp = enemyhp - damage;							
						if (enemyhp <= 0) {									
							dead = true;											
							yourwins2++;											
						}
					}
					if (dead == true) { break; }							
					if (enemyhc2 > randdouble2) {							
						short enemydmg = enemydc.rolldamage();				
						yourhp = yourhp - enemydmg;							
						if (yourhp <= 0) {									
							dead = true;										
							enemywins2++;										
						}
					}
				}
			}

			
			youpidwin = ((double)((double)yourwins2 / (double)simulations)) * 100.00;
			cout << fixed << setprecision(2) << endl;		
			cout << "(BREAKPOINT) You have PID. Enemy swaps Controlled & Defensive. Simulated 1,000,000 duels.\n";
			cout << "     Your Wins: " << yourwins2 << '\n';
			cout << "     Enemy Wins: " << enemywins2 << '\n';
			cout << "     Your chance to win: " << youpidwin << " percent.\n";

			
			yourwins2 = 0;			
			enemywins2 = 0;

			
			for (unsigned int i = 0; i < simulations; i++) {
				short yourhp = you.getbasehp();								
				short enemyhp = enemydc.getbasehp();
				bool dead = false;							

				
				while (!dead) {
					double randdouble1 = drandgen(0, 1);					
					double randdouble2 = drandgen(0, 1);

					if (dead == true) { break; }							
					if (enemyhc2 > randdouble2) {							
						short enemydmg = enemydc.rolldamage();				
						yourhp = yourhp - enemydmg;							
						if (yourhp <= 0) {									
							dead = true;										
							enemywins2++;										
						}
					}
					if (dead == true) { break; }							
					if (yourhc2 > randdouble1) {							
						short damage = you.rolldamage();					
						enemyhp = enemyhp - damage;							
						if (enemyhp <= 0) {									
							dead = true;											
							yourwins2++;											
						}
					}
				
				}
			}
			
			
			enemypidwin = ((double)((double)yourwins2 / (double)simulations)) * 100.00;
			cout << fixed << setprecision(2) << endl;
			cout << "(BREAKPOINT) Enemy has PID. Enemy swaps Controlled & Defensive. Simulated 1,000,000 duels.\n";
			cout << "     Your Wins: " << yourwins2 << '\n';
			cout << "     Enemy Wins: " << enemywins2 << '\n';
			cout << "     Your chance to win: " << enemypidwin << " percent.\n\n";

			double avgwinrate = ((enemypidwin + youpidwin) / 2.0);
			cout << "(BREAKPOINT) Average of simulation winrates: " << avgwinrate << " percent.\n";
			cout << "Assuming enemy uses Controlled & Defensive, this will be close to your average win percentage over many duels.\n";
		}

		cout << "\n\n--- RUNNING STANDARD SIMULATIONS... PLEASE WAIT... ---\n\n";


		unsigned int yourwins1 = 0;					
		unsigned int enemywins1 = 0;				
		double youpidwin1 = 0.0;					
		double enemypidwin1 = 0.0;

		
		for (unsigned int i = 0; i < simulations; i++) {
			short yourhp = you.getbasehp();								
			short enemyhp = enemy.getbasehp();
			bool dead = false;											

			while (!dead) {
				double randdouble1 = drandgen(0, 1);					
				double randdouble2 = drandgen(0, 1);

				if (dead == true) { break; }							
				if (yourhc > randdouble1) {								
					short damage = you.rolldamage();					
					enemyhp = enemyhp - damage;							
					if (enemyhp <= 0) {									
						dead = true;											
						yourwins1++;											
					}
				}
				if (dead == true) { break; }							
				if (enemyhc > randdouble2) {							
					short enemydmg = enemy.rolldamage();				
					yourhp = yourhp - enemydmg;							
					if (yourhp <= 0) {									
						dead = true;										
						enemywins1++;										
					}
				}
			}
		}

		
		youpidwin1 = ((double)((double)yourwins1 / (double)simulations)) * 100.00;
		cout << fixed << setprecision(2) << endl;
		cout << "(STANDARD) You have PID. Both swapping Accurate & Defensive. Simulated 1,000,000 duels.\n";
		cout << "     Your Wins: " << yourwins1 << '\n';
		cout << "     Enemy Wins: " << enemywins1 << '\n';
		cout << "     Your chance to win: " << youpidwin1 << " percent.\n";


		
		yourwins1 = 0;
		enemywins1 = 0;

		
		for (unsigned int i = 0; i < simulations; i++) {
			short yourhp = you.getbasehp();
			short enemyhp = enemy.getbasehp();
			bool dead = false;

			
			while (!dead) {
				double randdouble1 = drandgen(0, 1);
				double randdouble2 = drandgen(0, 1);

				if (dead == true) { break; }
				if (enemyhc > randdouble2) {
					short enemydmg = enemy.rolldamage();
					yourhp = yourhp - enemydmg;
					if (yourhp <= 0) {
						dead = true;
						enemywins1++;
					}
				}
				if (dead == true) { break; }
				if (yourhc > randdouble1) {
					short damage = you.rolldamage();
					enemyhp = enemyhp - damage;
					if (enemyhp <= 0) {
						dead = true;
						yourwins1++;
					}
				}

			}
		}

		
		enemypidwin1 = ((double)((double)yourwins1 / (double)simulations)) * 100.00;
		double avgwinrates = ((enemypidwin1 + youpidwin1) / 2.0);
		cout << fixed << setprecision(2) << endl;
		cout << "(STANDARD) Enemy has PID. Both swapping Accurate & Defensive. Simulated 1,000,000 duels.\n";
		cout << "     Your Wins: " << yourwins1 << '\n';
		cout << "     Enemy Wins: " << enemywins1 << '\n';
		cout << "     Your chance to win: " << enemypidwin1 << " percent.\n\n";

		cout << "(STANDARD) Average of simulation winrates: " << avgwinrates << " percent.\n";
		cout << "Assuming both players use Accurate & Defensive, this will be close to your average win percentage over many duels.\n";

		auto stop = std::chrono::high_resolution_clock::now();		
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);				
		cout << "_______________________________________________________________________________________________\n\n";
		cout << "               SIMULATIONS COMPLETED IN " << duration.count() << " SECONDS - RESETTING FOR NEXT OPPONENT\n";
		cout << "_______________________________________________________________________________________________\n\n";

	}

	return 0;
}
