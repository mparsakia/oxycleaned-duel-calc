#include <iostream>			// cin, cout, streams, etc.
#include <iomanip>			// formatting cout statements
#include <string>			// for string variables
#include <cmath>			// for more complex math operations
#include <chrono>			// to calculate time of execution
#include <random>			// for random_device seed and mersenne-twister PRNG
#include <thread>			// this_thread::sleep_for(1s);		
#include <map>				// container for the histogram data
#include <algorithm>		

using namespace std;	
using std::cout;
using std::cin;

// Globals for now because this actually gives a big performance boost for the randgen functions... otherwise I wouldnt use global variables


random_device g_rd;												// Construct random_device (seed generator).
mt19937_64 g_mt(g_rd());										// Construct mersenne-twister rng numbers with seed.

// Get random INT within this range inclusive [min,max]
int randgen(int min, int max) {
	uniform_int_distribution<int> intdist(min, max);					// Generate inclusive uniform distribution within range min to max.
	return intdist(g_mt);												// Pick a random INT within that distribution and return it.
}

// Get random DOUBLE within this range inclusive [min,max].. i.e. [0,1] can return between 0.00 to 1.00 ...
double drandgen(double min, double max) {
	uniform_real_distribution<double> doubdist(min, max);					// Generate inclusive uniform distribution within range min to max.
	return doubdist(g_mt);													// Pick a random DOUBLE within that distribution and return it.
}

class Player {
protected:
	short hp = 0;														// hitpoints level
	unsigned short attlvl, strlvl, deflvl = 0;							// Player's att, str, def levels.
	unsigned short attbonus, strbonus, defbonus = 0;					// Player's simple equipment bonuses.. ie: "attack slash +87, defense slash +120, strength +55" -- may add support for each att/def/str later on
	unsigned short attstyle, strstyle, defstyle = 0;					// Player's stance bonuses. Set in setcbstyle.
	unsigned int maxattroll, maxstrroll, maxdefroll = 0;				// Player's "attack roll", "max hit", "defense roll" MAX VALUES. The accuracy of hits is simulated using RNG.
	string name = "defaultName";										// Not really neccesary, but putting it in anyways so I can use it later if I ever implement hiscores lookup.

public:

	// Default constructor, sets garbage default values
	Player() {
		hp = attlvl = strlvl = deflvl = attbonus = strbonus = defbonus =
		attstyle = strstyle = defstyle = maxattroll = maxstrroll = 
		maxdefroll  = 0; 
		name = "constructDefaultName";
	}

	// Another constructor that takes arguments... we dont use it as we call functions to set these, but its here if needed!
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
	
	// Destructor
	~Player(){}


	// Set a player's Combat STATS and HP Level
	void setstats(unsigned short& att, unsigned short& str, unsigned short& def, short& hps) {
		attlvl = att;
		strlvl = str;
		deflvl = def;
		hp = hps;
		return;
	}

	// Set a player's equipment bonuses. 
	// NOTE: It's not really correct, we would need the bonus for "att & def slash,crush,etc...". TentWhip duels are 0 def bonus, so for our purpose its OK.

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

	// Sets a player's combat style bonuses.  We set this in the main function.
	void setcbstyle(int style) {
		if (style == 1) {				// Swapping Accurate and Defensive
			attstyle = 3;	
			strstyle = 0; 
			defstyle = 3;
			return;
		}
		else if (style == 2) {			// Swapping Controlled & Defensive (if it results in +1 max hit)
			attstyle = 1;
			strstyle = 1;
			defstyle = 3;
			return;
		}
		else { 
			cout << "ERROR setting combat stances. Please close the program." << endl;
			cin.get(); cin.get(); cin.get();	 // Alternative to system("pause")... Put it a couple times in case anything was still in the buffer			
			return;
		}
		return;
	}

	// Sets the players name.
	void setname(string playername) {
		name = playername;
		return;
	}

	// Sets the "maxattroll, maxstrroll, maxdefroll" of the "player"
	void setmaxrolls() {
																	// Max attack roll block
		double attstat = attlvl;											// Attack level from stats panel
		double attstance = attstyle;										// Stance bonus, i.e. Accurate (+3), Controlled (+1) ...
		double attefflvl = attstat + attstance;								// First part of effective level for Attack
		attefflvl = attefflvl + 8;											// Add the RS constant +8 to the eff level
		double equipatt = attbonus;											// Equipment bonus i.e. (Abyssal Tentacle Slash +90)
		double attmax = attefflvl * (equipatt + 64);						// The formula itself for the maximum attack roll
		maxattroll = (unsigned int)attmax;									// SET the maxattroll variable of the Player, CAST to drop decimals.

																	// Strength max hit block 
		double strstat = strlvl;											// Strength level from stats panel
		double strstance = strstyle;										// Stance bonus i.e. Aggressive (+3), Defensive (+0)...
		double strefflvl = strstat + strstance;								// First part of the effective level for Strength
		strefflvl = strefflvl + 8;											// Add the RS constant of +8 to the effective level
		double equipstr = strbonus;											// Melle Equipment Strength Bonus (Abyssal Tentacle +86 str)
		double strmaxhit = 0.5 + strefflvl * (equipstr + 64) / 640;			// The formula for calculating your MAX HIT. DECIMALS ARE DROPPED.
		maxstrroll = (unsigned int)strmaxhit;								// Set the maxstrroll var (max hit) of the Player. CAST to drop decimals.

																	// Defence max roll block
		double defstat = deflvl;											// Defence level from the stats panel
		double defstance = defstyle;										// Stance bonus i.e. Defensive (+3), Controlled (+1)...
		double defefflvl = defstat + defstance;								// First part of effective level for Defence
		defefflvl = defefflvl + 8;											// Add the constant +8 to the effective def level
		double equipdef = defbonus;											// Since this is for TentWhip fights, this will always be ZERO. ... However, if there was defense bonus, it would be "Def bonus against what your opponent is using" (i.e. If opponent is using slash, and your def bonus is slash+90, this value would be 90)
		double defmax = defefflvl * (equipdef + 64);						// The formula for the maximum defence roll
		maxdefroll = (unsigned int)defmax;									// Set the maxdefroll variable of the Player, CAST to drop decimals.

		return;
	}

	// Returns an int between [0, maxstrroll] from randgen func call
	unsigned int rolldamage() {
		return randgen(0, maxstrroll);
	}

	// Some getters...
	short getbasehp()  { return hp; }					// Get base hp level
	unsigned short getbaseatt() { return attlvl; }		// Get base attack level.
	unsigned short getbasedef() { return deflvl; }		// Get base def level.
	unsigned short getbasestr() { return strlvl; }		// Get base str level.
	string getname() { return name; }

	unsigned int gmsr() { return maxstrroll; }		// Get max strength roll
	unsigned int gmar() { return maxattroll; }		// Get max attack roll
	unsigned int gmdr() { return maxdefroll; }		// Get max def roll


};


// Formulas are split up because dobules were implicitly converting (somewhere?) to ints even with casts.
// There is some redundancy for clarity of the formulas. Some things could have just been gotten straight from the protected data.
// These aren't official RS formulas, they are Bitterkoekje's estimates.
// Bitters formulas are NOT the RSWIKI Formulas (however, they seem to be equivalent to Bitterkoekje's even though they are different).


// For Reference, the RS "Equipment" screen lists these bonuses as integers except the percent bonuses:
//		ATTACK BONUSES: STAB SLASH CRUSH MAGIC RANGE
//		DEFENCE BONUSES: STAB SLASH CRUSH MAGIC RANGE
//		OTHER BONUSES: MELEE STR, RANGE STR, MAGIC DAMAGE+%, PRAYER
//		TARGET SPECIFIC: UNDEAD+%, SLAYER+%
// When calculating your own melee max attack roll, the equipment bonus is the Attack bonus for the specific attack style you used.
//		If we are using a whip on slash we need to use the Slash bonus value from the Equipment Stats interface.
// When calculating your own melee max defence roll, the equipment bonus is the Defence bonus for the specific attack style used by the attacker (enemy).
//		If enemy is using slash, we would use the equipment bonus of OUR slash defence. (Same applies for other combat styles, except Magic is different)


// Things to keep in mind for the defense / attack rolls, and future work for the "Player" class and the program.
// The rolls depends on the "Slash, Crush, Stab, etc" armor def bonuses & what weapon style you are using... 
// This calculator DOESN'T deal with any of that ... (yet).
// This is because tent whip and abby whip give zero def bonuses, and this simulates standard "no armor, no food, no prayer, etc." duels
// When you are in the duel arena, always check for Zammy Hasta (gives def bonus), Rapier (beats TentWhip), and other common scams.
// It's impractical to calculate DDS or other duels where special attacks are used, as we can't predict when a player will spec.
// I.e. some players spec all 4 ASAP, others spec 3 times and save one, some swap to whip between hits etc...


// Because this is a simulator, and attempts to use "monte-carlo" methods to try and figure out your chance of winning is
// you may get very slightly different results against the same opponent each time..
// To increase the accuracy, run more simulations at the expense of computing time.


int main() {

	//-------------------------------OxyCleaned Console Logo and First-Run Messages---------------------------------------------
	cout << "\n\n\n";
	cout << " $$$$$$\\                       $$$$$$\\  $$\\                                               $$\\ \n$$  __$$\\                     $$  __$$\\ $$ |                                              $$ |\n$$ /  $$ |$$\\   $$\\ $$\\   $$\\ $$ /  \\__|$$ | $$$$$$\\   $$$$$$\\  $$$$$$$\\   $$$$$$\\   $$$$$$$ |\n$$ |  $$ |\\$$\\ $$  |$$ |  $$ |$$ |      $$ |$$  __$$\\  \\____$$\\ $$  __$$\\ $$  __$$\\ $$  __$$ |\n$$ |  $$ | \\$$$$  / $$ |  $$ |$$ |      $$ |$$$$$$$$ | $$$$$$$ |$$ |  $$ |$$$$$$$$ |$$ /  $$ |\n$$ |  $$ | $$  $$<  $$ |  $$ |$$ |  $$\\ $$ |$$   ____|$$  __$$ |$$ |  $$ |$$   ____|$$ |  $$ |\n $$$$$$  |$$  /\\$$\\ \\$$$$$$$ |\\$$$$$$  |$$ |\\$$$$$$$\\ \\$$$$$$$ |$$ |  $$ |\\$$$$$$$\\ \\$$$$$$$ |\n \\______/ \\__/  \\__| \\____$$ | \\______/ \\__| \\_______| \\_______|\\__|  \\__| \\_______| \\_______|\n                    $$\\   $$ |                                                                \n                    \\$$$$$$  |                                                                \n                     \\______/                                                                 \n\n\n";
	cout << "_______________________________________________________________________________________________\n\n";
	cout << "Utilizing Mersenne-Twister 64bit PRNG Algorithm - MT19937-64\n";
	cout << "Testing random_device for seed value 1:   " << g_rd() << '\n';
	this_thread::sleep_for(1s);				// Pause for a second, just in case we call g_rd() so fast we get the same seed again (unless its not supported).
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

	//-------------------------------Simulator Begins Here---------------------------------------------

	bool youexist = false;
	unsigned int simulations = 1000000;		// The number of simulations for the loops.
	unsigned short youratt = 0;				// Initializing variables for your stats
	unsigned short yourstr = 0;
	unsigned short yourdef = 0;
	short yourhp = 0;

	cout << "WARNING: Please restart the calculator if your stats change!\n\n";
	cout << "WARNING: This calculator assumes:\n   (1) You are NOT at a strength level breakpoint!\n   (2) You always swap between Accurate & Defensive!\n   (3) You and enemy are both using Abyssal Tentacle!\n\n";
	cout << "NOTE: Simulations may take a bit to calculate, please leave the console open even if it seems frozen!\n";
	cout << "NOTE: Default simulations are 1 million per scenario, you can change this value in the C++ source code.\n\n";
	cout << "_______________________________________________________________________________________________\n\n";

	
	// While (true) to reset the calculator after every simulation.
	while(true) {

		auto start = std::chrono::high_resolution_clock::now();	
		
		unsigned short enemyatt = 0;			// Initializing variables for enemy stats
		unsigned short enemystr = 0;
		unsigned short enemydef = 0;
		short enemyhp = 0;

		double yourhc = 0.0;					// Our hit chance when enemy uses "swap" - variable initialization
		double enemyhc = 0.0;					// Enemy hit chance on us when they use "swap" - variable initialization

		double yourdps = 0.0;					// Initializing variables for DPS calculations
		double enemydps = 0.0;

		int cbswap = 1;							// The function player.setcbstyle(int), takes int as a param to set the "stance" style.
		int cbcontdef = 2;						// Just defined the ints with names so its easier to understand what is being set, rather than using literals.


		cout << "Please enter stats EXACTLY in this format: ATT (space) STR (space) DEF (space) HP (enter)\n\n";

		// Only asks for your stats once.
		if (youexist == false){
			cout << "Your Stats: ";
			cin >> youratt >> yourstr >> yourdef >> yourhp;
		}
		youexist = true;					// If we've constructed "you" once, set this to true. This way we only ask for your stats once.

		cout << endl;
		cout << "Enemy Stats: ";
		cin >> enemyatt >> enemystr >> enemydef >> enemyhp;
		cout << endl;

		Player you;
		you.setstats(youratt, yourstr, yourdef, yourhp);
		you.setbonuses("tent");
		you.setcbstyle(cbswap);
		you.setname("You");
		you.setmaxrolls();				// you must always call setmaxrolls if you set or change stats, bonuses, cbstyle, as it needs to recalculate the max rolls.		
		
		Player enemy;
		enemy.setstats(enemyatt, enemystr, enemydef, enemyhp);
		enemy.setbonuses("tent");
		enemy.setcbstyle(cbswap);
		enemy.setname("Enemy");
		enemy.setmaxrolls();			

		// DISPLAY WARNING MESSAGE IF ENEMY HAS A STRENGTH BREAKPOINT
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
			enemy.setcbstyle(cbswap);	// set back to contdef at end of warning
			enemy.setmaxrolls();		// revert the max rolls (set them back to "cbswap" (Att and Def))
		}

		// CALCULATE AND SET YOUR HIT CHANCE USING APPROPRATE FORMULA. [CBSWAP, ATT AND DEF]
		if (you.gmar() >= enemy.gmdr()) {
			double formulaA = (double)1.0 - ((double)enemy.gmdr() + (double)2.0) / ((double)2.0 * (double)(you.gmar() + 1.0));
			yourhc = formulaA;
		}
		else {
			double formulaB = (double)you.gmar() / ((double)2.0 * (double)enemy.gmdr() + (double)1.0);
			yourhc = formulaB;
		}
		// CALCULATE AND SET ENEMY HIT CHANCE USING APPROPRIATE FORMULA. [CBSWAP, ATT AND DEF]
		if (enemy.gmar() >= you.gmdr()) {
			double formulaA_enemy = (double)1.0 - ((double)you.gmdr() + (double)2.0) / ((double)2.0 * (double)(enemy.gmar() + 1.0));
			enemyhc = formulaA_enemy;
		}
		else {
			double formulaB_enemy = (double)enemy.gmar() / ((double)2.0 * (double)you.gmdr() + (double)1.0);
			enemyhc = formulaB_enemy;
		}


		// DISPLAY WHAT THE USER ENTERED TO LET THEM DOUBLE CHECK THEY DIDN'T MISTYPE SOMETHING
		cout << "     ----- VERIFY THE FOLLOWING IS CORRECT -----\n";
		cout << "      YOUR STATS:  " << "ATT: " << youratt << "   STR: " << yourstr << "   DEF: " << yourdef << "   HP: " << yourhp << "\n";
		cout << "     ENEMY STATS: " << " ATT: " << enemyatt << "   STR: " << enemystr << "   DEF: " << enemydef << "   HP: " << enemyhp << "\n\n";


		// BASIC STATS FOR WHEN BOTH YOU & ENEMY USE ACCURATE AND DEFENSIVE
		cout << fixed << setprecision(2) << endl;
		cout << "STANDARD STATS - BOTH PLAYERS SWAP ACCURATE & DEFENSIVE\n";
		cout << "      YOUR HITRATE: " << (yourhc*100.0)<< " %" << "   YOUR MAX HIT: " << you.gmsr() << "   YOUR DPS: " << (double)yourhc * (double)(you.gmsr() / 2.0) / 2.4 << '\n';
		cout << "     ENEMY HITRATE: " << (enemyhc*100.0)<< " %" << "  ENEMY MAX HIT: " << enemy.gmsr() << "  ENEMY DPS: " << (double)enemyhc * (double)(enemy.gmsr() / 2.0) / 2.4 << "\n\n"; 
	

		// CHECK FOR ENEMY STRENGTH BREAKPOINT AND IF TRUE RUN SIMULATIONS AND PRINT OUTCOMES
		if (maxhit_enemy_contdef > maxhit_enemy_attdef) {
			double yourhc2 = 0.0;					// Our hit chance when enemy uses "contdef"
			double enemyhc2 = 0.0;					// Enemy hit chance when they use "contdef"
			unsigned int yourwins2 = 0;				// Counts our wins when enemy uses  "contdef" stance if they have a strength breakpoint (we still use swap)
			unsigned int enemywins2 = 0;			// Counts enemy wins if they have a strength breakpoint - "contdef"
			int contdef = 2;

			double youpidwin = 0.0;
			double enemypidwin = 0.0;

			Player enemydc;
			enemydc.setstats(enemyatt, enemystr, enemydef, enemyhp);
			enemydc.setbonuses("tent");
			enemydc.setcbstyle(contdef);
			enemydc.setname("Enemy");
			enemydc.setmaxrolls();
		
			// CALCULATE AND SET YOUR HIT CHANCE USING APPROPRATE FORMULA - BREAKPOINT
			if (you.gmar() >= enemydc.gmdr()) {
				double formulaA = (double)1.0 - ((double)enemydc.gmdr() + (double)2.0) / ((double)2.0 * (double)(you.gmar() + 1.0));
				yourhc2 = formulaA;
			}
			else {
				double formulaB = (double)you.gmar() / ((double)2.0 * (double)enemydc.gmdr() + (double)1.0);
				yourhc2 = formulaB;
			}

			// CALCULATE AND SET ENEMY HIT CHANCE USING APPROPRIATE FORMULA - BREAKPOINT
			if (enemydc.gmar() >= you.gmdr()) {
				double formulaA_enemy = (double)1.0 - ((double)you.gmdr() + (double)2.0) / ((double)2.0 * (double)(enemydc.gmar() + 1.0));
				enemyhc2 = formulaA_enemy;
			}
			else {
				double formulaB_enemy = (double)enemydc.gmar() / ((double)2.0 * (double)you.gmdr() + (double)1.0);
				enemyhc2 = formulaB_enemy;
			}

			// PRINT ADJUSTED STATS FOR BREAKPOINTS
			cout << fixed << setprecision(2) << endl;
			cout << "BREAKPOINT STATS - ENEMY SWAPS CONTROLLED & DEFENSIVE, YOU SWAP ACCURATE & DEFENSIVE\n";
			cout << "      YOUR HITRATE: " << (yourhc2*100.0) << " %" << "   YOUR MAX HIT: " << you.gmsr() << "   YOUR DPS: " << (double)yourhc2 * (double)(you.gmsr() / 2.0) / 2.4 << '\n';
			cout << "     ENEMY HITRATE: " << (enemyhc2*100.0) << " %" << "  ENEMY MAX HIT: " << enemydc.gmsr() << "  ENEMY DPS: " << (double)enemyhc2 * (double)(enemydc.gmsr() / 2.0) / 2.4 << '\n';
		
			cout << "\n\n--- RUNNING BREAKPOINT SIMULATIONS... PLEASE WAIT... ---\n\n";

			// FIGHT SIMULATION ASSUMING YOU HAVE PID - STR BREAKPOINT SIMULATION
			for(unsigned int i = 0; i < simulations; i++ ) {
				short yourhp = you.getbasehp();								// Reset hp each time the loop runs (when a simulation begins)
				short enemyhp = enemydc.getbasehp();
				bool dead = false;											// Reset the dead 'switch'

				while(!dead) {
					double randdouble1 = drandgen(0, 1);					// Get random doubles for your roll and enemy roll
					double randdouble2 = drandgen(0, 1);

					if (dead == true) { break; }							// Need this break statement for proper "deaths" count
					if (yourhc2 > randdouble1) {							// If you successfully roll the chance to hit...
						short damage = you.rolldamage();					// Roll what your damage dealt will be...
						enemyhp = enemyhp - damage;							// Deal it to the enemy's hitpoints...
						if (enemyhp <= 0) {									// Check if the enemy's HP is less than or equal to zero (if they died)
							dead = true;											// The enemy died! Set the bool dead to true to stop the while loop.
							yourwins2++;											// Increment your wins
						}
					}
					if (dead == true) { break; }							// Need this break statement for proper "deaths" count
					if (enemyhc2 > randdouble2) {							// Now we see if the enemy gets to hit...
						short enemydmg = enemydc.rolldamage();				// If enemy hits, what damage will you take...
						yourhp = yourhp - enemydmg;							// Enemy deals damage to you...
						if (yourhp <= 0) {									// Check to see if you are dead or not.
							dead = true;										// You died! Set bool dead to true to stop the while loop
							enemywins2++;										// Increment enemy wins
						}
					}
				}
			}

			// PRINT OUT THE SIMULATION FOR YOUR PID - BREAKPOINT
			youpidwin = ((double)((double)yourwins2 / (double)simulations)) * 100.00;
			cout << fixed << setprecision(2) << endl;		
			cout << "(BREAKPOINT) You have PID. Enemy swaps Controlled & Defensive. Simulated 1,000,000 duels.\n";
			cout << "     Your Wins: " << yourwins2 << '\n';
			cout << "     Enemy Wins: " << enemywins2 << '\n';
			cout << "     Your chance to win: " << youpidwin << " percent.\n";

			//RESET THE COUNTER VARIABLES BEFORE NEXT SIMULATION
			yourwins2 = 0;			
			enemywins2 = 0;

			// FIGHT SIMULATION ASSUMING ENEMY PID - STR BREAKPOINT SIMULATION
			for (unsigned int i = 0; i < simulations; i++) {
				short yourhp = you.getbasehp();								
				short enemyhp = enemydc.getbasehp();
				bool dead = false;							

				// Swapped the block order so enemy hits first, this simulates enemy having PID.
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
			
			// PRINT OUT THE SIMULATION FOR ENEMY PID - BREAKPOINT
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


		unsigned int yourwins1 = 0;					// Counts our wins when we use "swap" stance
		unsigned int enemywins1 = 0;				// Counts enemy wins when they use "swap" stance
		double youpidwin1 = 0.0;					// Holds percentage calculation after simulations
		double enemypidwin1 = 0.0;

		// FIGHT SIMULATION ASSUMING YOUR PID - STANDARD SIMULATION
		for (unsigned int i = 0; i < simulations; i++) {
			short yourhp = you.getbasehp();								// Reset hp each time the loop runs (after a simulation ends / begins)
			short enemyhp = enemy.getbasehp();
			bool dead = false;											// Reset the dead 'switch'

			while (!dead) {
				double randdouble1 = drandgen(0, 1);					// Get random doubles for your roll and enemy roll
				double randdouble2 = drandgen(0, 1);

				if (dead == true) { break; }							// Need this break statement for proper "deaths" count
				if (yourhc > randdouble1) {								// If you successfully roll the chance to hit...
					short damage = you.rolldamage();					// Roll what your damage dealt will be
					enemyhp = enemyhp - damage;							// Deal it to the enemy's hitpoints
					if (enemyhp <= 0) {									// Check if the enemy's HP is less than or equal to zero (if they died)
						dead = true;											// The enemy died! Set the bool dead to true to stop the while loop.
						yourwins1++;											// Increment your wins
					}
				}
				if (dead == true) { break; }							// Need this break statement for proper "deaths" count
				if (enemyhc > randdouble2) {							// Now we see if the enemy gets to hit
					short enemydmg = enemy.rolldamage();				// Roll what damage dealt to you will be
					yourhp = yourhp - enemydmg;							// Enemy deals damage to you
					if (yourhp <= 0) {									// Check to see if you are dead or not.
						dead = true;										// You died! Set bool to true, end the loop.
						enemywins1++;										// Increment enemy wins
					}
				}
			}
		}

		// PRINT SIMULATION OUTCOMES FOR STANDARD SIMULATION WHERE YOU HAVE PID
		youpidwin1 = ((double)((double)yourwins1 / (double)simulations)) * 100.00;
		cout << fixed << setprecision(2) << endl;
		cout << "(STANDARD) You have PID. Both swapping Accurate & Defensive. Simulated 1,000,000 duels.\n";
		cout << "     Your Wins: " << yourwins1 << '\n';
		cout << "     Enemy Wins: " << enemywins1 << '\n';
		cout << "     Your chance to win: " << youpidwin1 << " percent.\n";


		// RESET THE COUNTER VARIABLES BEFORE NEXT SIMULATION
		yourwins1 = 0;
		enemywins1 = 0;

		// FIGHT SIMULATION ASSUMING ENEMY PID - STANDARD SIMULATION
		for (unsigned int i = 0; i < simulations; i++) {
			short yourhp = you.getbasehp();
			short enemyhp = enemy.getbasehp();
			bool dead = false;

			// Swapped the block order so enemy hits first, this simulates enemy having PID.
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

		// PRINT SIMULATION OUTCOMES FOR STANDARD SIMULATION WHERE ENEMY HAS PID
		enemypidwin1 = ((double)((double)yourwins1 / (double)simulations)) * 100.00;
		double avgwinrates = ((enemypidwin1 + youpidwin1) / 2.0);
		cout << fixed << setprecision(2) << endl;
		cout << "(STANDARD) Enemy has PID. Both swapping Accurate & Defensive. Simulated 1,000,000 duels.\n";
		cout << "     Your Wins: " << yourwins1 << '\n';
		cout << "     Enemy Wins: " << enemywins1 << '\n';
		cout << "     Your chance to win: " << enemypidwin1 << " percent.\n\n";

		cout << "(STANDARD) Average of simulation winrates: " << avgwinrates << " percent.\n";
		cout << "Assuming both players use Accurate & Defensive, this will be close to your average win percentage over many duels.\n";

		auto stop = std::chrono::high_resolution_clock::now();		// Stop timer
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
		cout << "_______________________________________________________________________________________________\n\n";
		cout << "               SIMULATIONS COMPLETED IN " << duration.count() << " SECONDS - RESETTING FOR NEXT OPPONENT\n";
		cout << "_______________________________________________________________________________________________\n\n";

	}

	return 0;
}
